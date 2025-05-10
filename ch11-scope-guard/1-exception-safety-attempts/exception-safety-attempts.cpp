#include <cstdlib> // For rand
#include <iostream>
#include <stdexcept>

struct Record
{
};

class Database
{
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
      // if (rand() % 2 == 0)
      throw std::runtime_error("Index error");
    }
  };

public:
  void
  add(Record const &r)
  {
    m_storage.add(r);
    m_index.add(r);
  }

  void add_fix1(Record const &r)
  {
    m_storage.add(r);
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

  void add_fix2(Record const &r)
  {
    m_storage.add(r);
    try
    {
      m_index.add(r);
    }
    catch (...)
    {
      m_storage.undo();
      m_storage.finalize();
      throw;
    }
    m_storage.finalize();
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
    std::cerr << "Error: " << e.what() << std::endl;
  }

  std::cout << "------------------------" << std::endl;
  try
  {
    db.add_fix1(r);
  }
  catch (std::runtime_error &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  std::cout << "------------------------" << std::endl;
  try
  {
    db.add_fix2(r);
  }
  catch (std::runtime_error &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
