#include <iostream>
#include <cassert>

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
    m_chargeBonus = 0.0;                                    // Reset charge bonus after attack
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

template <typename To, typename From>
std::unique_ptr<To> move_cast(std::unique_ptr<From> &from)
{
#ifndef NDEBUG
  auto p = std::unique_ptr<To>(dynamic_cast<To *>(from.release()));
  assert(p.get());
  return p;
#else
  return std::unique_ptr<To>(static_cast<To *>(from.release()));
#endif
}

template <typename U>
class VeternanUnit : public U
{
public:
  template <typename P>
  VeternanUnit(P &&unit, double strengthBonus, double armorBonus)
      : U(std::move(*move_cast<U>(unit))), m_strengthBonus(strengthBonus), m_armorBonus(armorBonus)
  {
  }

  double attack() const override
  {
    return U::attack() + m_strengthBonus; // Veteran units get a strength bonus
  }

  double defense() const override
  {
    return U::defense() + m_armorBonus; // Veteran units get an armor bonus
  }

private:
  double m_strengthBonus{};
  double m_armorBonus{};
};

using UnitPtr = std::unique_ptr<Unit>;
using KnightPtr = std::unique_ptr<Knight>;

int main()
{
  KnightPtr knight(new Knight(10.0, 5.0));
  UnitPtr ogre(new Ogre(8.0, 4.0));

  KnightPtr vk(new VeternanUnit<Knight>(knight, 2.0, 1.0));
  UnitPtr vo(new VeternanUnit<Ogre>(ogre, 1.0, 2.0));
  UnitPtr vvo(new VeternanUnit<VeternanUnit<Ogre>>(std::move(vo), 1.0, 9.0));

  if (vk->hit(*vvo))
    std::cout << "Veteran Knight hits the Veteran Ogre!" << std::endl;
  else
    std::cout << "Veteran Knight misses the Veteran Ogre." << std::endl;

  vk->charge(); // Knight charges, adding a bonus to the next attack

  if (vk->hit(*vvo))
    std::cout << "Veteran Knight hits the Veteran Ogre after charging!" << std::endl;
  else
    std::cout << "Veteran Knight misses the Veteran Ogre after charging." << std::endl;

  return 0;
}
