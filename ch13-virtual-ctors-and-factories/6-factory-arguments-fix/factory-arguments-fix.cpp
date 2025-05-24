#include <iostream>

enum Buildings
{
  FARM,
  FORGE
};

class Building
{
public:
  virtual ~Building() {}
};

struct BuildingSpec
{
  virtual ~BuildingSpec() {}
  virtual Buildings type() const = 0;
};

class FarmSpec : public BuildingSpec
{
public:
  FarmSpec() = default;

  Buildings type() const override
  {
    return FARM;
  }

  FarmSpec &setPasture(bool withPasture)
  {
    this->withPasture = withPasture;
    return *this;
  }

  FarmSpec &setNumberOfStalls(int numberOfStalls)
  {
    this->numberOfStalls = numberOfStalls;
    return *this;
  }

  bool withPasture{false};
  int numberOfStalls{0};
};

class ForgeSpec : public BuildingSpec
{
public:
  ForgeSpec() = default;

  Buildings type() const override
  {
    return FORGE;
  }

  ForgeSpec &setMagicForge(bool magicForge)
  {
    this->magicForge = magicForge;
    return *this;
  }

  ForgeSpec &setNumberOfApprentices(int numberOfApprentices)
  {
    this->numberOfApprentices = numberOfApprentices;
    return *this;
  }

  bool magicForge{false};
  int numberOfApprentices{0};
};

class Farm : public Building
{
public:
  explicit Farm(const FarmSpec &spec) { std::cout << "new Farm, " << (spec.withPasture ? "with pasture" : "no pasture") << ", " << spec.numberOfStalls << " stalls" << std::endl; }
};

class Forge : public Building
{
public:
  explicit Forge(const ForgeSpec &spec) { std::cout << "new Forge, " << spec.numberOfApprentices << " apprentices, " << (spec.magicForge ? " magic" : "no magic") << std::endl; }
};

template <typename... Args>
std::unique_ptr<Building> makeBuilding(const BuildingSpec &spec)
{
  switch (spec.type())
  {
  case FARM:
    return std::make_unique<Farm>(static_cast<const FarmSpec &>(spec));
  case FORGE:
    return std::make_unique<Forge>(static_cast<const ForgeSpec &>(spec));
  default:
    return nullptr;
  }
}

int main()
{
  auto farm = makeBuilding(FarmSpec().setPasture(true).setNumberOfStalls(5));
  auto forge = makeBuilding(ForgeSpec().setNumberOfApprentices(2).setMagicForge(true));
}
