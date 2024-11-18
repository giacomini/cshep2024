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

struct ParticleExt
{
  Ext ext;
};

class Particle
{
  Vec position_;
  std::unique_ptr<ParticleExt> ext_;
 public:
  Particle(Vec const& p) : position_(p), ext_(new ParticleExt{}) {}
  void translate(Vec const& t) { position_ += t; }
};

using Particles = std::vector<Particle>;

void translate(Particles& particles, Vec const& t)
{
  std::for_each(particles.begin(), particles.end(), [=](Particle& p) {
      p.translate(t);
    });
}

template<class G>
auto make_vector(int N, G g)
{
  std::vector<Particle> result;
  result.reserve(N);
  std::generate_n(std::back_inserter(result), N, [&] {
    return Particle{{g(), g()}};
  });

  return result;
}

int main()
{
  std::random_device rd;
  std::default_random_engine e{rd()};
  std::uniform_real_distribution<> d;

  int const N = 1'000'000;
  auto particles = make_vector(N, [&] { return d(e); });

  auto t = benchmark([&] {
    for (int i = 0; i != 1000; ++i) {
      Vec t{d(e), d(e)};
      translate(particles, t);
    }
  });
  std::cout << t.count() << " s\n";
}
