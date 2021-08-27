#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>

#include "finestra.hpp"
#include "gui.hpp"

#ifndef CLASSE_HPP
#define CLASSE_HPP

// per disegnare altre cose oltre il vertex array
/* class Rappresentazione : public sf::Drawable { */

class Mondo /* : public sf::Drawable  */ {
 private:  // la draw non va nel protected??
  /* sf::VertexArray Griglia; */
  GUI m_overlay;

  Animazione m_dinamica;  // fare altra classe che contiene sia griglia sia bordi, gestire tutto lì

  Automa m_statica;
  // non ho capito perhcè qui dentro non ci posso mettere il costruttore;

  Finestra m_window;
  /* virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_ominoprova;
    // capire che madonna succede qui dentro!!!
    target.draw(Griglia, states);
  } */

  /* void settexture(); */

  // perchè la devo mettere qua
  // virtual qua non ce lo devo mettere, giusto? tanto non creerò classi derivate da questa
  // poi come fa window.draw(entity) a chiamare internamente entity.draw se è nella parte privata?!

 public:
  Mondo();

  // funzioni da usare nel main


  Finestra* Prendi_finestra() { return &m_window; }

  void Gestisci_input() {
    m_window.Update();  // gestisce gli eventi
    if (m_window.Isclosed()) {
      m_window.~Finestra();
    }
  }

  void Aggiorna() {
    if (m_window.GetVista() == Vista::Animazione ) {
      
      if(!m_dinamica.IsStopped()){
      m_dinamica.Aggiorna_Generale();
      std::cout << "Aggiorno dinamica" << '\n';
      m_statica.AzzeraOrologio();
      }
      
    } else {
      m_statica.Avanza();
      std::cout << "Aggiorno statica" << '\n';
      m_dinamica.AzzeraOrologio();
    }
  }

  void Disegna() {
    m_window.Pulisci();
    if (m_window.GetVista() == Vista::Animazione) {
      m_window.Disegna(m_dinamica);
      std::cout << "DIsegno dinamica" << '\n';

    } else {
      m_window.Disegna(m_statica);
      std::cout << "DIsegno statica" << '\n';
    }

    m_window.Disegna(m_overlay);

    m_window.Mostra();
  }
};

#endif