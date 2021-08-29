#include <iostream>

#include "lyra.hpp"
#include "newSIR.hpp"

int main(int argc, const char** argv) {
  try {
    std::string opzione;
    bool mostra_aiuto = false;
    auto list = lyra::help(mostra_aiuto) | lyra::opt(opzione, "opzione")["-f"]["--formato"](
                                               "Permette di formattare in colonne separate da virgole con \"virgola\"; da spazi con \"spazio\"; "
                                               "permette di creare solo il grafico con \"grafico\"");
    auto parser = list.parse({argc, argv});

    if (!parser) {
      throw std::runtime_error{"inserisci un'opzione valida"};
      std::cerr << list << "\n";
    }

    if (mostra_aiuto) {
      std::cout << list << "\n";
      return 0;  
    }

    if (opzione == "virgola") {
      epidemia::SIR mondo = epidemia::Insert();
      mondo.PrintSempliceVirgola(mondo.Convertitore(mondo.Riempimento()));
    } else if (opzione == "spazio") {
      epidemia::SIR mondo = epidemia::Insert();
      mondo.PrintSempliceSpazio(mondo.Convertitore(mondo.Riempimento()));
    } else if (opzione == "grafico") {
      epidemia::SIR mondo = epidemia::Insert();
      mondo.PrintGrafico(mondo.Convertitore(mondo.Riempimento()));
    } else if (opzione.empty()) {
      epidemia::SIR mondo = epidemia::Insert();
      mondo.Print(mondo.Convertitore(mondo.Riempimento()));
      mondo.PrintGrafico(mondo.Convertitore(mondo.Riempimento()));
    } else {
      throw std::runtime_error{"inserisci un'opzione valida"};
    }

  } catch (std::runtime_error const& e) {
    std::cerr << "ERRORE: " << e.what() << '\n';
  }
}
