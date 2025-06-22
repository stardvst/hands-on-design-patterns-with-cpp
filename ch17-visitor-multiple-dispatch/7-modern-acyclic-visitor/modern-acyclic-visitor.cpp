#include <iostream>
#include <string>
#include <string_view>
#include <cassert>

class Cat;
class Dog;

class PetVisitor
{
public:
  virtual void visit(Cat *c) = 0;
  virtual void visit(Dog *d) = 0;
};

class Pet
{
public:
  explicit Pet(std::string_view name) : m_name(name) {}
  const std::string &getName() const { return m_name; }

  virtual ~Pet() = default;
  virtual void accept(class PetVisitor &visitor) = 0;

private:
  std::string m_name;
};

template <typename PetVisitorType>
class VisitablePet : public Pet
{
public:
  using Pet::Pet;

  void accept(class PetVisitor &visitor) override
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

class FeedingCatVisitor : public PetVisitor
{
public:
  void visit(Cat *cat) override { std::cout << "Feeding cat: " << cat->getName() << std::endl; }
  void visit(Dog *dog) override { std::cout << "Feeding dog: " << dog->getName() << std::endl; }
};

class BathingDogVisitor : public PetVisitor
{
public:
  void visit(Cat *cat) override { std::cout << "Bathing cat: " << cat->getName() << std::endl; }

  // has to be implemented even though we don't bathe dogs
  void visit(Dog *dog) override {}
};

int main()
{
  std::unique_ptr<Pet> pCat = std::make_unique<Cat>("Mittens");
  std::unique_ptr<Pet> pDog = std::make_unique<Dog>("Rex");

  FeedingCatVisitor feedingCatVisitor;
  BathingDogVisitor bathingDogVisitor;

  pCat->accept(feedingCatVisitor); // Feeding cat: Mittens
  pDog->accept(feedingCatVisitor); // Feeding dog: Rex

  pCat->accept(bathingDogVisitor); // Bathing cat: Mittens
  pDog->accept(bathingDogVisitor); // Bathing dog: Rex
}
