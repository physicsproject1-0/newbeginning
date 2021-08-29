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

#ifndef CLASSE_HPP
#define CLASSE_HPP

struct Inserimento {
  int m_d_numero_persone;
  float m_d_parametro_beta;  // probabilita contagio
  float m_d_parametro_gamma;
  int m_d_infetti_iniziali;
  int m_d_rimossi_iniziali;

  int m_s_dimensione_lato;
  float m_s_parametro_beta;   // probabilita contagio
  float m_s_parametro_gamma;  // probabilita guarigione
  float m_s_parametro_eta;    // probabilita morte
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

  // funzioni da usare nel main

  Finestra* Prendi_finestra() { return &m_window; }

  void Gestisci_input() {
    m_window.Update();  // gestisce gli eventi
    if (m_window.Isclosed()) {
      m_window.~Finestra();
    }
  }

  void Aggiorna() {
    if (m_window.GetVista() == Vista::Animazione) {
      if (!m_dinamica.IsStopped()) {
        m_dinamica.Aggiorna_Generale();

        m_statica.AzzeraOrologio();
      } /* else {
        m_dinamica.AzzeraOrologiPersone();
      } */

    } else {
      /* m_dinamica.AzzeraOrologiPersone(); */
      if (!m_statica.IsStopped()) {
        m_statica.Avanza();

        m_dinamica.AzzeraOrologio();
      }
    }
  }

  void Disegna() {
    m_window.Pulisci();
    if (m_window.GetVista() == Vista::Animazione) {
      m_window.Disegna(m_dinamica);

    } else {
      m_window.Disegna(m_statica);
    }

    m_window.Disegna(m_overlay);

    m_window.Mostra();
  }
};

#endif