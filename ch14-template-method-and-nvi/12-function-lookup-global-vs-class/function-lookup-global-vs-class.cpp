#include <iostream>

void f()
{
  std::cout << "global f() called" << std::endl;
}

template <typename T>
struct A
{
  void f() const
  {
    std::cout << "A<T>::f() called" << std::endl;
  }

  void k() const
  {
    std::cout << "A<T>::k() called" << std::endl;
  }
};

template <typename T>
struct B : A<T>
{
  void h() const
  {
    f();
  }

  void l() const
  {
    // doesn't compile!
    // error: use of undeclared identifier 'k'
    // k();

    this->k();
  }
};

int main()
{
  B<int> b;
  b.h();
  b.l();
}
