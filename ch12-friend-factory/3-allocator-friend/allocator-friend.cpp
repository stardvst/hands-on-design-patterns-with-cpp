#include <cstdlib>
#include <iostream>

class Allocator
{
public:
  void *alloc(std::size_t size);
  void dealloc(void *p);

  friend void *operator new(std::size_t size, Allocator *allocator);
  friend void operator delete(void *p, Allocator *allocator);

  // The functions cannot be defined in the class body because then they
  // won't be able to be found by the compiler when they are called!

  // friend void *operator new(std::size_t size, Allocator *allocator)
  // {
  //   return allocator->alloc(size);
  // }

  // friend void operator delete(void *p, Allocator *allocator)
  // {
  //   allocator->dealloc(p);
  // }
};

void *Allocator::alloc(std::size_t size)
{
  return std::malloc(size);
}

void Allocator::dealloc(void *p)
{
  std::free(p);
}

void *operator new(std::size_t size, Allocator *allocator)
{
  return allocator->alloc(size);
}

void operator delete(void *p, Allocator *allocator)
{
  allocator->dealloc(p);
}

int main()
{
  Allocator allocator;
  int *p = new (&allocator) int(42);
  std::cout << *p << std::endl;
  delete p; // Calls allocator.dealloc(p)
}
