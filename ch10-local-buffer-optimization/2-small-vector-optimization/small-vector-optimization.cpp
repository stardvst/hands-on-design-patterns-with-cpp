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

class simple_vector
{
public:
  simple_vector() = default;

  simple_vector(std::initializer_list<int> elements)
      : m_size(elements.size()),
        m_data(static_cast<int *>(malloc(sizeof(int) * m_size)))
  {
    int *data = m_data;
    for (auto x : elements)
      *data++ = x;
  }

  ~simple_vector() { free(m_data); }

  std::size_t size() const { return m_size; }

private:
  std::size_t m_size = 0;
  int *m_data = nullptr;
};

class small_vector
{
public:
  small_vector() = default;

  small_vector(std::initializer_list<int> elements)
      : m_size(elements.size()),
        m_data(m_size < sizeof(m_buffer) / sizeof(m_buffer[0]) ? m_buffer : static_cast<int *>(malloc(sizeof(int) * m_size)))
  {
    int *data = m_data;
    for (auto x : elements)
      *data++ = x;
  }

  ~small_vector()
  {
    if (m_data != m_buffer)
      free(m_data);
  }

  std::size_t size() const { return m_size; }

private:
  std::size_t m_size = 0;
  int *m_data = nullptr;
  int m_buffer[16]; // small buffer for short vectors
};

class smaller_vector
{
public:
  smaller_vector() = default;

  smaller_vector(std::initializer_list<int> elements)
  {
    int *data{nullptr};
    if (elements.size() < sizeof(m_short.m_buffer) / sizeof(m_short.m_buffer[0]))
    {
      m_short.m_size = static_cast<unsigned char>(elements.size());
      data = m_short.m_buffer;
    }
    else
    {
      m_short.m_size = UCHAR_MAX;
      m_long.m_size = elements.size();
      data = m_long.m_data = static_cast<int *>(malloc(sizeof(int) * m_long.m_size));
    }

    for (auto x : elements)
      *data++ = x;
  }

  ~smaller_vector()
  {
    if (m_short.m_size == UCHAR_MAX)
      free(m_long.m_data);
  }

  std::size_t size() const
  {
    if (m_short.m_size == UCHAR_MAX)
      return m_long.m_size;
    else
      return m_short.m_size;
  }

private:
  union
  {
    struct
    {
      unsigned char m_size;
      int m_buffer[15];
    } m_short = {'\0', {0}};

    struct
    {
      std::size_t m_size;
      int *m_data;
    } m_long;
  };
};

int main()
{
  constexpr std::size_t N = 100000;
  double simpleVectorTime = 0, smallVectorTime = 0, smallerVectorTime = 0;

  {
    Timer timer("simple_vector", []()
                {
      simple_vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      (void)v; }, N);
    simpleVectorTime = timer.elapsed();
  }

  {
    Timer timer("small_vector", []()
                {
      small_vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      (void)v; }, N);
    smallVectorTime = timer.elapsed();
  }

  {
    Timer timer("smaller_vector", []()
                {
      smaller_vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      (void)v; }, N);
    smallerVectorTime = timer.elapsed();
  }

  std::cout << "small_vector is " << (smallVectorTime / simpleVectorTime) << " times faster than simple_vector" << std::endl;
  std::cout << "smaller_vector is " << (smallerVectorTime / smallVectorTime) << " times faster than small_vector" << std::endl;
  std::cout << "smaller_vector is " << (smallerVectorTime / simpleVectorTime) << " times faster than simple_vector" << std::endl;

  return 0;
}
