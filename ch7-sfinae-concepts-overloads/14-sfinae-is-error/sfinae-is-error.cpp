#include <iostream>

template <typename T>
void f(T)
{
  std::cout << sizeof(T::i) << '\n';
}

struct A {
  int i;
};

int main()
{
  A a;
  f(a);

  // type 'int' cannot be used prior to '::' because it has no members
  // int i;
  // f(i);

  return 0;
}
