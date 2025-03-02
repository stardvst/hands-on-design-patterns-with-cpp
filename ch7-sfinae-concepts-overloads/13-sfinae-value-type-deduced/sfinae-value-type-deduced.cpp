#include <iostream>

template <typename T, std::size_t N = T::N>
void f(T t, char (*)[N % 2] = nullptr)
{
  std::cout << "N = " << N << " is odd\n";
}

template <typename T, std::size_t N = T::N>
void f(T t, char (*)[1 - N % 2] = nullptr)
{
  std::cout << "N = " << N << " is even\n";
}

struct A {
  enum
  {
    N = 5
  };
};

struct B {
  enum
  {
    N = 6
  };
};

int
main()
{
  A a;
  f(a);

  B b;
  f(b);

  return 0;
}
