#include <iostream>

#include "classe.hpp"
//#include <SFML/OpenGL.hpp>

int main() {
  
  Inserimento inserimento_dati;

  Mondo map(inserimento_dati);

  while (!map.Prendi_finestra()->Isclosed()) {
    // handle events
    
    map.Gestisci_input();
    map.Aggiorna();
    map.Disegna();
    //map.azzera();
    // glFinish();
  }
}