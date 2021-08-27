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
  sf::Vector2f m_centro;
  float m_raggio;
  sf::Vector2f m_vel;
  sf::Clock m_cambiom_velocita;
  Stato m_P;
  bool checked = false;
  int m_m_numero_contatti = 0;
};

class Animazione : public sf::Drawable {
  sf::Clock m_m_orologio2;
  sf::Texture m_ominoprova;
  sf::VertexArray m_struttura;
  std::map<int, Persona> m_popolazione;  // vedere se meglio vector

  Bordi m_limiti;

  Stato m_P;

  // Nel momento in cui collidono due persone, se una era infetta, cambia lo stato anche dell' altra
  void Collisione() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      Persona& PallinaA = m_popolazione[i];
      if (PallinaA.m_P == Stato::VULNERABILE) {
        for (int j = 0; j < m_popolazione.size(); j++) {
          Persona& PallinaB = m_popolazione[j];

          if ((i != j) && (PallinaB.m_P == Stato::INFETTO)) {
            if (Modulo(PallinaA.m_centro - PallinaB.m_centro) <= 1.5 * PallinaB.m_raggio) {
              PallinaA.m_P = Stato::INFETTO;
              SetredTextures();
            }
          } else {
            continue;
          }
        }
      } else {
        continue;
      }
    }
  }
  /*
    void Conteggio_contatti() {
      for (int i = 0; i < m_popolazione.size(); i++) {
        Persona& PallinaA = m_popolazione[i];
        for (int j = 0; j < m_popolazione.size(); j++) {
          Persona& PallinaB = m_popolazione[j];
          if ((i != j) && (Modulo(PallinaA.m_centro - PallinaB.m_centro) <= 1.5 * PallinaB.m_raggio)) {
            PallinaA.m_m_numero_contatti++;
          } else {
            continue;
          }
        }
      }
    }

    void Morte_persona() {
      for (int i = 0; i < m_popolazione.size(); i++) {
        Persona& PallinaA = m_popolazione[i];
        if ((PallinaA.m_P == Stato::INFETTO) && (PallinaA.m_m_numero_contatti >= 80)) {
          PallinaA.m_P = Stato::RIMOSSO;
          SetwhiteTextures();
        } else {
          continue;
        }
      }
    }
  */
  // Funzione con cui carico la Texture verde
  void SetgreenTextures() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      sf::Vertex* iter = &m_struttura[i * 3];

      iter[0].texCoords = sf::Vector2f(110.f, 20.f);  // coordinate pupini verdi
      iter[1].texCoords = sf::Vector2f(20.f, 210.f);
      iter[2].texCoords = sf::Vector2f(205.f, 210.f);
    }
  }

  // Funzione in cui carico la Texture rossa sullo stato INFETTO
  void SetredTextures() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      if (m_popolazione[i].m_P == Stato::INFETTO) {
        sf::Vertex* iter = &m_struttura[i * 3];

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
  void SetwhiteTextures() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      Persona& PallinaA = m_popolazione[i];
      if (PallinaA.m_P == Stato::RIMOSSO) {
        srand(time(NULL));
        int a = rand() % 100 + 1;
        if (a < 40) {
          sf::Vertex* iter = &m_struttura[i * 3];
          iter[0].texCoords = sf::Vector2f(520.f, 20.f);  // coordinate pupini grigi
          iter[1].texCoords = sf::Vector2f(430.f, 210.f);
          iter[2].texCoords = sf::Vector2f(615.f, 210.f);
        } else {
          sf::Vertex* iter = &m_struttura[i * 3];
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
    states.texture = &m_ominoprova;
    // capire che madonna succede qui dentro!!!
    target.draw(m_struttura, states);
    target.draw(m_limiti);
  }

 public:
  Animazione(int n) : m_limiti(sf::Vector2f(600, 400), sf::Vector2f(100, 100)) {
    if (!m_ominoprova.loadFromFile("uomini.png")) {
      throw std::runtime_error{"texture loading failed"};  // catcharlo
    }

    Persona m_prova;

    srand(time(NULL));

    // Riempio la mappa (m_popolazione) di persone
    for (int i = 0; i < n; i++) {
      m_prova.m_raggio = 13.f;
      m_prova.m_centro =
          sf::Vector2f(rand() % static_cast<int>(m_limiti.getlimiti().width - 2 * m_prova.m_raggio) + m_limiti.getlimiti().left + m_prova.m_raggio,
                       rand() % static_cast<int>(m_limiti.getlimiti().height - 2 * m_prova.m_raggio) + m_limiti.getlimiti().top + m_prova.m_raggio);
      m_prova.m_vel = sf::Vector2f(rand() % 50 - 25.f, rand() % 50 - 25.f);
      m_popolazione[i] = m_prova;
      m_popolazione[i].m_P = Stato::VULNERABILE;
    }

    m_popolazione[rand() % m_popolazione.size() + 1].m_P = Stato::INFETTO;

    m_struttura.resize(m_popolazione.size() * 3);

    m_struttura.setPrimitiveType(sf::Triangles);

    SetgreenTextures();
    SetredTextures();
  }

  void Aggiorna_griglia() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      sf::Vertex* iter = &m_struttura[i * 3];
      iter[0].position = sf::Vector2f(m_popolazione[i].m_centro.x, m_popolazione[i].m_centro.y - m_popolazione[i].m_raggio);  // strane coord
      iter[1].position = sf::Vector2f(m_popolazione[i].m_centro.x - m_popolazione[i].m_raggio * (1.7f / 2),
                                      m_popolazione[i].m_centro.y + (m_popolazione[i].m_raggio / 2));
      iter[2].position = sf::Vector2f(m_popolazione[i].m_centro.x + m_popolazione[i].m_raggio * (1.7f / 2),
                                      m_popolazione[i].m_centro.y + (m_popolazione[i].m_raggio / 2));
      // bisognerà cancellare il puntatore?
    }
  }

  // void check_collisions();  // devo passare un puntatore all'm_orologio per averne solo uno  //pensarci

  void Aggiorna_lista() {
    // metto qua perchè se lo chiamo in punti diversi magari sono leggemente diversi
    float deltat2 = m_m_orologio2.restart().asSeconds();

    // std::cout << "tempo1 " << deltat << '\n';
    // std::cout << "tempo2 " << deltat2 << '\n';
    for (int i = 0; i < m_popolazione.size(); i++) {
      m_popolazione[i].m_centro += m_popolazione[i].m_vel * deltat2;
    }
  }

  void Check_borders() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      if (m_popolazione[i].m_centro.x < m_limiti.getlimiti().left + m_popolazione[i].m_raggio ||
          m_popolazione[i].m_centro.x > m_limiti.getlimiti().left + m_limiti.getlimiti().width - m_popolazione[i].m_raggio) {
        m_popolazione[i].m_vel.x = -m_popolazione[i].m_vel.x;
      }
      if (m_popolazione[i].m_centro.y < m_limiti.getlimiti().top + m_popolazione[i].m_raggio ||
          m_popolazione[i].m_centro.y > m_limiti.getlimiti().top + m_limiti.getlimiti().height - m_popolazione[i].m_raggio) {
        m_popolazione[i].m_vel.y = -m_popolazione[i].m_vel.y;
      }
    }
  };

  void Check_external_bounds(Persona& test);

  // void change_Stato();  // Fa cambiare la texture nel momento in cui le due particelle si scontrano

  // void change_m_vel();

  int Check_occur(Persona const& persona, int m_raggio);

  double Modulo(sf::Vector2f const& vettore);

  void Aggiorna_Generale() {
    Check_borders();

    // Conteggio_contatti();

    Collisione();

    // Morte_persona();

    Aggiorna_lista();

    Aggiorna_griglia();
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
    target.draw(m_rettangolo);
    target.draw(m_numero);
  }  // metterci anche states altrimenti rompe il casso
     // non posso loadare un font qui dentro direttamente

 public:
  // wtf

  void Aggiorna_colore() {
    switch (S) {
      case (Stato::INFETTO):  // Carichiamo la red texture...
        m_rettangolo.setFillColor(sf::Color::Red);
        std::cout << "sto Aggiornando il rosso" << '\n';

        break;

      case (Stato::RIMOSSO):  // carichiamo la white texture
        m_rettangolo.setFillColor(sf::Color::Cyan);
        break;

      case (Stato::VULNERABILE):  // carichiamo la green texture

        m_rettangolo.setFillColor(sf::Color::Green);
        break;
    }
  }

  int m_counter;

  int m_infection_days;

  Stato S = Stato::VULNERABILE;
  sf::RectangleShape m_rettangolo;

  sf::Text m_numero;

  Cellula(sf::Vector2f m_posizione, sf::Vector2f dimensione) : m_counter(0), m_infection_days(0) {  // funzionerà
    m_rettangolo.setPosition(m_posizione);
    m_rettangolo.setSize(dimensione);
    m_rettangolo.setOutlineColor(sf::Color::White);
    m_rettangolo.setOutlineThickness(2.f);
    Aggiorna_colore();

    /* m_numero.setFont(*p_font); */
    m_numero.setPosition(m_posizione);
    m_numero.setFillColor(sf::Color::White);
    m_numero.setCharacterSize(50);
  }
};

class Automa : public sf::Drawable {  // ESTRARRE LE CLASSI NESTATE E DISTINGUERE I MORTI DAI GUARITI
  /* sf::Font font; */

  // mettere errori per m_dimensioni minori di 0?
  std::vector<std::vector<Cellula>> m_grid;

  sf::Vector2f m_dimensioni;
  sf::Vector2f m_posizione;

  sf::Clock m_orologio;

  int m_numero_lato;

  float m_probabilita_contagio;

  float m_probabilita_guarigione;

  int m_giorni = 0;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int i = 0; i < m_numero_lato; i++) {
      for (int j = 0; j < m_numero_lato; j++) {
        target.draw(m_grid[i][j]);
      }
    }
  }

 public:
  Automa(sf::Vector2f t_m_posizione, sf::Vector2f t_dimensione, int t_m_numero, float t_m_probabilita_contagio, float t_m_probabilita_guarigione,
         int infetti, int rimossi)
      : m_posizione{t_m_posizione},
        m_dimensioni{t_dimensione},
        m_numero_lato{t_m_numero},
        m_probabilita_contagio{t_m_probabilita_contagio},
        m_probabilita_guarigione{t_m_probabilita_guarigione} {
    assert(m_probabilita_contagio <= 1 && m_probabilita_contagio >= 0);  // mettere except

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

    float t_lunghezza_x = m_dimensioni.x / m_numero_lato;

    float t_lunghezza_y = m_dimensioni.y / m_numero_lato;

    for (int i = 0; i < m_numero_lato; i++) {
      std::vector<Cellula> riga;
      for (int j = 0; j < m_numero_lato; j++) {
        sf::Vector2f m_posizionemovente(m_posizione.x + j * t_lunghezza_x, m_posizione.y + i * t_lunghezza_y);
        Cellula riempi(m_posizionemovente, sf::Vector2f(t_lunghezza_x, t_lunghezza_y) /* , &font */);

        riga.push_back(riempi);

        // i per le righe, j per le colonne
      }
      m_grid.push_back(riga);
    }
    Genera(infetti, rimossi);
  }

  void Genera(int infette, int rimosse) {
    assert(infette + rimosse < m_numero_lato * m_numero_lato);

    for (int a = 0; a < infette; a++) {
      int riga = rand() % m_numero_lato;
      int colonna = rand() % m_numero_lato;
      if (m_grid[riga][colonna].S != Stato::VULNERABILE) {
        a--;
        continue;
      }
      m_grid[riga][colonna].S = Stato::INFETTO;
      m_grid[riga][colonna].Aggiorna_colore();
    }

    for (int a = 0; a < rimosse; a++) {
      int riga = rand() % m_numero_lato;
      int colonna = rand() % m_numero_lato;
      if (m_grid[riga][colonna].S != Stato::VULNERABILE) {
        a--;
        continue;
      }
      m_grid[riga][colonna].S = Stato::RIMOSSO;
      m_grid[riga][colonna].Aggiorna_colore();
    }
  }

  bool Esiste(int i, int j) {
    if (i < 0 || i >= m_numero_lato || j < 0 || j >= m_numero_lato) {
      return false;
    }
    return true;
  }

  void Aggiorna_counter(int i, int j) {
    Cellula& cell = m_grid[i][j];
    /* if (i == 0 || j == 0 || i == (m_numero_lato - 1) ||) {
    }
 */
    std::cout << "controllo la cellula in m_posizione"
              << " riga " << i << " colonna " << j << '\n';
    for (int a = 0; a <= 2; a++) {
      if (Esiste(i - 1, j - 1 + a)) {
        std::cout << "Esiste "
                  << "in m_posizione"
                  << " riga " << i - 1 << " colonna " << j - 1 + a << '\n';
        if (m_grid[i - 1][j - 1 + a].S == Stato::INFETTO) {
          std::cout << "c'è infetto" << '\n';
          cell.m_counter++;
        }
      }
      if (Esiste(i + 1, j - 1 + a)) {
        std::cout << "Esiste "
                  << "in m_posizione"
                  << " riga " << i + 1 << " colonna " << j - 1 + a << '\n';

        if (m_grid[i + 1][j - 1 + a].S == Stato::INFETTO) {
          std::cout << "c'è infetto" << '\n';
          cell.m_counter++;
        }
      }
    }
    if (Esiste(i, j - 1)) {
      std::cout << "Esiste "
                << "in m_posizione"
                << " riga " << i << " colonna " << j - 1 << '\n';

      if (m_grid[i][j - 1].S == Stato::INFETTO) {
        std::cout << "c'è infetto" << '\n';

        cell.m_counter++;
      }
    }
    if (Esiste(i, j + 1)) {
      std::cout << "Esiste "
                << "in m_posizione"
                << " riga " << i << " colonna " << j + 1 << '\n';

      if (m_grid[i][j + 1].S == Stato::INFETTO) {
        std::cout << "c'è infetto" << '\n';
        cell.m_counter++;
      }
    }
  }

  void Aggiorna() {
    srand(time(NULL));
    m_giorni++;
    for (int i = 0; i < m_numero_lato; i++) {
      for (int j = 0; j < m_numero_lato; j++) {
        Cellula& cell = m_grid[i][j];
        std::cout << "riga " << i << " colonna " << j << '\n';
        if (cell.S == Stato::VULNERABILE) {
          std::cout << "sono vulnerabile" << '\n';
          int esponente = cell.m_counter;
          // cell.m_numero.setString(std::to_string(esponente));
          if (esponente == 0) {
            continue;
          } else {
            float prob_sano = pow(1 - m_probabilita_contagio, esponente);  // beta o gamma?
            std::cout << "prob sano" << prob_sano << '\n';
            float estrazione = (rand() % 101) / 100.f;  // IL .F è FONDAMENTALE
            std::cout << "estrazione " << estrazione << '\n';
            if (estrazione > prob_sano) {
              cell.S = Stato::INFETTO;
              std::cout << "ora sono infetto" << '\n';
              cell.Aggiorna_colore();
            }
          }
        }

        else if (cell.S == Stato::INFETTO) {
          std::cout << "sono arrivato qua" << '\n';
          cell.m_infection_days++;
          if ((rand() % 100) / 100.f < m_probabilita_guarigione) {
            cell.S = Stato::RIMOSSO;
            cell.Aggiorna_colore();
            // qua forse ci sta fare così
          }
        }

        cell.m_counter = 0;
      }
    }
  }

  void Avanza() {
    if (m_orologio.getElapsedTime().asSeconds() > 3) {
      for (int i = 0; i < m_numero_lato; i++) {
        for (int j = 0; j < m_numero_lato; j++) {
          Aggiorna_counter(i, j);
        }
      }
      Aggiorna();
      m_orologio.restart();
    }
  }
};

class Mondo /* : public sf::Drawable  */ {
 private:  // la draw non va nel protected??
  /* sf::VertexArray Griglia; */
  GUI overlay;

  Finestra a_window;

  Animazione m_dinamica;  // fare altra classe che contiene sia griglia sia bordi, gestire tutto lì

  Automa m_statica;
  // non ho capito perhcè qui dentro non ci posso mettere il costruttore;

  sf::Clock m_timer;
  sf::Time m_trascorso;

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

  void Azzera();

  Finestra* Prendi_finestra() { return &a_window; }

  void Gestisci_input() {
    a_window.Update();  // gestisce gli eventi
    if (a_window.Isclosed()) {
      a_window.~Finestra();
    }
  }

  void Aggiorna() {
    m_dinamica.Aggiorna_Generale();

    m_statica.Avanza();
  }

  void Disegna() {
    a_window.Pulisci();

    a_window.Disegna(m_dinamica);

    a_window.Disegna(m_statica);

    a_window.Disegna(overlay);

    a_window.Mostra();
  }
};

#endif