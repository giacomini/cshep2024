#include <random>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>

std::ostream& operator<<(std::ostream& os, std::vector<int> const& c);
std::vector<int> make_vector(int N);

int main()
{
  // create a vector of N elements, generated randomly
  int const N = 10;
  std::vector<int> v = make_vector(N);
  std::cout << v << '\n';

  // multiply all the elements of the vector
  // use std::accumulate

  // sort the vector in descending order
  // use std::sort

  // move the even numbers at the beginning of the vector
  // use std::partition

  // create another vector with the squares of the numbers in the first vector
  // use std::transform

  // find the first multiple of 3 or 7
  // use std::find_if

  // erase from the vector all the multiples of 3 or 7
  // use std::remove_if followed by vector::erase
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
