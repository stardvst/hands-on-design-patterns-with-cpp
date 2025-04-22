#pragma once

#include "html-element.h"
#include <concepts>

class HTML : public HtmlElement
{
public:
  HTML() : HtmlElement("html", "") {}
  HTML(std::initializer_list<HtmlElement> elements) : HtmlElement("html", "", std::move(elements)) {}
};

class HEAD : public HtmlElement
{
public:
  HEAD() : HtmlElement("head", "") {}
  HEAD(std::initializer_list<HtmlElement> elements) : HtmlElement("head", "", std::move(elements)) {}
};

class TITLE : public HtmlElement
{
public:
  TITLE(std::string_view text) : HtmlElement("title", text) {}
};

class BODY : public HtmlElement
{
public:
  BODY() : HtmlElement("body", "") {}
  BODY(std::initializer_list<HtmlElement> elements) : HtmlElement("body", "", std::move(elements)) {}
};

class OL : public HtmlElement
{
public:
  OL() : HtmlElement("ol", "") {}
  OL(std::initializer_list<HtmlElement> elements) : HtmlElement("ol", "", std::move(elements)) {}
};

class LI : public HtmlElement
{
public:
  LI(std::string_view text) : HtmlElement("li", text) {}

  // with this ctor, we need to wrap the second argument in {} to make it an initializer_list
  //
  // LI(std::string_view text, std::initializer_list<HtmlElement> elements) : HtmlElement("li", text, std::move(elements)) {}
  //

  // this ctor takes a variable number of children, and converts them to an initializer_list
  LI(std::string_view text, const std::derived_from<HtmlElement> auto &...children)
      : HtmlElement("li", text, std::initializer_list<HtmlElement>{children...})
  {
  }
};

class P : public HtmlElement
{
public:
  P(std::string_view text) : HtmlElement("p", text) {}
};
