#include <iostream>
#include <cstdlib>
#include <memory>
#include <functional>
#include <vector>

struct BuildingSpec
{
};

class FarmSpec : public BuildingSpec
{
public:
  FarmSpec() = default;

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

class Building
{
private:
  static std::size_t s_buildingTypeCount;

  using BuildingFactory = std::function<std::unique_ptr<Building>(const BuildingSpec &)>;
  static std::vector<BuildingFactory> s_buildingFactories;

public:
  virtual ~Building() = default;

  static std::size_t RegisterBuilding(BuildingFactory factory)
  {
    s_buildingFactories.push_back(std::move(factory));
    return s_buildingTypeCount++;
  }

  static std::unique_ptr<Building> CreateBuilding(std::size_t type, const BuildingSpec &spec)
  {
    BuildingFactory factory = s_buildingFactories[type];
    return std::unique_ptr<Building>(factory(spec));
  }
};

std::size_t Building::s_buildingTypeCount = 0;
std::vector<Building::BuildingFactory> Building::s_buildingFactories;

/////////////////////
// Example buildings
//

class Forge : public Building
{
public:
  explicit Forge(const ForgeSpec &spec) { std::cout << "new Forge, " << spec.numberOfApprentices << " apprentices, " << (spec.magicForge ? std::to_string(spec.magicForge) + " magic" : "no magic") << std::endl; }

  static std::unique_ptr<Building> MakeForge(const BuildingSpec &spec)
  {
    return std::make_unique<Forge>(static_cast<const ForgeSpec &>(spec));
  }

  static const std::size_t typeTag;
};

const std::size_t Forge::typeTag = Building::RegisterBuilding(Forge::MakeForge);

class Farm : public Building
{
public:
  explicit Farm(const FarmSpec &spec) { std::cout << "new Farm, " << (spec.withPasture ? "with pasture" : "no pasture") << ", " << spec.numberOfStalls << " stalls" << std::endl; }
  static const std::size_t typeTag;
};

const std::size_t Farm::typeTag = Building::RegisterBuilding([](const BuildingSpec &spec)
                                                             { return std::make_unique<Farm>(static_cast<const FarmSpec &>(spec)); });

int main()
{
  std::unique_ptr<Building> forge = Building::CreateBuilding(Forge::typeTag,
                                                             ForgeSpec().setMagicForge(true).setNumberOfApprentices(2));
  std::unique_ptr<Building> farm = Building::CreateBuilding(Farm::typeTag,
                                                            FarmSpec().setPasture(true).setNumberOfStalls(5));
}
