#include <iostream>

void f(int i) { std::cout << "f(int)\n"; }

template <typename T>
void f(T i) { std::cout << "f(T)\n"; }

template <typename T>
void f(T *i) { std::cout << "f(T*)\n"; }

int main()
{
  f(5);
  f(5L);

  int i = 0;
  f(&i);

  f(NULL);
  f(nullptr);

  return 0;
}
