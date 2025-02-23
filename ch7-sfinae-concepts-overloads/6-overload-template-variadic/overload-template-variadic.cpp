#include <iostream>

void f(int i) { std::cout << "f(int)\n"; }

void f(...) { std::cout << "f(...)\n"; }

int main()
{
  f(5);
  f(5L);
  f(5.0);

  struct A{};
  A a;
  f(a);

  struct B {
    operator int() const { return 0; }
  };
  B b;
  f(b);

  return 0;
}
