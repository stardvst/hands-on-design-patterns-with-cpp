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

class StorageGuard
{
public:
  StorageGuard(Storage &storage) : m_storage(storage) {}

  ~StorageGuard()
  {
    if (!m_commit)
      m_storage.undo();
  }

  void commit()
  {
    m_commit = true;
  }

  StorageGuard(StorageGuard const &) = delete;
  StorageGuard &operator=(StorageGuard const &) = delete;
  StorageGuard(StorageGuard &&) = delete;
  StorageGuard &operator=(StorageGuard &&) = delete;

private:
  Storage &m_storage;
  bool m_commit = false;
};

class Database
{
public:
  void
  add(Record const &r)
  {
    m_storage.add(r);

    StorageFinalizer sf(m_storage);
    StorageGuard sg(m_storage);

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
