#include <initializer_list>
#include <iostream>

class Timer
{
public:
  template <typename F>
  explicit Timer(const std::string &label, F &&f, std::size_t N) : m_startTime(std::chrono::high_resolution_clock::now()), m_label(label)
  {
    for (std::size_t i = 0; i < N; ++i)
    {
      f();
    }
  }

  double elapsed() const
  {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsedTime = endTime - m_startTime;
    return elapsedTime.count();
  }

  ~Timer()
  {
    std::cout << m_label << " elapsed time: " << elapsed() << " ms" << std::endl;
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
  std::string m_label;
};

class small_queue
{
public:
  bool push(int i)
  {
    if (m_head - m_tail >= s_bufSize)
      return false; // queue is full

    m_buffer[(m_head++) & (s_bufSize - 1)] = i;
    return true;
  }

  int front() const
  {
    return m_buffer[m_tail & (s_bufSize - 1)];
  }

  void pop()
  {
    if (!empty())
      ++m_tail;
  }

  bool empty() const
  {
    return m_head == m_tail;
  }

  std::size_t size() const
  {
    return m_head - m_tail;
  }

  void print() const
  {
    std::cout << "Queue: ";
    for (std::size_t i = m_tail; i < m_head; ++i)
      std::cout << m_buffer[i & (s_bufSize - 1)] << " ";
    std::cout << std::endl;
  }

private:
  static constexpr std::size_t s_bufSize = 16;
  static_assert((s_bufSize & (s_bufSize - 1)) == 0, "Buffer size must be a power of 2");

  int m_buffer[s_bufSize];
  std::size_t m_head = 0;
  std::size_t m_tail = 0;
};

template <typename Queue>
void useQueue(Queue &q)
{
  for (int i = 0; i < 100; ++i)
  {
    q.push(i);
    if (i == 50)
    {
      for (int j = 0; j < 5; ++j)
        q.pop();
    }
  }
}

int main()
{
  constexpr std::size_t N = 100000;

  double stdQueueTime = 0.0, smallQueueTime = 0.0;
  {
    Timer timer("std::queue", [&]()
                {
      small_queue q;
      useQueue(q);
      (void)q; }, N);
    stdQueueTime = timer.elapsed();
  }

  {
    Timer timer("small_queue", [&]()
                {
        small_queue q;
        useQueue(q);
      (void)q; }, N);
    smallQueueTime = timer.elapsed();
  }

  std::cout << "Small queue is " << (stdQueueTime / smallQueueTime) << " times faster than std::queue" << std::endl;

  return 0;
}
