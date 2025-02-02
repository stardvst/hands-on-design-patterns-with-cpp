#include <iostream>

struct Base1
{
  int a;
};

struct Base2
{
  int b;
  int c;
};

struct Derived : Base1, Base2
{
};

int main()
{
  Derived d;
  Derived *p = &d;
  std::cout << "Derived: " << (void *)p <<
    ", Base1: " << (void *)static_cast<Base1 *>(p) <<
    ", Base2: " << (void *)static_cast<Base2 *>(p) << '\n';

  p = nullptr;
  std::cout << "Derived: " << (void *)p <<
    ", Base1: " << (void *)static_cast<Base1 *>(p) << // includes a hidden check for p != nullptr
    ", Base2: " << (void *)static_cast<Base2 *>(p) << '\n'; // includes a hidden check for p != nullptr

  return 0;
}
