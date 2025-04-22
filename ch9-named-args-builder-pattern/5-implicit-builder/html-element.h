#pragma once

#include <string>
#include <vector>
#include <iostream>

class HtmlElement
{
public:
  HtmlElement(std::string_view name, std::string_view text)
      : m_name(name), m_text(text)
  {
  }

  HtmlElement(std::string_view name, std::string_view text, std::vector<HtmlElement> &&elements)
      : m_name(name), m_text(text), m_elements(std::move(elements))
  {
  }

  friend std::ostream &operator<<(std::ostream &os, const HtmlElement &element);

private:
  const std::string m_name;
  const std::string m_text;
  const std::vector<HtmlElement> m_elements;
};

std::ostream &operator<<(std::ostream &os, const HtmlElement &element)
{
  os << "<" << element.m_name << ">\n";
  if (!element.m_text.empty())
    os << "  " << element.m_text << "\n";
  for (const auto &e : element.m_elements)
    os << e;
  os << "</" << element.m_name << ">\n";
  return os;
}
