#include <iostream>

#include "classe.hpp"
//#include <SFML/OpenGL.hpp>

int main() {
  
  Mondo map;

  while (!map.Prendi_finestra()->Isclosed()) {
    // handle events
    
    map.Gestisci_input();
    map.Aggiorna();
    map.Disegna();
    //map.azzera();
    // glFinish();
  }
}