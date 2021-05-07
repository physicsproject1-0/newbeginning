#include <cassert>
#include <iostream>

#include "newSIR.hpp"

int main() {
  epidemia::SIR mondo = epidemia::insert();
  mondo.riempimento();

  int x;
  std::cout << "Preferisce una rappresentazione grafica a griglia oppure una visualizzazione minimale dei dati?\n"
               "Per la rappresentazione a schermo di una griglia premere 1, in caso contrario premere 0 \n";
  std::cin >> x;
  assert(x == 0 || x == 1);         //Da mettere un'exception al posto dell'assert
  if (x == 1) {
    mondo.print();
  } else if (x == 0) {
    mondo.print_semplice();
  }
}
