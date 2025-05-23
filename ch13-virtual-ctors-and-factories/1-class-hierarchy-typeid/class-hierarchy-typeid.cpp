#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

template <typename T>
auto type(T &&t)
{
  using RefRemoved = std::remove_reference_t<T>;

  std::string name;
  if (std::is_const_v<RefRemoved>) name += "const ";
  if (std::is_volatile_v<RefRemoved>) name += "volatile ";

  int r;
  char *mangled_name = abi::__cxa_demangle(typeid(t).name(), nullptr, nullptr, &r);
  name += mangled_name;
  ::free(mangled_name);
  return name;
}

class A
{
public:
  A() { std::cout << "A::A(): " << type(*this) << std::endl; }
  virtual ~A() { std::cout << "A::~A(): " << type(*this) << std::endl; }
};

class B : public A
{
public:
  B() { std::cout << "B::B(): " << type(*this) << std::endl; }
  ~B() { std::cout << "B::~B(): " << type(*this) << std::endl; }
};

class C : public B
{
public:
  C() { std::cout << "C::C(): " << type(*this) << std::endl; }
  ~C() { std::cout << "C::~C(): " << type(*this) << std::endl; }

  void f() const { std::cout << "C::f(): " << type(*this) << std::endl; }
};

int main()
{
  const C c;
  c.f();
}
