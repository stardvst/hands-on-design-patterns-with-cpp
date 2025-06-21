#include <iostream>
#include <sstream>

class Point;
class Circle;
class Line;
class Intersection;

class Geometry
{
public:
  virtual ~Geometry() = default;

  enum type_tag
  {
    POINT = 100,
    CIRCLE,
    LINE,
    INTERSECTION
  };

  virtual type_tag tag() const = 0;

  virtual void accept(class Visitor &visitor) = 0;
};

class Visitor
{
public:
  virtual ~Visitor() = default;

  static Geometry *create(Geometry::type_tag tag);

  virtual void visit(Geometry::type_tag &tag) = 0;

  virtual void visit(double &x) = 0;
  virtual void visit(Point &x) = 0;
  virtual void visit(Circle &x) = 0;
  virtual void visit(Line &x) = 0;
  virtual void visit(Intersection &x) = 0;
};

class Point : public Geometry
{
public:
  Point() = default;
  Point(double x, double y) : m_x(x), m_y(y) {}

  type_tag tag() const override { return POINT; }

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

  type_tag tag() const override { return CIRCLE; }

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

  type_tag tag() const override { return LINE; }

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

class Intersection : public Geometry
{
public:
  Intersection() = default;
  Intersection(Geometry *g1, Geometry *g2) : m_g1(g1), m_g2(g2) {}

  type_tag tag() const override { return INTERSECTION; }

  void accept(Visitor &visitor) override
  {
    Geometry::type_tag tag;
    if (m_g1)
      tag = m_g1->tag();
    visitor.visit(tag);

    if (!m_g1)
      m_g1.reset(Visitor::create(tag));
    m_g1->accept(visitor);

    if (m_g2)
      tag = m_g2->tag();
    visitor.visit(tag);

    if (!m_g2)
      m_g2.reset(Visitor::create(tag));
    m_g2->accept(visitor);
  }

private:
  std::unique_ptr<Geometry> m_g1{};
  std::unique_ptr<Geometry> m_g2{};
};

Geometry *Visitor::create(Geometry::type_tag tag)
{
  switch (tag)
  {
  case Geometry::type_tag::POINT:
    return new Point();
  case Geometry::type_tag::CIRCLE:
    return new Circle();
  case Geometry::type_tag::LINE:
    return new Line();
  default:
    return nullptr;
  }
}

class StreamVisitor : public Visitor
{
public:
  explicit StreamVisitor(std::ostream &out) : m_out(out) {}

  void visit(Geometry::type_tag &tag) override {}
  void visit(double &x) override
  {
    indent();
    m_out << x << std::endl;
  }
  void visit(Point &p) override { visit_impl(p); }
  void visit(Circle &c) override { visit_impl(c); }
  void visit(Line &l) override { visit_impl(l); }
  void visit(Intersection &x) override { visit_impl(x); }

private:
  void indent()
  {
    for (int i = 0; i < m_indent; ++i)
      m_out << " ";
  }

  template <typename T>
  void visit_impl(T &geom)
  {
    m_indent += 2;
    geom.accept(*this);
    m_indent -= 2;
  }

  std::ostream &m_out;
  int m_indent{0};
};

int main()
{
  Point p(1, 2);
  Circle c(p, 4);
  Line l(p, p);
  Intersection x(new Circle(p, 4), new Line(p, p));

  StreamVisitor writer(std::cout);
  std::cout << "Line l:\n";
  writer.visit(l);
  std::cout << "\n\n#########################\nCircle c:\n";
  writer.visit(c);
  std::cout << "\n\n#########################\nIntersection x:\n";
  writer.visit(x);
}
