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

MutexGuarded<int> guarded(42);

void add()
{
  guarded([](int &value)
          { ++value; });
}

int main()
{
  std::thread t1(add);
  std::thread t2(add);

  t1.join();
  t2.join();

  guarded([](const int &value)
          { std::cout << "Final value: " << value << std::endl; });

  return 0;
}
