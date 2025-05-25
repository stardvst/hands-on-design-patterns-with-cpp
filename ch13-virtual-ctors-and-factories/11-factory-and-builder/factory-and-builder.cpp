#include <functional>
#include <vector>
#include <ostream>
#include <istream>
#include <sstream>
#include <iostream>

class SerializerBase
{
private:
  static std::size_t s_serializeTypeCount;

  using SerializeFactory = std::function<std::unique_ptr<SerializerBase>(std::istream &)>;
  static std::vector<SerializeFactory> s_serializeFactories;

public:
  virtual ~SerializerBase() = default;

  static std::size_t registerType(const SerializeFactory &factory)
  {
    s_serializeFactories.push_back(factory);
    return s_serializeTypeCount++;
  }

  static std::unique_ptr<SerializerBase> deserialize(std::size_t typeId, std::istream &in)
  {
    SerializeFactory factory = s_serializeFactories[typeId];
    return std::unique_ptr<SerializerBase>(factory(in));
  }

protected:
  virtual void serialize(std::ostream &out) const = 0;
};

std::size_t SerializerBase::s_serializeTypeCount = 0;
std::vector<SerializerBase::SerializeFactory> SerializerBase::s_serializeFactories;

class DeserializeFactory
{
public:
  explicit DeserializeFactory(std::istream &in) : m_in(in) {}

  template <typename It>
  void deserialize(It iter) const
  {
    while (true)
    {
      std::size_t type;
      m_in >> type;
      if (m_in.eof())
        break;
      iter = SerializerBase::deserialize(type, m_in);
    }
  }

private:
  std::istream &m_in;
};

///////////////////////////
// Example derived classes
//

class Derived1 : public SerializerBase
{
public:
  Derived1(int data) : m_data(data) { std::cout << "Derived1(" << m_data << ")\n"; }

  void serialize(std::ostream &out) const override
  {
    out << typeTag << " " << m_data << std::endl;
  }

  static const std::size_t typeTag;

private:
  int m_data;
};

const std::size_t Derived1::typeTag = SerializerBase::registerType([](std::istream &in)
                                                                   {
  int data;
  in >> data;
  return std::make_unique<Derived1>(data); });

class Derived2 : public SerializerBase
{
public:
  Derived2(double x, double y) : m_x(x), m_y(y) { std::cout << "Derived2(" << m_x << ", " << m_y << ")\n"; }

  void serialize(std::ostream &out) const override
  {
    out << typeTag << " " << m_x << " " << m_y << std::endl;
  }

  static const std::size_t typeTag;

private:
  double m_x, m_y;
};

const std::size_t Derived2::typeTag = SerializerBase::registerType([](std::istream &in)
                                                                   {
  double x, y;
  in >> x >> y;
  return std::make_unique<Derived2>(x, y); });

class Derived3 : public SerializerBase
{
public:
  Derived3(int i) : m_integer(true), m_i(i) { std::cout << "Derived3(" << i << ") - integer" << std::endl; }
  Derived3(double x) : m_integer(false), m_x(x) { std::cout << "Derived3(" << x << ") - double" << std::endl; }

  void serialize(std::ostream &s) const override
  {
    s << type_tag << " " << m_integer << " ";
    if (m_integer)
      s << m_i;
    else
      s << m_x;
    s << std::endl;
  }
  static const size_t type_tag;

private:
  bool m_integer;
  int m_i{};
  double m_x{};
};

const size_t Derived3::type_tag = SerializerBase::registerType([](std::istream &s)
                                                               {
    bool integer; s >> integer;
    if (integer) {
      int i; s >> i; return std::make_unique<Derived3>(i);
    } else {
      double x; s >> x; return std::make_unique<Derived3>(x);
    } });

int main()
{
  std::stringstream S;

  Derived1(42).serialize(S);
  Derived2(3.1415, 2.71828).serialize(S);
  Derived3(0.5772156649).serialize(S);
  Derived3(144).serialize(S);

  std::cout << S.str();

  std::vector<std::unique_ptr<SerializerBase>> v;
  DeserializeFactory F(S);
  F.deserialize(std::back_inserter(v));

  return 0;
}
