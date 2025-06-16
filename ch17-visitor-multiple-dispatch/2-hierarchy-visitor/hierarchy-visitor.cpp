#include <string_view>
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

class Pet;
class Cat;
class Dog;

class PetVisitor
{
public:
  virtual void visit(Dog *dog, Pet *pet = nullptr) = 0;
  virtual void visit(Cat *cat, Pet *pet = nullptr) = 0;

  virtual ~PetVisitor() = default;
};

class Pet
{
public:
  Pet(std::string_view name) : m_name(name) {}
  const std::string &getName() const { return m_name; }

  void addChild(Pet *child) { m_children.push_back(child); }
  const std::vector<Pet *> &getChildren() const { return m_children; } // or make FamilyTreeVisitor a friend class

  virtual void accept(class PetVisitor &visitor, Pet *pet = nullptr) = 0;

  virtual ~Pet() = default;

private:
  std::string m_name;
  std::vector<Pet *> m_children;
};

class Dog : public Pet
{
public:
  using Pet::Pet;

  void accept(class PetVisitor &visitor, Pet *pet = nullptr)
  {
    visitor.visit(this, pet);
  }
};

class Cat : public Pet
{
public:
  using Pet::Pet;

  void accept(class PetVisitor &visitor, Pet *pet = nullptr)
  {
    visitor.visit(this, pet);
  }
};

class BirthVisitor : public PetVisitor
{
public:
  void visit(Dog *dog, Pet *pet) override
  {
    assert(dynamic_cast<Dog *>(pet));
    dog->addChild(pet);
  }

  void visit(Cat *cat, Pet *pet) override
  {
    assert(dynamic_cast<Cat *>(pet));
    cat->addChild(pet);
  }
};

class FamilyTreeVisitor : public PetVisitor
{
public:
  void visit(Dog *dog, Pet *pet) override
  {
    visitChildren(dog, "Puppies");
  }

  void visit(Cat *cat, Pet *pet) override
  {
    visitChildren(cat, "Kittens");
  }

  std::size_t getChildCount() const
  {
    return m_childCount;
  }

private:
  template <typename T>
  void visitChildren(const T *pet, const char *str)
  {
    std::cout << str << ": ";
    for (const auto &child : pet->getChildren())
    {
      std::cout << child->getName() << " ";
      ++m_childCount;
    }
    std::cout << std::endl;
  }

  std::size_t m_childCount{0};
};

int main()
{
  BirthVisitor birthVisitor;

  std::unique_ptr<Cat> parent = std::make_unique<Cat>("orange");
  std::unique_ptr<Cat> cat = std::make_unique<Cat>("calico");

  parent->accept(birthVisitor, cat.get());

  FamilyTreeVisitor familyTreeVisitor;
  parent->accept(familyTreeVisitor);

  std::cout << "Family tree of " << parent->getName() << " has " << familyTreeVisitor.getChildCount() << " children." << std::endl;

  return 0;
}
