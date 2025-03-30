#include <iostream>

template <typename D>
class B
{
public:
  void f(int i) { static_cast<D *>(this)->f_impl(i); }

  void f_impl(int i) { std::cout << "B::f_impl(int)\n"; }
};

class D : public B<D>
{
public:
  //void f(int i) { std::cout << "D::f(int)\n"; }

  void f_impl(int i) { std::cout << "D::f_impl(int)\n"; }
};

class E : public B<E>
{
  // no f() here
};

int main()
{
  D d;
  B<D> *b = &d;
  b->f(5);

  E e;
  B<E> *b2 = &e;
  b2->f(5);

  return 0;
}
