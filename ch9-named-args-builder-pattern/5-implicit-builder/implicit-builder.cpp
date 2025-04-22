#include "elements.h"

int main()
{
  auto doc = HTML{
      HEAD{
          TITLE{"Hello, World!"}},
      BODY{
          P{"p tag!"},
          OL{
              LI{"li tag 1!",
                 OL{
                     LI{"ol -> li tag!"},
                 }},
              LI{"li tag 2!"},
              LI{"li tag 3!"}}}};

  std::cout << doc;

  return 0;
}
