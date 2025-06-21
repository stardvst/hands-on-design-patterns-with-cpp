#include <iostream>
#include <sstream>

class Visitor;
class Point;
class Circle;
class Line;

class Geometry
{
public:
  virtual ~Geometry() = default;

  virtual void accept(class Visitor &visitor) = 0;
};

class Visitor
{
public:
  virtual ~Visitor() = default;

  virtual void visit(double &x) = 0;
  virtual void visit(Point &p) = 0;
  virtual void visit(Circle &c) = 0;
  virtual void visit(Line &l) = 0;
};

class Point : public Geometry
{
public:
  Point() = default;
  Point(double x, double y) : m_x(x), m_y(y) {}

  void accept(class Visitor &visitor) override
  {
    std::cout << "Visiting Point" << std::endl;
    visitor.visit(m_x);
    visitor.visit(m_y);
    std::cout << "Done visiting Point" << std::endl;
  }

private:
  double m_x{};
  double m_y{};
};

class Circle : public Geometry
{
public:
  Circle() = default;
  Circle(Point p, double r) : m_p(std::move(p)), m_r(r) {}

  void accept(class Visitor &visitor) override
  {
    std::cout << "\nVisiting Circle" << std::endl;
    visitor.visit(m_p);
    visitor.visit(m_r);
    std::cout << "Done visiting Circle" << std::endl;
  }

private:
  Point m_p{};
  double m_r{};
};

class Line : public Geometry
{
public:
  Line() = default;
  Line(Point p1, Point p2) : m_p1(std::move(p1)), m_p2(std::move(p2)) {}

  void accept(class Visitor &visitor) override
  {
    std::cout << "\nVisiting Line" << std::endl;
    visitor.visit(m_p1);
    visitor.visit(m_p2);
    std::cout << "Done visiting Line" << std::endl;
  }

private:
  Point m_p1{};
  Point m_p2{};
};

class StringSerializeVisitor : public Visitor
{
public:
  void visit(double &x) override
  {
    std::cout << "\tVisiting double" << std::endl;
    m_sstr << x << "-";
    std::cout << "\tDone visiting double" << std::endl;
  }

  void visit(Point &p) override { p.accept(*this); }
  void visit(Circle &c) override { c.accept(*this); }
  void visit(Line &l) override { l.accept(*this); }

  std::string str() const { return m_sstr.str(); }

private:
  std::stringstream m_sstr;
};

class StringDeserializeVisitor : public Visitor
{
public:
  explicit StringDeserializeVisitor(const std::string &s)
  {
    m_sstr.str(s);
  }

  void visit(double &x) override
  {
    std::cout << "\tDeserializing double" << std::endl;
    char c;
    m_sstr >> x >> c;
    std::cout << "\tDone deserializing double: " << x << ", separator: " << c << std::endl;
  }

  void visit(Point &p) override { p.accept(*this); }
  void visit(Circle &c) override { c.accept(*this); }
  void visit(Line &l) override { l.accept(*this); }

private:
  std::stringstream m_sstr;
};

int main()
{
  Point p(1, 2);
  Circle c(p, 4);

  StringSerializeVisitor visitor;
  c.accept(visitor);

  Line l(p, p);
  l.accept(visitor);

  std::string serialized = visitor.str();
  std::cout << "The serialized string: " << serialized << "\n\n" << std::endl;

  std::cout << "#### Deserialization ####" << std::endl;

  StringDeserializeVisitor deserializer(serialized);
  c.accept(deserializer);
  l.accept(deserializer);
}
