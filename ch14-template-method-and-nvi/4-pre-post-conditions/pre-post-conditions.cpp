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

  bool StateIsValid() const
  {
    return m_actionStarted == m_actionFinished;
  }
};

class Derived : public Base
{
public:
  void ActionImpl() override
  {
    ++m_actionStarted;

    // Simulate some action
    std::cout << "Action in Derived class executed." << std::endl;

    ++m_actionFinished;
  }
};

int main()
{
  Derived d;
  d.VerifiedAction();

  std::cout << "Action completed successfully." << std::endl;
}
