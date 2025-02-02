#include <iostream>

struct Base
{
  virtual ~Base() = default;
};

struct Derived : Base
{
};

int main()
{
  std::unique_ptr<Base> b(new Base);
  std::unique_ptr<Base> d(new Derived);

  Derived *d1 = dynamic_cast<Derived *>(b.get());
  Derived *d2 = dynamic_cast<Derived *>(d.get());

  std::cout << "d1: " << d1 << '\n';
  std::cout << "d2: " << d2 << '\n';

  return 0;
}
