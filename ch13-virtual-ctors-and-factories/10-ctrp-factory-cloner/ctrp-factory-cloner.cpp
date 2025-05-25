#include <memory>

class Base
{
public:
  virtual ~Base() = default;
  virtual std::unique_ptr<Base> clone() const = 0;
};

template <typename Derived>
class Cloner : public Base
{
public:
  std::unique_ptr<Base> clone() const override
  {
    return std::make_unique<Derived>(static_cast<const Derived &>(*this));
  }
};

class Derived : public Cloner<Derived>
{
};

int main()
{
  std::unique_ptr<Derived> d = std::make_unique<Derived>();
  std::unique_ptr<Base> dClone = d->clone();
  return 0;
}
