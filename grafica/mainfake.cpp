#include <iostream>

#include "classe.hpp"
//#include <SFML/OpenGL.hpp>

int main() {
  
  
  
  int n;
  std::cin >> n;
  Mondo map(n);

  while (map.prendi_finestra()->Isclosed()) {
    // handle events
    map.Gestisci_input();
    map.Aggiorna();
    map.Disegna();
    // glFinish();
  }
}