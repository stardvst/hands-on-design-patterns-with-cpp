#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class LockingQueue
{
  using value_type = typename std::queue<T>::value_type;

public:
  void push(const value_type &value)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(value);
  }

  void push(value_type &&value)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(std::move(value));
  }

  bool pop(value_type &value)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queue.empty())
      return false;

    value = std::move(m_queue.front());
    m_queue.pop();
    return true;
  }

  std::optional<value_type> try_pop()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queue.empty())
      return std::nullopt;

    value_type value = std::move(m_queue.front());
    m_queue.pop();
    return std::make_optional(value);
  }

private:
  std::queue<T> m_queue;
  std::mutex m_mutex;
};

int main()
{
  LockingQueue<int> queue;

  // Producer thread
  std::thread producer([&queue]()
                       {
    for (int i = 0; i < 10; ++i)
    {
      queue.push(i);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } });

  // Consumer thread
  std::thread consumer([&queue]()
                       {
    for (int i = 0; i < 11; ++i)
    {
      if (auto value = queue.try_pop())
      {
        std::cout << "Consumed: " << *value << std::endl;
      }
      else
      {
        std::cout << "Queue is empty." << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(150));
    } });

  producer.join();
  consumer.join();

  return 0;
}
