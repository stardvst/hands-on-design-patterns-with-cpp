#include <iostream>

template <typename T>
void f(T *i)
{
  std::cout << "f(T*)\n";
}

template <typename T>
void f(int T::* i)
{
  std::cout << "f(T::*)\n";
}

struct A
{
  int i;
};

int main()
{
  A a{5};
  f(&a.i);
  f(&A::i);

  int i;
  f(&i);

  return 0;
}
