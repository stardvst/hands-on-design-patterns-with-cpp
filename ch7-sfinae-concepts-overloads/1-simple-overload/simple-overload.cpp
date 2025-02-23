#include <iostream>

void f(int i) { std::cout << "f(int)\n";}
void f(long i) { std::cout << "f(long)\n";}
void f(double d) { std::cout << "f(double)\n"; }

int main()
{
  f(5);
  f(5L);
  f(5.0);

  return 0;
}
