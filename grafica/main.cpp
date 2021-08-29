#include <iostream>

#include "mondo.hpp"

int main() {
  try {
    Inserimento inserimento_dati;

    Mondo map(inserimento_dati);

    while (!map.Prendi_finestra()->IsClosed()) {

      map.GestisciInput();
      map.Aggiorna();
      map.Disegna();

    }
  } catch (std::runtime_error const& e) {
    std::cerr << "ERRORE: " << e.what() << '\n';
  }
}