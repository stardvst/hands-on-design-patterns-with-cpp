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

class StorageFinalizer
{
public:
  StorageFinalizer(Storage &storage) : m_storage(storage) {}

  ~StorageFinalizer()
  {
    m_storage.finalize();
  }

private:
  Storage &m_storage;
};

class Database
{
public:
  void
  add(Record const &r)
  {
    m_storage.add(r);
    StorageFinalizer sf(m_storage);
    try
    {
      m_index.add(r);
    }
    catch (...)
    {
      m_storage.undo();
      throw;
    }
  }

private:
  Storage m_storage;
  Index m_index;
};

int main()
{
  Database db;
  Record r;
  try
  {
    db.add(r);
  }
  catch (std::runtime_error &e)
  {
    std::cout << "Caught exception: " << e.what() << std::endl;
  }
  return 0;
}
