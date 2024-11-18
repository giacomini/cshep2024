#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <iterator>
#include <tuple>
#include "benchmark.hpp"

#ifndef EXTSIZE
#define EXTSIZE 0
#endif

constexpr int const ExtSize = EXTSIZE;

struct S
{
  int n;
  char ext[ExtSize]{};
};

auto make_vector(int N)
{
  std::random_device rd;
  std::default_random_engine eng{rd()};
  std::uniform_int_distribution<> dist;

  std::vector<S> result;
  result.reserve(N);
  std::generate_n(std::back_inserter(result), N, [&] {
    return S{dist(eng)};
  });

  return result;
}

int main()
{
  int const N = 1'0000'000;

  auto v = make_vector(N);

  auto t = benchmark(
    [&] { std::sort(v.begin(), v.end(), [](S const& l, S const& r) { return l.n < r.n; }); }
  );

  std::cout << t.count() << " s\n";
}
