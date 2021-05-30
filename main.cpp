#include <iostream>

#include "lyra.hpp"
#include "newSIR.hpp"

int main(int argc, const char** argv) {
  try {
    std::string opzione;
    bool mostra_aiuto = false;
    auto list = lyra::help(mostra_aiuto) | lyra::opt(opzione, "opzione")["-f"]["--formato"](
                                               "Permette di formattare in colonne separate da virgole con \"virgola\"; da spazi con \"spazio\"; permette di creare solo il grafico con \"grafico\"");
    auto parser = list.parse({argc, argv});

    if (!parser) {
      throw std::runtime_error{"inserisci un'opzione valida"};
      std::cerr << list << "\n";
    }

    if (mostra_aiuto) {
      std::cout << list << "\n";
      return 0;  // va bene qua?
    }

    if (opzione == "virgola") {
      epidemia::SIR mondo = epidemia::insert();
      mondo.print_semplice_virgola(mondo.convertitore(mondo.riempimento()));
    } else if (opzione == "spazio") {
      epidemia::SIR mondo = epidemia::insert();
      mondo.print_semplice_spazio(mondo.convertitore(mondo.riempimento()));
    } else if (opzione == "grafico") {
      epidemia::SIR mondo = epidemia::insert();
      mondo.print_grafico(mondo.convertitore(mondo.riempimento()));
    } else if (opzione.empty()) {
      epidemia::SIR mondo = epidemia::insert();
      mondo.print(mondo.convertitore(mondo.riempimento()));
      mondo.print_grafico(mondo.convertitore(mondo.riempimento()));
    } else {
      throw std::runtime_error{"inserisci un'opzione valida"};
    }

    /*
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
*/

    } catch (std::runtime_error const& e) {
    std::cerr << "ERRORE: " << e.what() << '\n';
  }
}
