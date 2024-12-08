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

  // multiply all the elements of the vector
  std::cout << "product = "
            << std::accumulate(std::begin(v), std::end(v), 1LL, std::multiplies<>{})
            << '\n';

  // sort the vector in descending order
  std::sort(std::begin(v), std::end(v), std::greater<>{});
  // or
  // std::sort(std::begin(v), std::end(v), [](int i, int j) { return j < i; });
  std::cout << "sorted: " << v << '\n';

  // move the even numbers at the beginning of the vector
  auto first_odd = std::partition(std::begin(v), std::end(v), [](int i) { return i % 2 == 0;});
  std::cout << "partitioned: " << v << '\n';
  std::cout << "first odd number at position "
            << std::distance(std::begin(v), first_odd)
            << '\n';

  // create another vector with the squares of the numbers in the first vector
  std::vector<int> v2;
  std::transform(std::begin(v), std::end(v), std::back_inserter(v2), [](int n) { return n * n; });
  std::cout << "squares: " << v2 << '\n';

  // find the first multiple of 3 or 7
  auto found_it = std::find_if(
      std::begin(v),
      std::end(v),
      [](int i) { return i % 3 == 0 || i % 7 == 0;}
  );
  std::cout << "first multiple of 3 or 7 at position "
            << std::distance(std::begin(v), found_it)
            << '\n';

  // erase from the vector all the multiples of 3 or 7
  v.erase(
      std::remove_if(
          std::begin(v)
          , std::end(v)
          , [](int i) { return (i % 3) == 0 || (i % 7) == 0;}
      ),
      std::end(v)
  );
  std::cout << "multiples of 3 or 7 erased: " << v << '\n';
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
