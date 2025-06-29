#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <memory>

template <typename T, std::size_t N>
class lock_free_queue
{
public:
  lock_free_queue() = default;
  lock_free_queue(const lock_free_queue &) = delete;
  lock_free_queue &operator=(const lock_free_queue &) = delete;

  template <typename U>
  bool push(U &&value)
  {
    const std::size_t front = m_front.load(std::memory_order_acquire);
    std::size_t back = m_back.load(std::memory_order_relaxed);
    if (back == front) // Queue is full
      return false;

    m_data[back] = std::forward<U>(value);
    m_back.store((back + 1) % N, std::memory_order_release);
    return true;
  }

  std::optional<T> pop()
  {
    std::size_t back = m_back.load(std::memory_order_acquire);
    std::size_t front = (m_front.load(std::memory_order_relaxed) + 1) % N;
    if (front == back) // Queue is empty
      return std::nullopt;

    std::optional<T> value = std::move(m_data[front]);
    m_front.store(front, std::memory_order_release);
    return value;
  }

  void dump() const
  {
    const std::size_t front = m_front.load(std::memory_order_relaxed);
    const std::size_t back = m_back.load(std::memory_order_relaxed);
    std::cout << "back: " << back << ", front: " << front << std::endl;
    for (size_t i = (front + 1) % N; i != back; i = (i + 1) % N)
    {
      std::cout << " " << m_data[i] << "[" << i << "]";
    }
    std::cout << std::endl;
  }

private:
  T m_data[N];
  std::atomic<std::size_t> m_back{0};
  std::atomic<std::size_t> m_front{N - 1};
};

lock_free_queue<int, 10> queue;

int main()
{
  std::atomic<bool> done{false};
  size_t wasFullTimes = 0;
  std::thread producer([&]()
                       {
        for (size_t i = 0; i != 150; ++i)
        {
          while (!queue.push(2 * i + 1))
            ++wasFullTimes;

          if (i && (i % 10) == 0)
            std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(1)); // Let the consumer catch up
        }
        done.store(true, std::memory_order_release); });

  std::thread consumer([&]()
                       {
        size_t wasEmptyTimes = 0;
        while (1)
        {
          std::optional<int> res = queue.pop();
          if (res)
          {
            std::cout << *res << " " << std::flush;
          }
          else
          {
            if (done.load(std::memory_order_acquire))
              break;

            ++wasEmptyTimes;
          }
        }
        std::cout << "\nQueue was empty " << wasEmptyTimes << " times" << std::endl; });

  producer.join();
  consumer.join();

  std::cout << "Queue was full " << wasFullTimes << " times" << std::endl;
}
