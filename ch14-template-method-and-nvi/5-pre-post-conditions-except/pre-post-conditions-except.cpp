#include <cassert>
#include <cstdlib>
#include <iostream>

class Base
{
public:
  virtual ~Base() = default;

  void VerifiedAction()
  {
    assert(StateIsValid());
    ActionImpl();
    assert(StateIsValid());
  }

  virtual void ActionImpl() = 0;

protected:
  std::size_t m_actionStarted = 0;
  std::size_t m_actionFinished = 0;
  std::size_t m_actionFailed = 0;

  bool StateIsValid() const
  {
    return m_actionStarted == m_actionFinished + m_actionFailed;
  }
};

class Derived : public Base
{
public:
  void ActionImpl() override
  {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator

    ++m_actionStarted;

    try
    {
      // Simulate some action that might throw an exception
      if (rand() % 2 == 0) // Randomly simulate failure
        throw std::runtime_error("Simulated action failure");

      ++m_actionFinished;
      std::cout << "Action completed successfully." << std::endl;
    }
    catch (const std::exception &e)
    {
      std::cerr << "Exception caught: " << e.what() << std::endl;
      ++m_actionFailed;
    }
  }
};

int main()
{
  Derived d;
  d.VerifiedAction();
}
