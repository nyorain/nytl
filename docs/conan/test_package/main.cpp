#include "nytl/flags.hpp"

enum testEnum {one, two, three};


int main (){
  nytl::Flags<testEnum, unsigned int> flags;
  return 0;
}
