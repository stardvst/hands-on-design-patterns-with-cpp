#include <iostream>
#include <string>
#include <string_view>
#include <cassert>

template <typename ...Types>
class Visitor;

template <typename T>
class Visitor<T>
{
public:
  virtual void visit(T *t) = 0;
};

template <typename T, typename ...Types>
class Visitor<T, Types...> : public Visitor<Types...>
{
public:
  using Visitor<Types...>::visit;
  virtual void visit(T *t) = 0;
};

using PetVisitor = Visitor<class Cat, class Dog>;

class Pet
{
public:
  explicit Pet(std::string_view name) : m_name(name) {}
  const std::string &getName() const { return m_name; }

  virtual ~Pet() = default;
  virtual void accept(PetVisitor &visitor) = 0;

private:
  std::string m_name;
};

template <typename PetVisitorType>
class VisitablePet : public Pet
{
public:
  using Pet::Pet;

  void accept(PetVisitor &visitor) override
  {
    visitor.visit(static_cast<PetVisitorType *>(this));
  }
};

class Cat : public VisitablePet<Cat>
{
public:
  using VisitablePet<Cat>::VisitablePet;
};

class Dog : public VisitablePet<Dog>
{
public:
  using VisitablePet<Dog>::VisitablePet;
};

class FeedingVisitor : public PetVisitor
{
public:
  void visit(Cat *cat) override { std::cout << "Feeding cat: " << cat->getName() << std::endl; }
  void visit(Dog *dog) override { std::cout << "Feeding dog: " << dog->getName() << std::endl; }
};

class BathingVisitor : public PetVisitor
{
public:
  void visit(Cat *cat) override { std::cout << "Bathing cat: " << cat->getName() << std::endl; }

  // has to be implemented even though we don't bathe dogs
  void visit(Dog *dog) override { std::cout << "Doing nothng for bathing dog: " << dog->getName() << std::endl; }
};

int main()
{
  std::unique_ptr<Pet> pCat = std::make_unique<Cat>("Mittens");
  std::unique_ptr<Pet> pDog = std::make_unique<Dog>("Rex");

  FeedingVisitor feedingVisitor;
  BathingVisitor bathingVisitor;

  pCat->accept(feedingVisitor); // Feeding cat: Mittens
  pDog->accept(feedingVisitor); // Feeding dog: Rex

  pCat->accept(bathingVisitor); // Bathing cat: Mittens
  pDog->accept(bathingVisitor); // Bathing dog: Rex
}
