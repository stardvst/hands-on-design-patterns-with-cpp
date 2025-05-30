#include <iostream>

class Base
{
public:
  // call to pure virtual member function 'clear' has undefined behavior;
  // overrides of 'clear' in subclasses are not available in the destructor of 'Base'
  virtual ~Base()
  {
    std::cout << "Deleting now" << std::endl;
    clear();
    std::cout << "Deleted" << std::endl;
  }

protected:
  virtual void clear() = 0;
};

class Derived : public Base
{
protected:
  void clear() override
  {
    std::cout << "Derived::clear()" << std::endl;
    Base::clear();
  }
};

int main()
{
  Base *b = new Derived();
  delete b;
  return 0;
}
