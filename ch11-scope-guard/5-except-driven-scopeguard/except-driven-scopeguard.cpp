#include <cstdlib> // For rand
#include <iostream>
#include <stdexcept>
#include <functional>

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

class ScopeGuardBase
{
public:
  ScopeGuardBase() = default;
  ScopeGuardBase &operator=(ScopeGuardBase const &other) = delete;

  void commit() noexcept
  {
    m_commit = true;
  }

protected:
  ScopeGuardBase(ScopeGuardBase &&other)
      : m_commit(other.m_commit)
  {
    other.commit();
  }

  bool m_commit = false;
};

template <typename Func, bool OnSuccess, bool OnFailure>
class ScopeGuard : public ScopeGuardBase
{
public:
  ScopeGuard(Func &&func)
      : m_func(std::move(func))
  {
  }

  ScopeGuard(const Func &func)
      : m_func(std::move(func))
  {
  }

  ScopeGuard(ScopeGuard &&) = default;

  ~ScopeGuard()
  {
    if ((OnSuccess && !m_detector) || (OnFailure && m_detector))
      m_func();
  }

private:
  Func m_func;
  UncaughtExceptionDetector m_detector;
};

#define CONCAT2(x, y) x##y
#define CONCAT(x, y) CONCAT2(x, y)
#ifdef __COUNTER__
#define ANON_VAR(x) CONCAT(x, __COUNTER__)
#else
#define ANON_VAR(x) CONCAT(x, __LINE__)
#endif

struct ScopeGuardOnExit
{
};
template <typename Func>
auto operator+(ScopeGuardOnExit, Func &&func)
{
  return ScopeGuard<Func, true, true>(std::forward<Func>(func));
}
#define ON_SCOPE_EXIT \
  auto ANON_VAR(SCOPE_EXIT_STATE) = ScopeGuardOnExit() + [&]()

struct ScopeGuardOnSuccess
{
};
template <typename Func>
auto operator+(ScopeGuardOnSuccess, Func &&func)
{
  return ScopeGuard<Func, true, false>(std::forward<Func>(func));
}
#define ON_SCOPE_SUCCESS \
  auto ANON_VAR(SCOPE_EXIT_STATE) = ScopeGuardOnSuccess() + [&]()

struct ScopeGuardOnFailure
{
};
template <typename Func>
auto operator+(ScopeGuardOnFailure, Func &&func)
{
  return ScopeGuard<Func, false, true>(std::forward<Func>(func));
}
#define ON_SCOPE_FAILURE \
  auto ANON_VAR(SCOPE_EXIT_STATE) = ScopeGuardOnFailure() + [&]()

class Database
{
public:
  void
  add(Record const &r)
  {
    m_storage.add(r);

    ON_SCOPE_EXIT { m_storage.finalize(); };
    ON_SCOPE_FAILURE { m_storage.undo(); };

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
