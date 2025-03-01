#include <iostream>
#include <typeinfo>

template <typename T>
void f(T i, T *p)
{
  std::cout << "f(" << typeid(i).name() << ", " << typeid(p).name() << ")\n";
}

int main()
{
  int i{};
  f(5, &i);

  // f(5L, &i); // no matching function for call to 'f'
  f<int>(5L, &i);

  return 0;
}
