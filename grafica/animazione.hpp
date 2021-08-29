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
#include "struct_enum.hpp"

#ifndef ANIMAZIONE_HPP
#define ANIMAZIONE_HPP

struct Persona {
  sf::Vector2f m_centro;
  float m_raggio;
  sf::Vector2f m_vel;
  Stato m_S;
  bool checked = false;
  int m_numero_contatti = 0;
};

class Animazione : public sf::Drawable {
  Bordi m_limiti;
  sf::Clock m_orologio2;
  sf::Texture m_ominoprova;
  sf::VertexArray m_struttura;
  std::map<int, Persona> m_popolazione;

  int m_d_numero_persone;    // numero totale di persone
  int m_d_infetti_iniziali;  // numero iniziale di infetti
  int m_d_rimossi_iniziali;  // numero iniziale di rimossi
  bool m_is_stopped;
  Censimento m_censimento;
  float m_d_parametro_beta;   // influenza contagio
  float m_d_parametro_gamma;  // influenza le morti

  void Collisione();
  void Conteggio_contatti();
  void Morte_persona();
  void SetAllTextures();

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_ominoprova;
    target.draw(m_struttura, states);
    target.draw(m_limiti);
  }

 public:
  Animazione(int t_d_numero_persone, int t_d_infetti_iniziali, int t_d_rimossi_iniziali, float t_d_parametro_beta, float t_d_parametro_gamma)
      : m_d_numero_persone{t_d_numero_persone},
        m_d_infetti_iniziali{t_d_infetti_iniziali},
        m_d_rimossi_iniziali{t_d_rimossi_iniziali},
        m_limiti(sf::Vector2f(600, 400), sf::Vector2f(100, 100)),
        m_is_stopped{true},
        m_censimento{m_d_numero_persone - m_d_infetti_iniziali - m_d_rimossi_iniziali, m_d_infetti_iniziali, m_d_rimossi_iniziali, 0},
        m_d_parametro_beta{t_d_parametro_beta},
        m_d_parametro_gamma{t_d_parametro_gamma} {
    if (!m_ominoprova.loadFromFile("uomini.png")) {
      throw std::runtime_error{"texture loading failed"};
    }

    Persona m_prova;

    // Riempio la mappa (m_popolazione) di persone
    for (int i = 0; i < m_d_numero_persone; i++) {
      m_prova.m_raggio = 13.f;
      m_prova.m_centro =
          sf::Vector2f((Casuale() / 100.f) * (m_limiti.getlimiti().width - 2 * m_prova.m_raggio) + m_limiti.getlimiti().left + m_prova.m_raggio,
                       (Casuale() / 100.f) * (m_limiti.getlimiti().height - 2 * m_prova.m_raggio) + m_limiti.getlimiti().top + m_prova.m_raggio);
      m_prova.m_vel = sf::Vector2f(Casuale() % 50 - 25.f, Casuale() % 50 - 25.f);
      m_popolazione[i] = m_prova;
      m_popolazione[i].m_S = Stato::VULNERABILE;
    }
    for (int i = 0; i < m_d_infetti_iniziali; i++) {
      m_popolazione[static_cast<int>((Casuale() / 100.f) * m_popolazione.size())].m_S = Stato::INFETTO;
    }

    m_struttura.resize(m_popolazione.size() * 3);

    m_struttura.setPrimitiveType(sf::Triangles);

    SetAllTextures();
    Aggiorna_griglia();
  }

  Bordi get_bordi();
  void Aggiorna_griglia();
  void Aggiorna_lista();
  void Check_borders();
  void AzzeraOrologio();
  int Check_occur(Persona const& persona, int m_raggio);
  double Modulo(sf::Vector2f const& vettore);
  void StopAnimazione();
  void StartAnimazione();
  bool IsStopped();
  Censimento GetCensimento();
  void Aggiorna_Generale();
};

#endif