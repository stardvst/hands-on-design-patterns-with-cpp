#include <cstdlib> // For rand
#include <iostream>
#include <stdexcept>

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

template <typename Func>
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
    if (!m_commit)
      m_func();
  }

private:
  Func m_func;
};

class Database
{
public:
  void
  add(Record const &r)
  {
    m_storage.add(r);

    ScopeGuard sf = [&]
    { m_storage.finalize(); };

    ScopeGuard sg = [&]
    { m_storage.undo(); };

    m_index.add(r);

    // Reaching here means both the storage and index operations were successful, so commit the storage changes
    sg.commit();
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
