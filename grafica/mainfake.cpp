#include <iostream>

#include "classe.hpp"
//#include <SFML/OpenGL.hpp>

int main() {
  
  
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window(sf::VideoMode(600, 600, desktop.bitsPerPixel), "Tilemap");
  window.setFramerateLimit(60);
  int n;
  std::cin >> n;
  Mondo map(n);
  map.settexture();

  while (window.isOpen()) {
    // handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    // draw the map
    window.clear();

    map.azzera();
    map.evolvi();
    map.aggiornagriglia();
    window.draw(map);
    window.display();
    // glFinish();
  }
}