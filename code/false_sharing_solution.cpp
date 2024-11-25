#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <new>
#include <random>
#include <span>
#include <thread>
#include <vector>

auto f(std::span<double> s)
{
  for (int i{0}; i != 100'000'000; ++i) {
    for (auto&& e : s) {
      e = 1. / e;
    }
  }
}

auto generate(int n)
{
  std::vector<double> v;
  std::default_random_engine eng;
  std::uniform_real_distribution<> dist{0., 1.};
  v.reserve(n);
  std::generate_n(std::back_inserter(v), n, [&] { return dist(eng); });

  return v;
}

int main()
{
  int const n = 28;
  std::vector<double> v = generate(n);

  auto const cache_line_size = 64; // std::hardware_destructive_interference_size
  auto const N = cache_line_size / sizeof(double);

  auto it0 = v.begin();
  auto it1 = it0 + N;
  auto it2 = it0 + 2 * N;
  auto it3 = it0 + 3 * N;
  assert(it3 <= v.end());
  auto it4 = v.end();

  std::vector<std::thread> vth;
  vth.reserve(4);

  auto t0 = std::chrono::system_clock::now();

  vth.emplace_back(std::thread{[s = std::span<double>{it0, it1}] { f(s); }});
  vth.emplace_back(std::thread{[s = std::span<double>{it1, it2}] { f(s); }});
  vth.emplace_back(std::thread{[s = std::span<double>{it2, it3}] { f(s); }});
  vth.emplace_back(std::thread{[s = std::span<double>{it3, it4}] { f(s); }});

  for (auto&& th : vth) {
    th.join();
  }

  auto t1 = std::chrono::system_clock::now();

  std::cout << std::chrono::duration<double>{t1 - t0}.count() << " s\n";
}