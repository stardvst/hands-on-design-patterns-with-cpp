#include <iostream>

template <typename D>
class B
{
public:
  // option 2
  /* virtual */ ~B()
  {
    std::cout << "B::~B()\n";

    // infinite loop; object type isn't D anymore
    // static_cast<D *>(this)->~D();
  }
};

class D : public B<D>
{
public:
  ~D() { std::cout << "D::~D()\n"; }
};

// option 1
template <typename D>
void destroy(B<D> *b)
{
  delete static_cast<D *>(b);
}

int main()
{
  B<D> *b1 = new D;
  delete b1; // ~D() isn't called

  std::cout << "\n";
  B<D> *b2 = new D;
  destroy(b2);

  return 0;
}
