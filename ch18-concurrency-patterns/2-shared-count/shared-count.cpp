#include <iostream>
#include <mutex>
#include <thread>

template <typename T>
class MutexGuarded
{
public:
  MutexGuarded() = default;

  template <typename... Args>
  explicit MutexGuarded(Args &&...args) : m_value(std::forward<Args>(args)...)
  {
  }

  template <typename F>
  decltype(auto) operator()(F &&f)
  {
    std::lock_guard lock(m_mutex);
    return f(m_value);
  }

private:
  std::mutex m_mutex;
  T m_value;
};

MutexGuarded<std::size_t> counter;

void events(unsigned int value)
{
  for (std::size_t i = 0; i < 100; ++i)
  {
    if (rand_r(&value) % 10 == 0)
    {
      counter([](std::size_t &v)
              { ++v; });
    }
  }
}

int main()
{
  std::thread t1(events, 1);
  std::thread t2(events, 2);
  t1.join();
  t2.join();
  counter([](int i)
          { std::cout << i << std::endl; });
}
