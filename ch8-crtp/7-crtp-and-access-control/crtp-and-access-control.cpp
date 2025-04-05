#include <iostream>

template <typename D>
class B
{
public:
  void f(int i) { static_cast<D *>(this)->f_impl(i); }

private:
  void f_impl(int i) { std::cout << "B::f_impl(int)\n"; }
};

class D : public B<D>
{
private:
  void f_impl(int i) { std::cout << "D::f_impl(int)\n"; }
  friend class B<D>;
};

class D1 : public B<D1> // with B<D>: error: 'B<D>' is not a base of 'D1'
{
private:
  void f_impl(int i) { std::cout << "D1::f_impl(int)\n"; }
  friend class B<D1>;
};

template <typename D>
class AbstractB
{
  friend D; // declare D as a friend

public:
  void f(int i) { static_cast<D *>(this)->f_impl(i); }

private:
  void f_impl(int i) { std::cout << "AbstractB::f_impl(int)\n"; }
};

class CorrectD : public AbstractB<CorrectD>
{
};

// wrong base class
// class WrongD : public AbstractB<CorrectD>
// {
// };

int main()
{
  D d;
  B<D> *b = &d;
  b->f(5);

  // error: cannot initialize a variable of type 'B<D1> *' with an rvalue of type 'D1 *'
  D1 d1;
  B<D1> *b1 = &d1;
  b1->f(5);

  CorrectD cd;
  AbstractB<CorrectD> *ab = &cd;
  ab->f(5);

  // error: static_cast from 'AbstractB<WrongD> *' to 'WrongD *', which are not related by inheritance, is not allowed
  // WrongD wd;
  // AbstractB<WrongD> *ab2 = &wd;
  // ab2->f(5);

  return 0;
}
