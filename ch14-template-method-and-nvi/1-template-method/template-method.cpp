#include <iostream>

class Base
{
public:
  bool TheAlgorithm()
  {
    if (!Step1())
    {
      std::cout << "Step1 failed." << std::endl;
      return false;
    }
    Step2();
    return true;
  }

  virtual bool Step1()
  {
    std::cout << "Base Step1 executed." << std::endl;
    return true;
  }

  virtual void Step2() = 0;

  virtual ~Base() = default;
};

class Derived1 : public Base
{
public:
  void Step2() override
  {
    std::cout << "Derived1 Step2 executed." << std::endl;
  }
};

class Derived2 : public Base
{
public:
  bool Step1() override
  {
    std::cout << "Derived2 Step1 executed." << std::endl;
    return false;
  }

  void Step2() override
  {
    std::cout << "Derived2 Step2 executed." << std::endl;
  }
};

class Derived3 : public Base
{
public:
  bool Step1() override
  {
    std::cout << "Derived3 Step1 executed." << std::endl;
    return true;
  }

  void Step2() override
  {
    std::cout << "Derived3 Step2 executed." << std::endl;
  }
};

int main()
{
  auto b1 = std::make_unique<Derived1>();
  b1->TheAlgorithm();
  std::cout << "------------------------" << std::endl;

  auto b2 = std::make_unique<Derived2>();
  b2->TheAlgorithm();
  std::cout << "------------------------" << std::endl;

  auto b3 = std::make_unique<Derived3>();
  b3->TheAlgorithm();

  return 0;
}
