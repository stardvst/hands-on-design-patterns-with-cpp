#include <cstring>
#include <cstdlib>
#include <string>
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

class string
{
public:
  string() = default;

  explicit string(const char *str) : m_str(strdup(str)) {}

  string(const string &other) : m_str(strdup(other.m_str)) {}
  string &operator=(const string &other)
  {
    if (this != &other)
    {
      free(m_str);
      m_str = strdup(other.m_str);
    }
    return *this;
  }

  string &operator=(const char *str)
  {
    free(m_str);
    m_str = strdup(str);
    return *this;
  }

  bool operator==(const string &other) const
  {
    return strcmp(m_str, other.m_str) == 0;
  }

  ~string()
  {
    free(m_str);
  }

private:
  char *m_str = nullptr;
};

class string_sso
{
public:
  string_sso() = default;

  explicit string_sso(const char *str)
      : m_str(strlen(str) + 1 < sizeof(m_buffer) ? strcpy(m_buffer, str) : strdup(str))
  {
  }

  string_sso(const string_sso &other)
      : m_str(other.m_str == other.m_buffer ? strcpy(m_buffer, other.m_buffer) : strdup(other.m_str))
  {
  }

  string_sso &operator=(const string_sso &other)
  {
    if (this != &other)
    {
      if (m_str != m_buffer)
        free(m_str);
      m_str = other.m_str == other.m_buffer ? strcpy(m_buffer, other.m_buffer) : strdup(other.m_str);
    }
    return *this;
  }

  string_sso &operator=(const char *str)
  {
    if (m_str != m_buffer)
      free(m_str);
    m_str = strlen(str) + 1 < sizeof(m_buffer) ? strcpy(m_buffer, str) : strdup(str);
    return *this;
  }

  bool operator==(const string_sso &other) const
  {
    return strcmp(m_str, other.m_str) == 0;
  }

  ~string_sso()
  {
    if (m_str != m_buffer)
      free(m_str);
  }

private:
  char *m_str = nullptr;
  char m_buffer[16]; // small buffer for short strings
};

int main()
{
  constexpr std::size_t N = 100000;

  double smallStringTime = 0, betterStringTime = 0;

  {
    Timer timer("string copy", []()
                {
      string s1("Hello");
      string s2(s1); }, N);
    smallStringTime += timer.elapsed();
  }

  {
    Timer timer("string_sso copy", []()
                {
      string_sso s1("Hello");
      string_sso s2(s1); }, N);
    betterStringTime += timer.elapsed();
  }
  std::cout << "better string is better than simple string by: " << (betterStringTime / smallStringTime) << "\n----------" << std::endl;

  {
    Timer timer("string assign", []()
                {
      string s1;
      s1 = string("Hello"); }, N);
    smallStringTime += timer.elapsed();
  }

  {
    Timer timer("string_sso assign", []()
                {
      string_sso s1;
      s1 = string_sso("Hello"); }, N);
    betterStringTime += timer.elapsed();
  }
  std::cout << "better string is better than simple string by: " << (betterStringTime / smallStringTime) << "\n----------" << std::endl;

  {
    Timer timer("string compare", []()
                {
      string s1("Hello");
      string s2("World");
      auto b = s1 == s2 ? s1 : s2; }, N);
    smallStringTime += timer.elapsed();
  }

  {
    Timer timer("string_sso compare", []()
                {
      string_sso s1("Hello");
      string_sso s2("World");
      auto b = s1 == s2 ? s1 : s2; }, N);
    betterStringTime += timer.elapsed();
  }
  std::cout << "better string is better than simple string by: " << (betterStringTime / smallStringTime) << std::endl;

  return 0;
}
