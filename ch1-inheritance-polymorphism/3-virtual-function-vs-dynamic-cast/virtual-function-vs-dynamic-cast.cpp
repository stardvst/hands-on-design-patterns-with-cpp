#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

class Timer
{
public:
  explicit Timer(const std::string &label) : m_startTime(std::chrono::high_resolution_clock::now()), m_label(label) {}

  ~Timer()
  {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsedTime = endTime - m_startTime;
    std::cout << m_label << " elapsed time: " << elapsedTime.count() << " ms" << std::endl;
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
  std::string m_label;
};

class Base
{
public:
  virtual ~Base() = default;
  virtual void doSomething() const {}
};

class Derived : public Base
{
public:
  void doSomething() const override {}
};

void measureVirtualCallPerformance(const std::vector<std::unique_ptr<Base>> &objects)
{
  Timer timer("Virtual function call");
  for (const auto &obj : objects)
    obj->doSomething();
}

void measureDynamicCastPerformance(const std::vector<std::unique_ptr<Base>> &objects)
{
  Timer timer("Dynamic cast");
  for (const auto &obj : objects)
  {
    if (Derived *d1 = dynamic_cast<Derived *>(obj.get()))
      d1->doSomething();
  }
}

int main()
{
  constexpr size_t numObjects = 1000000;
  std::vector<std::unique_ptr<Base>> objects;
  objects.reserve(numObjects);

  for (size_t i = 0; i < numObjects; ++i)
    objects.emplace_back(std::make_unique<Derived>());

  measureVirtualCallPerformance(objects);
  measureDynamicCastPerformance(objects);

  return 0;
}
