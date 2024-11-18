#include "benchmark.hpp"
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <iterator>
#include <memory>
#include <cassert>

struct Vec
{
  double x, y;
  Vec& operator+=(Vec const& o) {
    x += o.x;
    y += o.y;
    return *this;
  }
};

#ifndef EXTSIZE
#define EXTSIZE 0
#endif

constexpr int const ExtSize = EXTSIZE;

using Ext = std::array<char, ExtSize>;

struct Particles
{
  std::vector<Vec> positions;
  std::vector<Ext> exts;
};

void translate(Particles& particles, Vec const& t)
{
  auto& positions = particles.positions; 
  std::for_each(positions.begin(), positions.end(), [=](Vec& p) {
      p += t;
    });
}

template<class G>
auto make_positions(int N, G g)
{
  std::vector<Vec> result;
  result.reserve(N);
  std::generate_n(std::back_inserter(result), N, [&] {
    return Vec{g(), g()};
  });

  return result;
}

int main()
{
  std::random_device rd;
  std::default_random_engine e{rd()};
  std::uniform_real_distribution<> d;

  int const N = 1'000'000;
  Particles particles{
    make_positions(N, [&] { return d(e); }),
    std::vector<Ext>(N)
  };

  assert(particles.positions.size() == particles.exts.size());

  auto t = benchmark([&] {
    for (int i = 0; i != 1000; ++i) {
      Vec t{d(e), d(e)};
      translate(particles, t);
    }
  });
  std::cout << t.count() << " s\n";
}
