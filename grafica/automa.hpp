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

class Cellula : public Bordi {
 public:
  Cellula(sf::Vector2f m_posizione, sf::Vector2f dimensione) : Bordi(dimensione, m_posizione), m_counter(0), m_infection_days(0) {
    m_rettangolo.setOutlineColor(sf::Color::White);
    m_rettangolo.setOutlineThickness(2.f);
    Aggiorna_colore();
  }

  void Aggiorna_colore();

  int m_counter;

  int m_infection_days;

  Stato m_S = Stato::VULNERABILE;
};

class Automa : public sf::Drawable {
  std::vector<std::vector<Cellula>> m_grid;
  sf::Vector2f m_posizione;
  sf::Vector2f m_dimensioni;
  int m_numero_lato;
  float m_probabilita_contagio;
  float m_probabilita_guarigione;
  float m_probabilita_morte;
  Bordi limiti;
  bool m_is_stopped;
  Censimento m_censimento;
  int m_giorni = 0;
  sf::Clock m_orologio;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates) const {
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
        m_numero_lato{t_numero},
        m_probabilita_contagio{t_probabilita_contagio},
        m_probabilita_guarigione{t_probabilita_guarigione},
        m_probabilita_morte{t_probabilita_morte},
        limiti{t_dimensione, t_posizione},
        m_is_stopped{true},
        m_censimento{t_numero * t_numero - infetti - rimossi, infetti, rimossi, 0} {
    if (t_probabilita_guarigione + t_probabilita_morte > 1) {
      throw std::runtime_error("la somma delle probabilità di morire e di guarire deve essere massimo 1");
    }

    float t_lunghezza_x = m_dimensioni.x / m_numero_lato;

    float t_lunghezza_y = m_dimensioni.y / m_numero_lato;

    for (int i = 0; i < m_numero_lato; i++) {
      std::vector<Cellula> riga;
      for (int j = 0; j < m_numero_lato; j++) {
        sf::Vector2f m_posizionemovente(m_posizione.x + j * t_lunghezza_x, m_posizione.y + i * t_lunghezza_y);
        Cellula riempi(m_posizionemovente, sf::Vector2f(t_lunghezza_x, t_lunghezza_y));

        riga.push_back(riempi);
      }
      m_grid.push_back(riga);
    }
    Genera(infetti, rimossi);
  }

  Bordi get_bordi();
  void Genera(int infette, int rimosse);
  bool Esiste(int i, int j);
  void Aggiorna_counter(int i, int j);
  void Aggiorna();
  void Avanza();
  std::pair<int, int> CheckMousePosition(sf::Vector2f t_coordinate_mouse);
  void ChangeStatus(std::pair<int, int> t_coordinate, Stato t_stato);
  void AzzeraOrologio();
  void StopAutoma();
  void StartAutoma();
  bool IsStopped();
  Censimento GetCensimento();
  void Aggiorna_senza_avanzare();
};

#endif
