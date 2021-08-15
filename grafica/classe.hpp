#include <math.h>

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>

#include "finestra.hpp"

#ifndef DISA
#define DISA

struct Persona {
  sf::Vector2f centro;
  float raggio;
  sf::Vector2f vel;
  sf::Clock cambiovelocita;
  sf::FloatRect scia;
  float metalato;
  Persona getGlobalBounds();  // ?
  bool intersects();          // ?
  bool infect();
  bool try_infect();  // ?
};

class Mondo : public sf::Drawable {
 private:  // la draw non va nel protected??
  sf::VertexArray Griglia;

  std::map<int, Persona> Lista;  // contiene solo le persone

  sf::Clock timer;
  sf::Time trascorso;
  sf::Texture ominoprova;
  Finestra a_window;

  enum Status { VULNERABLE, INFECTED, REMOVED };  // Lo status e' qualcosa della persona non del mondo, forse va spostato

  Status S;  // Qualcosa di questo genere per le diverse texture

  /*
    switch (S) {                                                            // Qualcosa non funziona
      case (INFECTED):  // Carichiamo la red texture...
        if (!ominoprova.loadFromFile("uomorosso.png")) {
          throw std::runtime_error{"texture loading failed"};
        }
        break;

      case (REMOVED):  // carichiamo la white texture
        if (!ominoprova.loadFromFile("uomogrigio.png")) {
          throw std::runtime_error{"texture loading failed"};
        }
        break;

      default:  // carichiamo la green texture
        if (!ominoprova.loadFromFile("uomoverde.png")) {
          throw std::runtime_error{"texture loading failed"};
        }
    }
  */

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &ominoprova;
    // capire che madonna succede qui dentro!!!
    target.draw(Griglia, states);
  }

  void settexture();

  // perchè la devo mettere qua
  // virtual qua non ce lo devo mettere, giusto? tanto non creerò classi derivate da questa
  // poi come fa window.draw(entity) a chiamare internamente entity.draw se è nella parte privata?!

 public:
  Mondo(int persone);

  void aggiornagriglia();

  void create_sweptvolume(Persona& persona);

  void evolvi_singolo(int indice);

  bool check_collisions();

  void change_status();  // Fa cambiare la texture nel momento in cui le due particelle si scontrano

  void change_vel();

  void evolvi();

  int check_occur(Persona const& persona, int raggio);

  double modulo(sf::Vector2f const& vettore);

  // funzioni da usare nel main

  void azzera();

  Finestra* prendi_finestra() { return &a_window; }

  void Gestisci_input() {
    a_window.Update();  // gestisce gli eventi
    if (a_window.Isclosed()) {
      a_window.~Finestra();
    }
  }

  void Aggiorna() {
    evolvi();
    aggiornagriglia();
  }

  void Disegna() {
    a_window.Pulisci();
    a_window.Disegna(*this);
    a_window.Mostra();
  }
};

#endif