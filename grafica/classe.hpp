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

  bool checked = false;
  float metalato;
  Persona getGlobalBounds();  // ?
  bool intersects();          // ?
  bool infect();
  bool try_infect();  // ?
};

// per disegnare altre cose oltre il vertex array

class Griglia : public sf::Drawable {
  sf::Texture ominoprova;
  sf::VertexArray struttura;
  int numeropersone;

  void settexturecoords() {
    for (int i = 0; i < numeropersone; i++) {
      sf::Vertex* iter = &struttura[i * 3];
      // iter[0].color = sf::Color::Transparent;
      // iter[1].color = sf::Color::Transparent;
      // iter[2].color = sf::Color::Transparent;
      iter[0].texCoords = sf::Vector2f(430.f, 0.f);  // strane coord
      iter[1].texCoords = sf::Vector2f(0.f, 1681.f);
      iter[2].texCoords = sf::Vector2f(860.f, 1681.f);

      //  C'è UN MEMORY LEAK
      
    }
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &ominoprova;
    // capire che madonna succede qui dentro!!!
    target.draw(struttura, states);
  }

 public:
  void begin(const std::map<int, Persona>& list) {
    if (!ominoprova.loadFromFile("uomoverde.png")) {
      throw std::runtime_error{"texture loading failed"};
    }
    numeropersone = list.size();
    struttura.resize(list.size() * 3);

    struttura.setPrimitiveType(sf::Triangles);

    settexturecoords();
  }

  void aggiorna(const std::map<int, Persona>& list) {
    for (int i = 0; i < numeropersone; i++) {
      sf::Vertex* iter = &struttura[i * 3];
      iter[0].position = sf::Vector2f(list.at(i).centro.x, list.at(i).centro.y - list.at(i).raggio);  // strane coord
      iter[1].position = sf::Vector2f(list.at(i).centro.x - list.at(i).raggio * (1.7f / 2), list.at(i).centro.y + (list.at(i).raggio / 2));
      iter[2].position = sf::Vector2f(list.at(i).centro.x + list.at(i).raggio * (1.7f / 2), list.at(i).centro.y + (list.at(i).raggio / 2));
      // bisognerà cancellare il puntatore?

      // non posso usare [] perchè mi dice che non funziona per le robe const
    }
  }
};

class Bordi : public sf::Drawable {
  sf::RectangleShape rettangolo;
  sf::CircleShape cerchio;
  sf::FloatRect bordo_collisioni;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(rettangolo);
    target.draw(cerchio);
  }  // metterci anche states altrimenti rompe il casso

 public:
  Bordi(sf::Vector2f dimensione) {
    rettangolo.setSize(dimensione);
    rettangolo.setFillColor(sf::Color::Transparent);
    rettangolo.setOutlineColor(sf::Color::White);
    rettangolo.setOutlineThickness(3);
    rettangolo.setPosition(100, 100);

    cerchio.setPosition(0, 0);
    cerchio.setRadius(10);
    cerchio.setFillColor(sf::Color::Transparent);
    cerchio.setOutlineColor(sf::Color::White);
    cerchio.setOutlineThickness(-2);

    bordo_collisioni.left = 100;
    bordo_collisioni.top = 100;
    bordo_collisioni.width = dimensione.x;
    bordo_collisioni.height = dimensione.y;
  }

  sf::FloatRect getlimiti() { return bordo_collisioni; };
};

enum class Status { VULNERABLE, INFECTED, REMOVED };

/* class Automa : public sf::Drawable {
  class Cellula : public sf::Drawable {  // se è una struct non funziona l'inheritance?
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
      target.draw(rettangolo);
    }  // metterci anche states altrimenti rompe il casso
    void aggiorna_colore() {
      switch (S) {                // Qualcosa non funziona
        case (Status::INFECTED):  // Carichiamo la red texture...
          rettangolo.setFillColor(sf::Color::Red);
          break;

        case (Status::REMOVED):  // carichiamo la white texture
          rettangolo.setFillColor(sf::Color::Cyan);
          break;

        case (Status::VULNERABLE):  // carichiamo la green texture

          rettangolo.setFillColor(sf::Color::Green);
          break;
      }
    }

   public:
    Status S = Status::VULNERABLE;
    sf::RectangleShape rettangolo;

    Cellula(sf::Vector2f posizione, sf::Vector2f dimensione) {
      rettangolo.setPosition(posizione);
      rettangolo.setSize(dimensione);
      rettangolo.setOutlineColor(sf::Color::White);
      rettangolo.setOutlineThickness(2.f);
      aggiorna_colore();
    }
  };
  // mettere errori per dimensioni minori di 0?
  std::vector<std::vector<Cellula>> grid;

  sf::Vector2f dimensioni;
  sf::Vector2f posizione;
  int numero_lato;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int i = 0; i < numero_lato; i++) {
      for (int j = 0; j < numero_lato; j++) {
        target.draw(grid[i][j]);
      }
    }
  }

 public:
  Automa(sf::Vector2f t_dimensione, sf::Vector2f t_posizione, int t_numero)
      : dimensioni{t_dimensione}, posizione{t_posizione}, numero_lato{t_numero} {
    float t_lunghezza_x = dimensioni.x / numero_lato;
    float t_lunghezza_y = dimensioni.y / numero_lato;

    for (int i = 0; i < numero_lato; i++) {
      for (int j = 0; j < numero_lato; j++) {
        sf::Vector2f posizionemovente(posizione.x + j * t_lunghezza_x, posizione.y + i * t_lunghezza_y);
        Cellula riempi(posizionemovente, sf::Vector2f(t_lunghezza_x, t_lunghezza_y));
        grid[i][j] = riempi;
        // i per le colonne, j per le righe
      }
    }
  }
};
 */
class Mondo /* : public sf::Drawable  */ {
 private:  // la draw non va nel protected??
  /* sf::VertexArray Griglia; */
  Finestra a_window;

  std::map<int, Persona> Lista;  // contiene solo le persone

  Griglia uomini; //fare altra classe che contiene sia griglia sia bordi, gestire tutto lì

  Bordi limiti;  // non ho capito perhcè qui dentro non ci posso mettere il costruttore;

  sf::Clock timer;
  sf::Time trascorso;

  enum Status { VULNERABLE, INFECTED, REMOVED };  // Lo status e' qualcosa della persona non del mondo, forse va spostato

  Status S;  // Qualcosa di questo genere per le diverse texture

  /* Automa rappresentazione; */
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

  /* virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &ominoprova;
    // capire che madonna succede qui dentro!!!
    target.draw(Griglia, states);
  } */

  /* void settexture(); */

  // perchè la devo mettere qua
  // virtual qua non ce lo devo mettere, giusto? tanto non creerò classi derivate da questa
  // poi come fa window.draw(entity) a chiamare internamente entity.draw se è nella parte privata?!

 public:
  Mondo(int persone);

  void aggiornagriglia();

  void check_collisions();

  void check_borders() {
    for (int i = 0; i < Lista.size(); i++) {
      if (Lista[i].centro.x < limiti.getlimiti().left + Lista[i].raggio ||
          Lista[i].centro.x > limiti.getlimiti().left + limiti.getlimiti().width - Lista[i].raggio) {
        Lista[i].vel.x = -Lista[i].vel.x;
      }
      if (Lista[i].centro.y < limiti.getlimiti().top + Lista[i].raggio ||
          Lista[i].centro.y > limiti.getlimiti().top + limiti.getlimiti().height - Lista[i].raggio) {
        Lista[i].vel.y = -Lista[i].vel.y;
      }
    }
  };

  void check_external_bounds(Persona& test);

  void change_status();  // Fa cambiare la texture nel momento in cui le due particelle si scontrano

  void change_vel();

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
    check_collisions();
    check_borders();
    aggiornagriglia();
  }

  void Disegna() {
    a_window.Pulisci();
    
    /* a_window.Disegna(rappresentazione); */
    
    a_window.Disegna(limiti);

    a_window.Disegna(uomini);

    a_window.Mostra();
  }
};

#endif