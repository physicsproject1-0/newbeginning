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
  Stato m_stato;

  sf::Clock m_orologio_personale;
  sf::Time m_tempo_trascorso;

  float m_valore_casuale;
};

class Animazione : public sf::Drawable {
  sf::Clock m_orologio2;
  sf::Texture m_ominoprova;
  sf::VertexArray m_struttura;
  std::map<int, Persona> m_popolazione;

  Bordi m_limiti;

  bool m_is_stopped;
  bool m_orologi_stanno_andando;

  int m_d_numero_persone;     // numero totale di persone
  int m_d_infetti_iniziali;   // numero iniziale di infetti
  int m_d_rimossi_iniziali;   // numero iniziale di rimossi
  float m_d_parametro_beta;   // influenza il contagio
  float m_d_parametro_gamma;  // influenza le morti

  Censimento m_censimento;

  void Collisione();
  void AzzeraOrologiVulnerabili();
  void MortePersona();  // fa evolvere la cellula
  void SetAllTextures();

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_ominoprova;
    target.draw(m_struttura, states);
    target.draw(m_limiti);
  }

 public:
  Animazione(int t_d_numero_persone, int t_d_infetti_iniziali, int t_d_rimossi_iniziali, float t_d_parametro_beta, float t_d_parametro_gamma);

  void ImmagazzinaTempo();
  void AzzeraOrologiPersone();
  void StartOrologi();
  void StopOrologi();
  bool GetStatusOrologi();

  Bordi GetBordi();

  void AggiornaGriglia();
  void Aggiorna_lista();
  void CheckBorders();

  void AzzeraOrologio();

  double Modulo(sf::Vector2f const& vettore);

  void StopAnimazione();
  void StartAnimazione();
  bool IsStopped();

  Censimento GetCensimento();

  void Aggiorna_Generale();
};

#endif