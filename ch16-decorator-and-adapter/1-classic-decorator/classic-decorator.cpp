#include <iostream>

class Unit
{
public:
  virtual ~Unit() = default;

  explicit Unit(double strength, double armor) : m_strength(strength), m_armor(armor) {}

  virtual bool hit(Unit &target) const
  {
    return attack() > target.defense();
  }

  virtual double attack() const = 0;
  virtual double defense() const = 0;

protected:
  double m_strength{0.0};
  double m_armor{0.0};
};

class Knight : public Unit
{
public:
  using Unit::Unit;

  double attack() const override
  {
    double res = m_strength + s_swordBonus + m_chargeBonus; // Knights can charge, adding a bonus to their attack
    m_chargeBonus = 0.0; // Reset charge bonus after attack
    return res;
  }

  double defense() const override
  {
    return m_armor + s_plateBonus; // Knights have a plate armor bonus
  }

  void charge()
  {
    m_chargeBonus = 1.0;
  }

protected:
  static constexpr double s_swordBonus = 2.0;
  static constexpr double s_plateBonus = 3.0;

  mutable double m_chargeBonus{0.0}; // Knights can charge, adding a bonus to their attack
};

class Ogre : public Unit
{
public:
  using Unit::Unit;

  double attack() const override
  {
    return m_strength + s_clubPenalty; // Ogres have a club penalty
  }

  double defense() const override
  {
    return m_armor + s_leatherPenalty; // Ogres have a leather armor penalty
  }

protected:
  static constexpr double s_clubPenalty = -1.0;
  static constexpr double s_leatherPenalty = -1.0;
};

class VeternanUnit : public Unit
{
public:
  VeternanUnit(Unit &unit, double strengthBonus, double armorBonus)
      : Unit(strengthBonus, armorBonus), m_unit(unit)
  {
  }

  double attack() const override
  {
    return m_unit.attack() + m_strength; // Veteran units get a strength bonus
  }

  double defense() const override
  {
    return m_unit.defense() + m_armor; // Veteran units get an armor bonus
  }

private:
  Unit &m_unit;
};

int main()
{
  Knight knight(10.0, 5.0);
  Ogre ogre(12.0, 2.0);

  knight.charge(); // Knight charges, adding a bonus to the next attack

  if (knight.hit(ogre))
    std::cout << "Knight hits the Ogre!" << std::endl;
  else
    std::cout << "Knight misses the Ogre." << std::endl;

  if (ogre.hit(knight))
    std::cout << "Ogre hits the Knight!" << std::endl;
  else
    std::cout << "Ogre misses the Knight." << std::endl;

  {
    VeternanUnit veteranKnight(knight, 7.0, 2.0);
    VeternanUnit veteranOgre(ogre, 1.0, 9.0);

    // doesn't compile because VeternanUnit is not a Knight and doesn't have a charge method
    // veteranKnight.charge(); // Veteran Knight charges, adding a bonus to the next attack

    if (veteranKnight.hit(veteranOgre))
      std::cout << "Veteran Knight hits the Veteran Ogre!" << std::endl;
    else
      std::cout << "Veteran Knight misses the Veteran Ogre." << std::endl;

    {
      VeternanUnit veteranOgre2(veteranOgre, 1.0, 9.0);
      if (veteranKnight.hit(veteranOgre2))
        std::cout << "Veteran Knight hits the Veteran Ogre 2!" << std::endl;
      else
        std::cout << "Veteran Knight misses the Veteran Ogre 2." << std::endl;
    }
  }

  return 0;
}
