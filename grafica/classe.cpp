#include "classe.hpp"

#include <algorithm>


Mondo::Mondo()
    :m_overlay(sf::Vector2f(70, 500)) , m_dinamica(50), m_statica(sf::Vector2f(1800, 1800), sf::Vector2f(500, 500), 3, 0.3, 0.5, 0, 0), m_window("test", sf::Vector2u(900, 600), &m_overlay, &m_dinamica, &m_statica,  m_dinamica.get_bordi(), m_statica.get_bordi()){
  /*   if (!ominoprova.loadFromFile("uomoverde.png")) {
      throw std::runtime_error{"texture loading failed"};
    }
   */
}
