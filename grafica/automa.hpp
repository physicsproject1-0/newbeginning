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

#include "gui.hpp"

#ifndef AUTOMA_HPP
#define AUTOMA_HPP

class Cellula : public Bordi {  // se è una struct non funziona l'inheritance?
                                // metterci anche states altrimenti rompe il casso
                                // non posso loadare un font qui dentro direttamente

 public:
  Cellula(sf::Vector2f m_posizione, sf::Vector2f dimensione) : Bordi(dimensione, m_posizione), m_counter(0), m_infection_days(0) {  // funzionerà

    m_rettangolo.setOutlineColor(sf::Color::White);
    m_rettangolo.setOutlineThickness(2.f);
    Aggiorna_colore();
  }

  void Aggiorna_colore();

  int m_counter;

  int m_infection_days;

  Stato m_S = Stato::VULNERABILE;
};

class Automa : public sf::Drawable {  // ESTRARRE LE CLASSI NESTATE E DISTINGUERE I MORTI DAI GUARITI
  /* sf::Font font; */

  // mettere errori per m_dimensioni minori di 0?
  std::vector<std::vector<Cellula>> m_grid;

  sf::Vector2f m_posizione;
  sf::Vector2f m_dimensioni;
  int m_numero_lato;
  float m_probabilita_contagio;
  float m_probabilita_guarigione;
  float m_probabilita_morte;
  Bordi limiti;
  bool m_is_stopped;
  Censimento popolazione;
  int m_giorni = 0;
  sf::Clock m_orologio;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int i = 0; i < m_numero_lato; i++) {
      for (int j = 0; j < m_numero_lato; j++) {
        target.draw(m_grid[i][j]);
      }
    }
    target.draw(limiti);
  }

 public:
  Automa(sf::Vector2f t_posizione, sf::Vector2f t_dimensione, int t_numero, float t_probabilita_contagio, float t_probabilita_guarigione,
         float t_probabilita_morte, int infetti, int rimossi)
      : m_posizione{t_posizione},
        m_dimensioni{t_dimensione},
        m_numero_lato{t_numero},  // rimossi si considerano guariti
        m_probabilita_contagio{t_probabilita_contagio},
        m_probabilita_guarigione{t_probabilita_guarigione},
        m_probabilita_morte{t_probabilita_morte},
        limiti{t_dimensione, t_posizione},
        m_is_stopped{true},
        popolazione{t_numero * t_numero - infetti - rimossi, infetti, rimossi, 0} {
    assert(m_probabilita_contagio <= 1 && m_probabilita_contagio >= 0);  // mettere except

    if (t_probabilita_guarigione + t_probabilita_morte > 1) {
      throw std::runtime_error("la somma delle probabilità di morire e di guarire deve essere massimo 1");
    }

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
  Bordi get_bordi() { return limiti; }
  void Genera(int infette, int rimosse);
  bool Esiste(int i, int j);

  void Aggiorna_counter(int i, int j);
  void Aggiorna();

  /*void contatore (){
    stampa il censimento, non so come si fa. Edo aiutami tu
     } */
  void Avanza();
  std::pair<int, int> CheckMousePosition(sf::Vector2f t_coordinate_mouse);
  void ChangeStatus(std::pair<int, int> t_coordinate, Stato t_stato);
  void AzzeraOrologio();
  void StopAutoma();
  void StartAutoma();
  bool IsStopped();
  Censimento GetCensimento() { return popolazione; }
  void AggiornaSenzaAvanzare() {
    popolazione = {0, 0, 0, 0};
    for (int i = 0; i < m_numero_lato; i++) {
      for (int j = 0; j < m_numero_lato; j++) {
        Cellula& cell = m_grid[i][j];
        censimento(cell, popolazione);
        cell.m_counter = 0;
        cell.Aggiorna_colore();
      }
    }
  }
};

#endif
