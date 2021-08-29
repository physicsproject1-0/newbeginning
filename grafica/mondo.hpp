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

#include "animazione.hpp"
#include "automa.hpp"
#include "finestra.hpp"
#include "gui.hpp"

#ifndef MONDO_HPP
#define MONDO_HPP

struct Inserimento {
  // dinamica
  int m_d_numero_persone;
  float m_d_parametro_beta;   // probabilità contagio
  float m_d_parametro_gamma;  // probabilità guarigione
  int m_d_infetti_iniziali;
  int m_d_rimossi_iniziali;

  // statica
  int m_s_dimensione_lato;
  float m_s_parametro_beta;   // probabilità contagio
  float m_s_parametro_gamma;  // probabilità guarigione
  float m_s_parametro_eta;    // probabilità morte
  int m_s_infetti_iniziali;
  int m_s_rimossi_iniziali;

  Inserimento();
};

class Mondo {
 private:
  GUI m_overlay;

  Animazione m_dinamica;

  Automa m_statica;

  Finestra m_window;

 public:
  Mondo(Inserimento t_inserimento);

  Finestra* Prendi_finestra();
  void GestisciInput();
  void Aggiorna();
  void Disegna();
};

#endif