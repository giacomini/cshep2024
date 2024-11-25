#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include "benchmark.hpp"

struct P1
{
  int a;
  double b;
  int c;
};

struct P2
{
  int a;
  int c;
  double b;
};

auto make_vector(int N)
{
  std::random_device rd;
  std::default_random_engine eng{rd()};

  std::uniform_real_distribution<> dist{-1., 1.};

  std::vector<P1> result;
  result.reserve(N);
  std::generate_n(std::back_inserter(result), N, [&] {
    return P1{1, dist(eng), 2};
  });

  return result;
}

int main()
{
  std::cout << "sizeof(P1): " << sizeof(P1) << '\n';
  int const N{100'000'000};
  auto const v = make_vector(N);
  auto v1 = v;
  std::cout << "sorting a vector<P1> in " << benchmark([&] {
    std::sort(
      v1.begin(),
      v1.end(),  //
      [](auto& e1, auto& e2) { return e1.b < e2.b; }
    );
  }).count() << " s\n";

  std::cout << "sizeof(P2): " << sizeof(P2) << '\n';
  std::vector<P2> v2;
  v2.reserve(v.size());
  std::transform(v.begin(), v.end(), std::back_inserter(v2), [](auto& p1) { return P2{p1.a, p1.c, p1.b}; });
  std::cout << "sorting a vector<P2> in " << benchmark([&] {
    std::sort(
      v1.begin(),
      v1.end(),  //
      [](auto& e1, auto& e2) { return e1.b < e2.b; }
    );
  }).count() << " s\n";

}