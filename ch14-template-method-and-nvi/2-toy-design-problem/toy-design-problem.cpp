#include <iostream>

class Character
{
public:
  virtual ~Character() = default;
  virtual void CombatTurn() = 0;

  void set_health(int h) { m_health = h; } // For testing

protected:
  Character(std::string_view name, int health) : m_name(name), m_health(health) {}

  void Flee() { std::cout << m_name << " runs for his life" << std::endl; }
  void Attack() { std::cout << m_name << " charges at his enemies" << std::endl; }

  std::string m_name;
  int m_health = 100;
};

class Swordsman : public Character
{
public:
  explicit Swordsman(std::string_view name) : Character(name, 10) {}

  void CombatTurn() override
  {
    if (m_health < 5)
    {
      Flee();
      return;
    }

    if (!m_wieldingSword)
    {
      Wield();
      return;
    }

    Attack();
  }

private:
  void Wield()
  {
    m_wieldingSword = true;
    std::cout << m_name << " readies his sword" << std::endl;
  }

  bool m_wieldingSword = false;
};

/// Too much code duplication in the CombatTurn method
/// of the Wizard class, which is similar to the Swordsman class.
class Wizard : public Character
{
public:
  explicit Wizard(std::string_view name) : Character(name, 4) {}

  void CombatTurn() override
  {
    if (m_health < 2 || m_mana == 0)
    {
      Flee();
      return;
    }

    if (!m_scrollReady)
    {
      ReadScroll();
      return;
    }

    CastSpell();
  }

private:
  void ReadScroll()
  {
    m_scrollReady = true;
    std::cout << m_name << " unfurls the scroll of doom" << std::endl;
  }
  void CastSpell()
  {
    std::cout << m_name << " unleashes the wrath of Arcana" << std::endl;
    --m_mana;
  }

  int m_mana = 12;
  bool m_scrollReady = false;
};

int main()
{
  Swordsman Arthur("Arthur");
  Wizard Merlin("Merlin");

  Arthur.CombatTurn();
  Merlin.CombatTurn();

  Arthur.set_health(3);
  Arthur.CombatTurn();
  Merlin.CombatTurn();
  Merlin.CombatTurn();

  Merlin.set_health(1);
  Merlin.CombatTurn();

  Arthur.set_health(50);
  Arthur.CombatTurn();
}
