#include <iostream>
#include <string>
#include <string_view>
#include <cassert>

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

class PetVisitor
{
public:
  virtual ~PetVisitor() = default;
};

class Cat;
class CatVisitor
{
public:
  virtual void visit(Cat *cat) = 0;
};

class Cat : public Pet
{
public:
  explicit Cat(std::string_view name) : Pet(name) {}

  void accept(class PetVisitor &visitor) override
  {
    if (auto catVisitor = dynamic_cast<CatVisitor *>(&visitor))
      catVisitor->visit(this);
    else
      assert(false);
  }
};

class Dog;
class DogVisitor
{
public:
  virtual void visit(Dog *dog) = 0;
};

class Dog : public Pet
{
public:
  explicit Dog(std::string_view name) : Pet(name) {}

  void accept(class PetVisitor &visitor) override
  {
    if (auto dogVisitor = dynamic_cast<DogVisitor *>(&visitor))
      dogVisitor->visit(this);
    else
      assert(false);
  }
};

class FeedingCatVisitor : public PetVisitor, public CatVisitor, public DogVisitor
{
public:
  void visit(Cat *cat) override { std::cout << "Feeding cat: " << cat->getName() << std::endl; }
  void visit(Dog *dog) override { std::cout << "Feeding dog: " << dog->getName() << std::endl; }
};

class BathingDogVisitor : public PetVisitor, public CatVisitor
{
public:
  void visit(Cat *cat) override { std::cout << "Bathing cat: " << cat->getName() << std::endl; }

  // This will not compile, as dog doesn't support bathing
  // void visit(Dog *dog) override { std::cout << "Bathing dog: " << dog->getName() << std::endl; }
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

  // Error: BathingDogVisitor doesn't support bathing Dog
  // pDog->accept(bathingDogVisitor);
}
