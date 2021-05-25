#include <iostream>

#include "newSIR.hpp"

int main() {
  try {
    epidemia::SIR mondo = epidemia::insert();
    int x;
    std::cout << "Preferisce una rappresentazione grafica a griglia oppure una visualizzazione minimale dei dati?\n"
                 "Per la rappresentazione a schermo di una griglia premere 1, in caso contrario premere 0 \n";

    std::cin >> x;
    if (x != 0 && x != 1) {
      throw std::runtime_error{"Si può inserire solo 1 o 0 nella selezione del metodo di stampa"};
    }

    if (x == 1) {
      mondo.print(mondo.convertitore(mondo.riempimento()));
    } else if (x == 0) {
      mondo.print_semplice(mondo.convertitore(mondo.riempimento()));
    }
    ///////////
    /////////// così lo vedete ahahah
    ///////////
    ///////////
    //plot() con dentro le tre componenti del vettore direi;
  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << '\n';
  }
}
