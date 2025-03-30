#include <iostream>

template <typename D>
class B
{
public:
  void f(int i) { static_cast<D *>(this)->f(i); }
};

class D : public B<D>
{
public:
  void f(int i) { std::cout << "D::f(int)\n"; }
};

template <typename D>
void apply(B<D> *b, int i)
{
  b->f(i);
}

int main()
{
  D d;
  B<D> *b = &d;
  b->f(5);

  apply(b, 5);

  return 0;
}
