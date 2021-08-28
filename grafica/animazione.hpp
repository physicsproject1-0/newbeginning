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

#ifndef ANIMAZIONE_HPP
#define ANIMAZIONE_HPP

struct Persona {
  sf::Vector2f m_centro;
  float m_raggio;
  sf::Vector2f m_vel;
  // sf::Clock m_cambiom_velocita;   senza collisioni non serve più giusto???
  Stato m_S;
  bool checked = false;
  int m_numero_contatti = 0;
};

class Animazione : public sf::Drawable {
  sf::Clock m_orologio2;
  sf::Texture m_ominoprova;
  sf::VertexArray m_struttura;
  std::map<int, Persona> m_popolazione;  // vedere se meglio vector

  bool m_is_stopped;

  Bordi m_limiti;

  Censimento m_burocrazia;

  // Nel momento in cui collidono due persone, se una era infetta, cambia lo stato anche dell' altra
  void Collisione();
  void Conteggio_contatti();
  void Morte_persona();

  // Funzione con cui carico la Texture verde
  void SetGreenTextures();

  // Funzione in cui carico la Texture rossa sullo stato INFETTO
  void SetRedTextures();

  // Ho immaginato che al 30% muoiano e al 70% guariscono, si possono cambiare le probabilita' of course
  // Funzione in cui carico sullo stato RIMOSSO al 30% la texture grigia e al 70% quella azzurra
  void SetWhiteTextures();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_ominoprova;
    target.draw(m_struttura, states);
    target.draw(m_limiti);
  }

 public:
  Animazione(int n) : m_limiti(sf::Vector2f(600, 400), sf::Vector2f(100, 100)), m_is_stopped{true} {
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
      m_popolazione[i].m_S = Stato::VULNERABILE;
    }

    m_popolazione[rand() % m_popolazione.size() + 1].m_S = Stato::INFETTO;

    m_struttura.resize(m_popolazione.size() * 3);

    m_struttura.setPrimitiveType(sf::Triangles);

    SetGreenTextures();
    SetRedTextures();
    Aggiorna_griglia();  // chiamarlo almeno una volta sennò no good;
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
  void Aggiorna_Generale();
};

#endif