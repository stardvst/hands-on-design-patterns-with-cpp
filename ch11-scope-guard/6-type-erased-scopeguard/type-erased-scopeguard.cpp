#include <cstdlib> // For rand
#include <iostream>
#include <stdexcept>
#include <functional>

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

struct Record
{
};

struct Storage
{
  Storage()
  {
    std::srand(std::time(nullptr)); // Seed the random number generator
  }

  void add(Record const &r)
  {
    if (rand() % 2 == 0)
      throw std::runtime_error("Storage error");
  }

  void undo()
  {
    std::cout << "Undoing last operation in Storage" << std::endl;
  }

  void finalize()
  {
    std::cout << "Finalizing last operation in Storage" << std::endl;
  }
};
struct Index
{
  void add(Record const &r)
  {
    if (rand() % 2 == 0)
      throw std::runtime_error("Index error");
  }
};

class UncaughtExceptionDetector
{
public:
  operator bool() const noexcept
  {
    return std::uncaught_exceptions() > m_currentCount;
  }

private:
  const int m_currentCount = std::uncaught_exceptions();
};

class ScopeGuard
{
public:
  template <typename Func>
  ScopeGuard(Func &&func)
      : m_func(std::move(func))
  {
  }

  ScopeGuard &operator=(ScopeGuard const &) = delete;

  ScopeGuard(ScopeGuard &&other)
      : m_commit(other.m_commit), m_func(std::move(other.m_func))
  {
    other.commit();
  }

  ~ScopeGuard()
  {
    if (!m_commit)
      m_func();
  }

  void commit() const noexcept
  {
    m_commit = true;
  }

private:
  std::function<void()> m_func;
  mutable bool m_commit = false;
};

class Database
{
public:
  void
  add(Record const &r)
  {
    m_storage.add(r);

    ScopeGuard SF{[&]
                  { m_storage.finalize(); }};
    ScopeGuard SG{[&]
                  { m_storage.undo(); }};

    m_index.add(r);
  }

private:
  Storage m_storage;
  Index m_index;
};

int main()
{
  try
  {
    Database db;
    Record r;
    db.add(r);
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}
