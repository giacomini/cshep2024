#include <iostream>
#include <thread>
#include <vector>

auto with_atomic()
{
  int count{0};
  int const n = 1'000'000;
  std::thread t1{[&count, n] {
    for (int i{0}; i != n; ++i) {
      ++count;
    }
  }};
  std::thread t2{[&count, n] {
    for (int i{0}; i != n; ++i) {
      --count;
    }
  }};

  t2.join();
  t1.join();

  std::cout << count << " (should be 0)\n";
}

auto with_mutex()
{
  int const n = 1'000'000;
  std::vector<int> v{};
  v.reserve(n);
  v.resize(n / 2);

  std::thread t1{[&v, n] {
    for (int i{0}; i != n / 2; ++i) {
      v.push_back(0);
    }
  }};
  std::thread t2{[&v, n] {
    for (int i{0}; i != n / 2; ++i) {
      v.pop_back();
    }
  }};

  t2.join();
  t1.join();

  std::cout << static_cast<int>(v.size()) - n / 2  << " (should be 0)\n";
}

int main()
{
  with_atomic();
  with_mutex();
}
