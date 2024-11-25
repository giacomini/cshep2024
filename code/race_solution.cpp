#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

auto with_atomic()
{
  std::atomic<int> count{0};
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
  std::mutex m;

  std::thread t1{[&v, &m, n] {
    for (int i{0}; i != n / 2; ++i) {
      std::scoped_lock l(m);
      v.push_back(0);
    }
  }};
  std::thread t2{[&v, &m, n] {
    for (int i{0}; i != n / 2; ++i) {
      std::scoped_lock l(m);
      v.pop_back();
    }
  }};

  t2.join();
  t1.join();

  std::cout << static_cast<int>(v.size()) - n / 2 << " (should be 0)\n";
}

int main()
{
  with_atomic();
  with_mutex();
}
