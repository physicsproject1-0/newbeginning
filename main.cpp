#include <cassert>
#include <iostream>

#include "newSIR.hpp"

int main() {
  epidemia::SIR mondo = epidemia::insert();
  mondo.riempimento();
  mondo.print();
}
