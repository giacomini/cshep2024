#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>

template <class F, class P = std::ratio<1>>
auto benchmark(F f, P = P{})
{
  auto t0{std::chrono::steady_clock::now()};

  f();

  auto t1{std::chrono::steady_clock::now()};

  return std::chrono::duration<double, P>(t1 - t0);
}

#endif
