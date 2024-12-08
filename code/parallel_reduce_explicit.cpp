#include <oneapi/tbb.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include "benchmark.hpp"

namespace {

auto generate(int n)
{
  std::default_random_engine eng{};
  std::uniform_real_distribution dist(-1., 1.);
  std::vector<double> v;
  v.reserve(n);
  std::generate_n(
      std::back_inserter(v), v.capacity(), [&] { return dist(eng); });

  return v;
}

class Sum
{
  std::vector<double>::const_iterator begin_;
  double sum_{};
  std::atomic<int>& count_;

 public:
  void operator()(const tbb::blocked_range<size_t>& r)
  {
    ++count_;
    auto const first = begin_ + r.begin();
    auto const last = begin_ + r.end();
    sum_ += std::reduce(first, last);
  }

  Sum(std::vector<double> const& v, std::atomic<int>& c)
      : begin_{v.begin()}, count_{c}
  {
  }
  Sum(Sum const& o, tbb::split) : begin_{o.begin_}, count_{o.count_}
  {
  }

  void join(Sum const& o)
  {
    sum_ += o.sum_;
  }

  auto result() const
  {
    return sum_;
  }
  int count() const
  {
    return count_;
  }
};

class Min
{
  std::vector<double>::const_iterator begin_;
  std::vector<double>::const_iterator min_{begin_};
  std::atomic<int>& count_;

 public:
  void operator()(tbb::blocked_range<std::size_t> const& r)
  {
    ++count_;
    auto const first = begin_ + r.begin();
    auto const last = begin_ + r.end();
    auto it = std::min_element(first, last);
    if (*it < *min_) {
      min_ = it;
    }
  }

  Min(std::vector<double> const& v, std::atomic<int>& c)
      : begin_{v.begin()}, count_{c}
  {
  }
  Min(Min const& o, tbb::split) : begin_{o.begin_}, count_{o.count_}
  {
  }

  void join(Min const& o)
  {
    if (*o.min_ < *min_) {
      min_ = o.min_;
    }
  }

  auto result() const
  {
    return min_;
  }
  int count() const
  {
    return count_;
  }
};

}  // namespace

int main()
{
  int const n{10'000'000};
  std::cout << "Generating " << n << " numbers... " << std::flush;
  auto const v = generate(n);
  std::cout << "done\n";

  std::cout << "\n=== Sum ===\n";
  for (std::size_t G :
       {1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000}) {
    std::atomic<int> count{};
    Sum sum(v, count);

    auto const bench = benchmark(
        [&] {
          tbb::simple_partitioner partitioner{};
          tbb::parallel_reduce(tbb::blocked_range<std::size_t>{0, v.size(), G},
                               sum,
                               partitioner);
        },
        std::milli{});

    std::cout << "sum " << sum.result() << ", G " << G << ", " << sum.count()
              << " invocations, " << bench.count() << " ms\n";
  }

  std::cout << "\n=== Min ===\n";
  for (std::size_t G :
       {1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000}) {
    std::atomic<int> count{};
    Min min(v, count);

    auto const bench = benchmark(
        [&] {
          tbb::simple_partitioner partitioner{};
          tbb::parallel_reduce(tbb::blocked_range<std::size_t>{0, v.size(), G},
                               min,
                               partitioner);
        },
        std::milli{});

    std::cout << "position " << std::distance(v.begin(), min.result())
              << ", min " << *min.result() << ", G " << G << ", " << min.count()
              << " invocations, " << bench.count() << " ms\n";
  }
}
