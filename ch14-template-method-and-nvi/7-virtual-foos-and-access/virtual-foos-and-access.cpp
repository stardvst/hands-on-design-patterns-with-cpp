#include <iostream>

class Base
{
public:
  void method1()
  {
    std::cout << "Base method1 executed." << std::endl;
    method2();
  }

  virtual void method2()
  {
    std::cout << "Base method2 executed." << std::endl;
  }

private:
  virtual void method3()
  {
    std::cout << "Base method3 executed." << std::endl;
  }
};

class Derived : public Base
{
private:
  void method2() override
  {
    std::cout << "Derived method2 executed." << std::endl;
    method3();
  }

  void method3() override
  {
    std::cout << "Derived method3 executed." << std::endl;
  }
};

int main()
{
  auto d = new Derived();
  Base *b = d;
  b->method2();
  // d->method2(); // doesn't compile, method2 is private in Derived
}
