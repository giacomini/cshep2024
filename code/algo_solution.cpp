#include <random>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <functional>
#include <cassert>

std::ostream& operator<<(std::ostream& os, std::vector<int> const& c);
std::vector<int> make_vector(int N);

int main()
{
  std::vector<int> v = make_vector(10);
  std::cout << v << '\n';

  // sum all the elements of the vector
  std::cout << "sum = "
            << std::accumulate(std::begin(v), std::end(v), 0)
            << '\n';

  // average of the first half of the vector
  auto mid_it = std::begin(v) + v.size() / 2;
  std::cout << "average 1st half = "
            << 1. * std::accumulate(std::begin(v), mid_it, 0) / std::distance(std::begin(v), mid_it)
            << '\n';

  // average of the second half of the vector
  std::cout << "average 2nd half = "
            << 1. * std::accumulate(mid_it, std::end(v), 0) / std::distance(mid_it, std::end(v))
            << '\n';

  // move the three central elements to the beginning of the vector
  assert(v.size() >= 3);
  auto n_first = std::prev(mid_it);
  auto last = std::next(mid_it, 2);
  std::rotate(std::begin(v), n_first, last);
  std::cout << "rotated: " << v << '\n';

  // remove duplicate elements
  // first sort the vector
  std::sort(std::begin(v), std::end(v));
  // then remove consecutive duplicate elements
  std::vector<int> unique_v;
  std::unique_copy(std::begin(v), std::end(v), std::back_inserter(unique_v));
  std::cout << "unique: " << unique_v << '\n';
}

std::ostream& operator<<(std::ostream& os, std::vector<int> const& c)
{
  os << "{ ";
  std::copy(
            std::begin(c),
            std::end(c),
            std::ostream_iterator<int>{os, " "}
            );
  os << '}';

  return os;
}

std::vector<int> make_vector(int N)
{
  // define a pseudo-random number generator engine and seed it using an actual
  // random device
  std::random_device rd;
  std::default_random_engine eng{rd()};

  int const MAX_N = 100;
  std::uniform_int_distribution<int> dist{1, MAX_N};

  std::vector<int> result;
  result.reserve(N);
  std::generate_n(std::back_inserter(result), N, [&] { return dist(eng); });

  return result;
}
