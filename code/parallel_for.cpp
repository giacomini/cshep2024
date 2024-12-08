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
  std::uniform_real_distribution dist(0., 1.);
  std::vector<double> v;
  v.reserve(n);
  std::generate_n(
      std::back_inserter(v), v.capacity(), [&] { return dist(eng); });

  return v;
}
}  // namespace

int main()
{
  std::cout << "Generating... " << std::flush;
  auto v{generate(100'000'000)};
  std::cout << "done\n";

  for (std::size_t G : {1, 10, 100, 1'000, 10'000, 100'000, 1'000'000}) {
    std::atomic<int> count{};
    auto bench = benchmark([&] {
      tbb::simple_partitioner partitioner{};
      tbb::parallel_for(
          tbb::blocked_range<std::size_t>{0, v.size(), G},
          [&](auto const& r) {
            ++count;
            auto const first = v.begin() + r.begin();
            auto const last = v.begin() + r.end();
            std::transform(first, last, first, [=](auto d) {
              for (int i{0}; i != 10; ++i) {
                d = std::pow(std::log(d), std::sqrt(d));
              }
              return d;
            });
          },
          partitioner);
    });
    std::cout << "G " << G << ", " << count << " invocations, " << bench.count()
              << " s\n";
  }
}
