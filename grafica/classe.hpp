#include <assert.h>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "finestra.hpp"

#ifndef DISA
#define DISA

enum class StatoPupino { VULNERABILE, INFETTO, RIMOSSO };

// per disegnare altre cose oltre il vertex array
/* class Rappresentazione : public sf::Drawable { */
class Animazione : public sf::Drawable {
  struct Persona {
    sf::Vector2f centro;
    float raggio;
    sf::Vector2f vel;
    sf::Clock cambiovelocita;
    StatoPupino P;
    bool checked = false;
  };

  class Bordi : public sf::Drawable {
    sf::RectangleShape rettangolo;
    sf::FloatRect bordo_collisioni;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
      target.draw(rettangolo);
    }  // metterci anche states altrimenti rompe il casso

   public:
    Bordi(sf::Vector2f dimensione) {
      rettangolo.setSize(dimensione);
      rettangolo.setFillColor(sf::Color::Transparent);
      rettangolo.setOutlineColor(sf::Color::White);
      rettangolo.setOutlineThickness(3);
      rettangolo.setPosition(100, 100);

      bordo_collisioni.left = 100;
      bordo_collisioni.top = 100;
      bordo_collisioni.width = dimensione.x;
      bordo_collisioni.height = dimensione.y;
    }

    sf::FloatRect getlimiti() { return bordo_collisioni; };
  };

  sf::Clock* orologio;

  sf::Clock orologio2;
  sf::Texture ominoprova;
  sf::VertexArray struttura;
  std::map<int, Persona> popolazione;  // vedere se meglio vector

  Bordi limiti;

  StatoPupino P;

  void collisione() {
    for (int i = 0; i < popolazione.size(); i++) {
      Persona& PallinaA = popolazione[i];
      if (PallinaA.P == StatoPupino::VULNERABILE) {
        for (int j = 0; j < popolazione.size(); j++) {
          Persona& PallinaB = popolazione[j];

          if ((i != j) && (PallinaB.P == StatoPupino::INFETTO)) {
            if (modulo(PallinaA.centro - PallinaB.centro) <= 1.5 * PallinaB.raggio) {
              PallinaA.P = StatoPupino::INFETTO;
              aggiorna_texture();
            }
          } else {
            break;
          }
        }
      } else {
        break;
      }
    }
  }
  void aggiorna_texture() {
    for (int i = 0; i < popolazione.size(); i++) {
      sf::Vertex* iter = &struttura[i * 3];

      switch (P) {
        case (StatoPupino::INFETTO):  // Carichiamo la red texture...
          iter[0].color = sf::Color::Red;
          iter[1].color = sf::Color::Red;
          iter[2].color = sf::Color::Red;

        case (StatoPupino::RIMOSSO):  // carichiamo la white texture
          iter[0].color = sf::Color::White;
          iter[1].color = sf::Color::White;
          iter[2].color = sf::Color::White;
          break;

        case (StatoPupino::VULNERABILE):  // carichiamo la green texture
          iter[0].color = sf::Color::Green;
          iter[1].color = sf::Color::Green;
          iter[2].color = sf::Color::Green;
          break;
      }
    }
  }

  void settexturecoords() {
    for (int i = 0; i < popolazione.size(); i++) {
      sf::Vertex* iter = &struttura[i * 3];

      iter[0].texCoords = sf::Vector2f(430.f, 0.f);  // strane coord
      iter[1].texCoords = sf::Vector2f(0.f, 1681.f);
      iter[2].texCoords = sf::Vector2f(860.f, 1681.f);
    }
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &ominoprova;
    // capire che madonna succede qui dentro!!!
    target.draw(struttura, states);
    target.draw(limiti);
  }

 public:
  Animazione(int n, sf::Clock* t_orologio) : limiti(sf::Vector2f(600, 400)), orologio{t_orologio} {
    if (!ominoprova.loadFromFile("uomoverde.png")) {
      throw std::runtime_error{"texture loading failed"};  // catcharlo
    }

    Persona prova;

    for (int i = 0; i < n; i++) {
      prova.raggio = 10.f;
      prova.centro = sf::Vector2f(rand() % static_cast<int>(limiti.getlimiti().width - 2 * prova.raggio) + limiti.getlimiti().left + prova.raggio,
                                  rand() % static_cast<int>(limiti.getlimiti().height - 2 * prova.raggio) + limiti.getlimiti().top + prova.raggio);
      prova.vel = sf::Vector2f(rand() % 50 - 25.f, rand() % 50 - 25.f);
      popolazione[i] = prova;
    }
    struttura.resize(popolazione.size() * 3);

    struttura.setPrimitiveType(sf::Triangles);

    settexturecoords();
  }

  void aggiorna_griglia() {
    for (int i = 0; i < popolazione.size(); i++) {
      sf::Vertex* iter = &struttura[i * 3];
      iter[0].position = sf::Vector2f(popolazione.at(i).centro.x, popolazione.at(i).centro.y - popolazione.at(i).raggio);  // strane coord
      iter[1].position = sf::Vector2f(popolazione.at(i).centro.x - popolazione.at(i).raggio * (1.7f / 2),
                                      popolazione.at(i).centro.y + (popolazione.at(i).raggio / 2));
      iter[2].position = sf::Vector2f(popolazione.at(i).centro.x + popolazione.at(i).raggio * (1.7f / 2),
                                      popolazione.at(i).centro.y + (popolazione.at(i).raggio / 2));
      // bisognerà cancellare il puntatore?

      // non posso usare [] perchè mi dice che non funziona per le robe const
    }
  }

  // void check_collisions();  // devo passare un puntatore all'orologio per averne solo uno  //pensarci

  void aggiorna_lista() {
    float deltat = /* orologio2.restart().asSeconds(); */ orologio->restart().asSeconds();
    // metto qua perchè se lo chiamo in punti diversi magari sono leggemente diversi
    float deltat2 = orologio2.restart().asSeconds();

    // std::cout << "tempo1 " << deltat << '\n';
    // std::cout << "tempo2 " << deltat2 << '\n';
    for (int i = 0; i < popolazione.size(); i++) {
      popolazione[i].centro += popolazione[i].vel * deltat2;
    }
  }

  void check_borders() {
    for (int i = 0; i < popolazione.size(); i++) {
      if (popolazione[i].centro.x < limiti.getlimiti().left + popolazione[i].raggio ||
          popolazione[i].centro.x > limiti.getlimiti().left + limiti.getlimiti().width - popolazione[i].raggio) {
        popolazione[i].vel.x = -popolazione[i].vel.x;
      }
      if (popolazione[i].centro.y < limiti.getlimiti().top + popolazione[i].raggio ||
          popolazione[i].centro.y > limiti.getlimiti().top + limiti.getlimiti().height - popolazione[i].raggio) {
        popolazione[i].vel.y = -popolazione[i].vel.y;
      }
    }
  };

  void check_external_bounds(Persona& test);

  void change_status();  // Fa cambiare la texture nel momento in cui le due particelle si scontrano

  void change_vel();

  int check_occur(Persona const& persona, int raggio);

  double modulo(sf::Vector2f const& vettore);

  void aggiorna_generale() {
    check_borders();

    collisione();

    aggiorna_lista();

    aggiorna_griglia();
  }
};

/* }; */

enum class Status { VULNERABLE, INFECTED, REMOVED };

class Automa : public sf::Drawable {
  class Cellula : public sf::Drawable {  // se è una struct non funziona l'inheritance?
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
      target.draw(rettangolo);
      target.draw(numero);
    }  // metterci anche states altrimenti rompe il casso

   public:
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

    int counter;

    int infection_days;

    Status S = Status::VULNERABLE;
    sf::RectangleShape rettangolo;

    sf::Text numero;

    Cellula(sf::Vector2f posizione, sf::Vector2f dimensione) : counter(0), infection_days(0) {  // funzionerà
      rettangolo.setPosition(posizione);
      rettangolo.setSize(dimensione);
      rettangolo.setOutlineColor(sf::Color::White);
      rettangolo.setOutlineThickness(2.f);
      aggiorna_colore();

      numero.setPosition(posizione);
      numero.setColor(sf::Color::White);
    }
  };
  // mettere errori per dimensioni minori di 0?
  std::vector<std::vector<Cellula>> grid;

  sf::Vector2f dimensioni;
  sf::Vector2f posizione;

  sf::Clock orologio;

  sf::Font font;

  int numero_lato;

  float probabilita_contagio;

  float probabilita_guarigione;

  int giorni = 0;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int i = 0; i < numero_lato; i++) {
      for (int j = 0; j < numero_lato; j++) {
        target.draw(grid[i][j]);
      }
    }
  }

 public:
  Automa(sf::Vector2f t_posizione, sf::Vector2f t_dimensione, int t_numero, float t_probabilita_contagio, float t_probabilita_guarigione, int infetti,
         int rimossi)
      : posizione{t_posizione},
        dimensioni{t_dimensione},
        numero_lato{t_numero},
        probabilita_contagio{t_probabilita_contagio},
        probabilita_guarigione{t_probabilita_guarigione} {
    assert(probabilita_contagio <= 1 && probabilita_contagio >= 0);  // mettere except

    if (!font.loadFromFile("Arial.ttf")) {
      // error...
    }

    float t_lunghezza_x = dimensioni.x / numero_lato;

    float t_lunghezza_y = dimensioni.y / numero_lato;

    for (int i = 0; i < numero_lato; i++) {
      std::vector<Cellula> riga;
      for (int j = 0; j < numero_lato; j++) {
        sf::Vector2f posizionemovente(posizione.x + j * t_lunghezza_x, posizione.y + i * t_lunghezza_y);
        Cellula riempi(posizionemovente, sf::Vector2f(t_lunghezza_x, t_lunghezza_y));

        riga.push_back(riempi);

        // i per le righe, j per le colonne
      }
      grid.push_back(riga);
    }
    genera(infetti, rimossi);
  }

  void genera(int infette, int rimosse) {
    assert(infette + rimosse < numero_lato * numero_lato);

    for (int a = 0; a < infette; a++) {
      int riga = rand() % numero_lato;
      int colonna = rand() % numero_lato;
      if (grid[riga][colonna].S != Status::VULNERABLE) {
        a--;
        continue;
      }
      grid[riga][colonna].S = Status::INFECTED;
      grid[riga][colonna].aggiorna_colore();
    }

    for (int a = 0; a < rimosse; a++) {
      int riga = rand() % numero_lato;
      int colonna = rand() % numero_lato;
      if (grid[riga][colonna].S != Status::VULNERABLE) {
        a--;
        continue;
      }
      grid[riga][colonna].S = Status::REMOVED;
      grid[riga][colonna].aggiorna_colore();
    }
  }

  bool esiste(int i, int j) {
    if (i < 0 || i >= numero_lato || j < 0 || j >= numero_lato) {
      return false;
    }
    return true;
  }

  void aggiorna_counter(int i, int j) {
    Cellula& cell = grid[i][j];
    /* if (i == 0 || j == 0 || i == (numero_lato - 1) ||) {
    }
 */
    for (int a = 0; a < 2; a++) {
      if (esiste(i - 1, j - 1 + a)) {
        if (grid[i - 1][j - 1 + a].S == Status::INFECTED) {
          cell.counter++;
        }
      }
      if (esiste(i + 1, j - 1 + a)) {
        if (grid[i + 1][j - 1 + a].S == Status::INFECTED) {
          cell.counter++;
        }
      }
    }
    if (esiste(i, j - 1)) {
      if (grid[i][j - 1].S == Status::INFECTED) {
        cell.counter++;
      }
    }
    if (esiste(i, j + 1)) {
      if (grid[i][j + 1].S == Status::INFECTED) {
        cell.counter++;
      }
    }
  }

  void aggiorna() {
    giorni++;
    for (int i = 0; i < numero_lato; i++) {
      for (int j = 0; j < numero_lato; j++) {
        Cellula& cell = grid[i][j];

        if (cell.S == Status::VULNERABLE) {
          int esponente = cell.counter;
          cell.numero.setString(std::to_string(esponente));
          if (esponente == 0) {
            continue;
          } else {
            float prob_sano = pow(1 - probabilita_contagio, esponente);  // beta o gamma?

            if ((rand() % 101) / 100 > prob_sano) {
              // aggiungere i seed randomici
              cell.S == Status::INFECTED;
              cell.aggiorna_colore();
            }
          }
        }

        if (cell.S == Status::INFECTED) {
          cell.infection_days++;
          if ((rand() % 100) / 100 < probabilita_guarigione) {
            cell.S = Status::REMOVED;
            cell.aggiorna_colore();
            // qua forse ci sta fare così
          }
        }

        cell.counter = 0;
      }
    }
  }

  void avanza() {
    if (orologio.getElapsedTime().asSeconds() > 3) {
      for (int i = 0; i < numero_lato; i++) {
        for (int j = 0; j < numero_lato; j++) {
          aggiorna_counter(i, j);
        }
      }
      aggiorna();
      orologio.restart();
    }
  }
};

class Mondo /* : public sf::Drawable  */ {
 private:  // la draw non va nel protected??
  /* sf::VertexArray Griglia; */
  Finestra a_window;

  Animazione dinamica;  // fare altra classe che contiene sia griglia sia bordi, gestire tutto lì

  // non ho capito perhcè qui dentro non ci posso mettere il costruttore;

  sf::Clock timer;
  sf::Time trascorso;

  Automa statica;
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
  Mondo();

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
    dinamica.aggiorna_generale();
    statica.avanza();
  }

  void Disegna() {
    a_window.Pulisci();

    a_window.Disegna(dinamica);

    a_window.Disegna(statica);

    a_window.Mostra();
  }
};

#endif