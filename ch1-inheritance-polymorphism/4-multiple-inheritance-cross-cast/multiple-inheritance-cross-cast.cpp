#include <iostream>

struct Base1
{
  virtual ~Base1() = default;
};

struct Base2
{
  virtual ~Base2() = default;
};

struct Derived : Base1, Base2
{
};

int main()
{
  Base1 *b = new Derived;
  Base2 *d = dynamic_cast<Base2 *>(b);

  std::cout << "b: " << b << '\n';
  std::cout << "d: " << d << '\n';

  return 0;
}
