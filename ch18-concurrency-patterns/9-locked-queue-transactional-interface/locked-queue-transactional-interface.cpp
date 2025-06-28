#include <queue>
#include <mutex>
#include <optional>
#include <thread>
#include <utility>
#include <iostream>

template <typename T>
class ts_queue
{
public:
  ts_queue() = default;

  template <typename U>
  void push(U &&value)
  {
    std::lock_guard lock(m_mutex);
    m_queue.push(std::forward<U>(value));
  }

  std::optional<T> pop()
  {
    std::lock_guard lock(m_mutex);
    if (m_queue.empty())
      return std::nullopt;

    T value = std::move(m_queue.front());
    m_queue.pop();
    return std::optional<T>(std::move(value));
  }

private:
  std::mutex m_mutex;
  std::queue<T> m_queue;
};

ts_queue<int> queue;
std::mutex mutex;

void producer()
{
  queue.push(42);
}

void consumer()
{
  auto value = queue.pop();
  if (value)
  {
    std::lock_guard lock(mutex);
    std::cout << "Consumed value: " << *value << std::endl;
  }
}

int main()
{
  // Launch producers.
  constexpr size_t npthread = 5;
  std::thread pt[npthread];
  for (size_t i = 0; i != npthread; ++i)
    pt[i] = std::thread(producer);

  // Launch consumers.
  constexpr size_t ncthread = 50;
  std::thread ct[ncthread];
  for (size_t i = 0; i != ncthread; ++i)
    ct[i] = std::thread(consumer);

  // Join threads.
  for (size_t i = 0; i != ncthread; ++i)
    ct[i].join();
  for (size_t i = 0; i != npthread; ++i)
    pt[i].join();
}
