#include <oneapi/tbb.h>
#include <atomic>
#include <chrono>
#include <iostream>
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
    double sum{};

    auto const bench = benchmark(
        [&] {
          tbb::simple_partitioner partitioner{};
          sum = tbb::parallel_reduce(
              tbb::blocked_range<std::size_t>{0, v.size(), G},
              sum,
              [&](tbb::blocked_range<std::size_t> const& r, auto init) {
                ++count;
                auto const first = v.begin() + r.begin();
                auto const last = v.begin() + r.end();
                return init + std::reduce(first, last);
              },
              [](auto lhs, auto rhs) { return lhs + rhs; },
              partitioner);
        },
        std::milli{});

    std::cout << "sum " << sum << ", G " << G << ", " << count
              << " invocations, " << bench.count() << " ms\n";
  }

  std::cout << "\n=== Min ===\n";
  for (std::size_t G :
       {1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000}) {
    std::atomic<int> count{};
    auto min = v.begin();
    auto const bench = benchmark(
        [&] {
          tbb::simple_partitioner partitioner{};
          min = tbb::parallel_reduce(
              tbb::blocked_range<std::size_t>{0, v.size(), G},
              v.begin(),
              [&](tbb::blocked_range<std::size_t> const& r, auto init) {
                ++count;
                auto const first = v.begin() + r.begin();
                auto const last = v.begin() + r.end();
                auto it = std::min_element(first, last);
                return *it < *init ? it : init;
              },
              [](auto lhs, auto rhs) { return *lhs < *rhs ? lhs : rhs; },
              partitioner);
        },
        std::milli{});

    std::cout << "position " << std::distance(v.begin(), min) << ", min "
              << *min << ", G " << G << ", " << count << " invocations, "
              << bench.count() << " ms\n";
  }
}
