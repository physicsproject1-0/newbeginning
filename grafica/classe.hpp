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
enum class Stato { VULNERABILE, INFETTO, RIMOSSO };

struct Persona {
  sf::Vector2f centro;
  float raggio;
  sf::Vector2f vel;
  sf::Clock cambiovelocita;
  Stato P;
  bool checked = false;
};

class Animazione : public sf::Drawable {
  sf::Clock orologio2;
  sf::Texture ominoprova;
  sf::VertexArray struttura;
  std::map<int, Persona> popolazione;  // vedere se meglio vector

  Bordi limiti;

  Stato P;

  // Nel momento in cui collidono due persone, se una era infetta, cambia lo stato anche dell' altra
  void collisione() {
    for (int i = 0; i < popolazione.size(); i++) {
      Persona& PallinaA = popolazione[i];
      if (PallinaA.P == Stato::VULNERABILE) {
        for (int j = 0; j < popolazione.size(); j++) {
          Persona& PallinaB = popolazione[j];

          if ((i != j) && (PallinaB.P == Stato::INFETTO)) {
            if (modulo(PallinaA.centro - PallinaB.centro) <= 1.5 * PallinaB.raggio) {
              PallinaA.P = Stato::INFETTO;
              setredTextures();
            }
          } else {
            continue;
          }
        }
      } else if (PallinaA.P == Stato::INFETTO) {  // Se una e' gia' infetta dopo tot secondi diventa rimossa
        if (orologio2.getElapsedTime().asSeconds() > 8) {
          popolazione[i].P = Stato::RIMOSSO;
          setwhiteTextures();
          orologio2.restart();
        }
      } else {
        continue;
      }
    }
  }

  // Funzione con cui carico la Texture verde
  void setgreenTextures() {
    for (int i = 0; i < popolazione.size(); i++) {
      sf::Vertex* iter = &struttura[i * 3];

      iter[0].texCoords = sf::Vector2f(110.f, 20.f);  // coordinate pupini verdi
      iter[1].texCoords = sf::Vector2f(20.f, 210.f);
      iter[2].texCoords = sf::Vector2f(205.f, 210.f);
    }
  }

  // Funzione in cui carico la Texture rossa sullo stato INFETTO
  void setredTextures() {
    for (int i = 0; i < popolazione.size(); i++) {
      if (popolazione[i].P == Stato::INFETTO) {
        sf::Vertex* iter = &struttura[i * 3];

        iter[0].texCoords = sf::Vector2f(315.f, 20.f);  // coordinate pupini rossi
        iter[1].texCoords = sf::Vector2f(230.f, 210.f);
        iter[2].texCoords = sf::Vector2f(410.f, 210.f);

      } else {
        continue;
      }
    }
  }

  // Ho immaginato che al 40% muoiano e al 60% guariscono, si possono cambiare le probabilita' of course
  // Funzione in cui carico sullo stato RIMOSSO al 40% la texture grigia e al 60% quella azzurra
  void setwhiteTextures() {
    for (int i = 0; i < popolazione.size(); i++) {
      if (popolazione[i].P == Stato::RIMOSSO) {
        sf::Vertex* iter = &struttura[i * 3];
        srand(time(NULL));
        int a = rand() % 100 + 1;
        if (a < 40) {
          iter[0].texCoords = sf::Vector2f(520.f, 20.f);  // coordinate pupini grigi
          iter[1].texCoords = sf::Vector2f(430.f, 210.f);
          iter[2].texCoords = sf::Vector2f(615.f, 210.f);
        } else {
          iter[0].texCoords = sf::Vector2f(730.f, 20.f);  // coordinate pupini azzurri
          iter[1].texCoords = sf::Vector2f(635.f, 210.f);
          iter[2].texCoords = sf::Vector2f(825.f, 210.f);
        }
      } else {
        continue;
      }
    }
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &ominoprova;
    // capire che madonna succede qui dentro!!!
    target.draw(struttura, states);
    target.draw(limiti);
  }

 public:
  Animazione(int n) : limiti(sf::Vector2f(600, 400), sf::Vector2f(100, 100)) {
    if (!ominoprova.loadFromFile("uomini.png")) {
      throw std::runtime_error{"texture loading failed"};  // catcharlo
    }

    Persona prova;

    srand(time(NULL));

    // Riempio la mappa (popolazione) di persone
    for (int i = 0; i < n; i++) {
      prova.raggio = 13.f;
      prova.centro = sf::Vector2f(rand() % static_cast<int>(limiti.getlimiti().width - 2 * prova.raggio) + limiti.getlimiti().left + prova.raggio,
                                  rand() % static_cast<int>(limiti.getlimiti().height - 2 * prova.raggio) + limiti.getlimiti().top + prova.raggio);
      prova.vel = sf::Vector2f(rand() % 50 - 25.f, rand() % 50 - 25.f);
      popolazione[i] = prova;
      popolazione[i].P = Stato::VULNERABILE;
    }

    popolazione[rand() % popolazione.size() + 1].P = Stato::INFETTO;

    struttura.resize(popolazione.size() * 3);

    struttura.setPrimitiveType(sf::Triangles);

    setgreenTextures();
    setredTextures();
  }

  void aggiorna_griglia() {
    for (int i = 0; i < popolazione.size(); i++) {
      sf::Vertex* iter = &struttura[i * 3];
      iter[0].position = sf::Vector2f(popolazione[i].centro.x, popolazione[i].centro.y - popolazione[i].raggio);  // strane coord
      iter[1].position = sf::Vector2f(popolazione[i].centro.x - popolazione[i].raggio * (1.7f / 2),
                                      popolazione[i].centro.y + (popolazione[i].raggio / 2));
      iter[2].position = sf::Vector2f(popolazione[i].centro.x + popolazione[i].raggio * (1.7f / 2),
                                      popolazione[i].centro.y + (popolazione[i].raggio / 2));
      // bisognerà cancellare il puntatore?
    }
  }

  // void check_collisions();  // devo passare un puntatore all'orologio per averne solo uno  //pensarci

  void aggiorna_lista() {
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

  void change_Stato();  // Fa cambiare la texture nel momento in cui le due particelle si scontrano

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

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

class Cellula : public sf::Drawable {  // se è una struct non funziona l'inheritance?
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(rettangolo);
    target.draw(numero);
  }  // metterci anche states altrimenti rompe il casso
     // non posso loadare un font qui dentro direttamente

 public:
  // wtf

  void aggiorna_colore() {
    switch (S) {
      case (Stato::INFETTO):  // Carichiamo la red texture...
        rettangolo.setFillColor(sf::Color::Red);
        std::cout << "sto aggiornando il rosso" << '\n';

        break;

      case (Stato::RIMOSSO):  // carichiamo la white texture
        rettangolo.setFillColor(sf::Color::Cyan);
        break;

      case (Stato::VULNERABILE):  // carichiamo la green texture

        rettangolo.setFillColor(sf::Color::Green);
        break;
    }
  }

  int counter;

  int infection_days;

  Stato S = Stato::VULNERABILE;
  sf::RectangleShape rettangolo;

  sf::Text numero;

  Cellula(sf::Vector2f posizione, sf::Vector2f dimensione) : counter(0), infection_days(0) {  // funzionerà
    rettangolo.setPosition(posizione);
    rettangolo.setSize(dimensione);
    rettangolo.setOutlineColor(sf::Color::White);
    rettangolo.setOutlineThickness(2.f);
    aggiorna_colore();

    /* numero.setFont(*p_font); */
    numero.setPosition(posizione);
    numero.setFillColor(sf::Color::White);
    numero.setCharacterSize(50);
  }
};

class Automa : public sf::Drawable {  // ESTRARRE LE CLASSI NESTATE E DISTINGUERE I MORTI DAI GUARITI
  /* sf::Font font; */

  // mettere errori per dimensioni minori di 0?
  std::vector<std::vector<Cellula>> grid;

  sf::Vector2f dimensioni;
  sf::Vector2f posizione;

  sf::Clock orologio;

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

    /* if (!font.loadFromFile("Arial.ttf")) {
      throw std::runtime_error{"texture loading failed"};
    } */
    /*  try {
       if (!font.loadFromFile("Arial.ttf")) {
         throw std::runtime_error{"denominator is zero"};
       }
     } catch (std::runtime_error const& e) {
       std::cerr << e.what() << '\n';  //
     } */

    float t_lunghezza_x = dimensioni.x / numero_lato;

    float t_lunghezza_y = dimensioni.y / numero_lato;

    for (int i = 0; i < numero_lato; i++) {
      std::vector<Cellula> riga;
      for (int j = 0; j < numero_lato; j++) {
        sf::Vector2f posizionemovente(posizione.x + j * t_lunghezza_x, posizione.y + i * t_lunghezza_y);
        Cellula riempi(posizionemovente, sf::Vector2f(t_lunghezza_x, t_lunghezza_y) /* , &font */);

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
      if (grid[riga][colonna].S != Stato::VULNERABILE) {
        a--;
        continue;
      }
      grid[riga][colonna].S = Stato::INFETTO;
      grid[riga][colonna].aggiorna_colore();
    }

    for (int a = 0; a < rimosse; a++) {
      int riga = rand() % numero_lato;
      int colonna = rand() % numero_lato;
      if (grid[riga][colonna].S != Stato::VULNERABILE) {
        a--;
        continue;
      }
      grid[riga][colonna].S = Stato::RIMOSSO;
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
    std::cout << "controllo la cellula in posizione"
              << " riga " << i << " colonna " << j << '\n';
    for (int a = 0; a <= 2; a++) {
      if (esiste(i - 1, j - 1 + a)) {
        std::cout << "esiste "
                  << "in posizione"
                  << " riga " << i - 1 << " colonna " << j - 1 + a << '\n';
        if (grid[i - 1][j - 1 + a].S == Stato::INFETTO) {
          std::cout << "c'è infetto" << '\n';
          cell.counter++;
        }
      }
      if (esiste(i + 1, j - 1 + a)) {
        std::cout << "esiste "
                  << "in posizione"
                  << " riga " << i + 1 << " colonna " << j - 1 + a << '\n';

        if (grid[i + 1][j - 1 + a].S == Stato::INFETTO) {
          std::cout << "c'è infetto" << '\n';
          cell.counter++;
        }
      }
    }
    if (esiste(i, j - 1)) {
      std::cout << "esiste "
                << "in posizione"
                << " riga " << i << " colonna " << j - 1 << '\n';

      if (grid[i][j - 1].S == Stato::INFETTO) {
        std::cout << "c'è infetto" << '\n';

        cell.counter++;
      }
    }
    if (esiste(i, j + 1)) {
      std::cout << "esiste "
                << "in posizione"
                << " riga " << i << " colonna " << j + 1 << '\n';

      if (grid[i][j + 1].S == Stato::INFETTO) {
        std::cout << "c'è infetto" << '\n';
        cell.counter++;
      }
    }
  }

  void aggiorna() {
    giorni++;
    for (int i = 0; i < numero_lato; i++) {
      for (int j = 0; j < numero_lato; j++) {
        Cellula& cell = grid[i][j];
        std::cout << "riga " << i << " colonna " << j << '\n';
        if (cell.S == Stato::VULNERABILE) {
          std::cout << "sono vulnerabile" << '\n';
          int esponente = cell.counter;
          // cell.numero.setString(std::to_string(esponente));
          if (esponente == 0) {
            continue;
          } else {
            float prob_sano = pow(1 - probabilita_contagio, esponente);  // beta o gamma?
            std::cout << "prob sano" << prob_sano << '\n';
            float estrazione = (rand() % 101) / 100.f;  // IL .F è FONDAMENTALE
            std::cout << "estrazione " << estrazione << '\n';
            if (estrazione > prob_sano) {
              // aggiungere i seed randomici

              cell.S = Stato::INFETTO;  // PORCO DIO AVEVO MESSO DUE UGUALI
              std::cout << "ora sono infetto" << '\n';
              cell.aggiorna_colore();
            }
          }
        }

        else if (cell.S == Stato::INFETTO) {
          std::cout << "sono arrivato qua" << '\n';
          cell.infection_days++;
          if ((rand() % 100) / 100.f < probabilita_guarigione) {
            cell.S = Stato::RIMOSSO;
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
  GUI overlay;

  Finestra a_window;

  Animazione dinamica;  // fare altra classe che contiene sia griglia sia bordi, gestire tutto lì

  Automa statica;
  // non ho capito perhcè qui dentro non ci posso mettere il costruttore;

  sf::Clock timer;
  sf::Time trascorso;

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

    a_window.Disegna(overlay);

    a_window.Mostra();
  }
};

#endif