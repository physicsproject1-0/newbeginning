#include "mondo.hpp"

#include <algorithm>
#include <sstream>

// Funzione di controllo input

template <class T>
bool IsInputGood(T& a) {
  std::string line;
  if (std::getline(std::cin, line)) {
    std::stringstream ss(line);

    if ((ss >> a) && ss.eof()) {
      return true;
    }
  }
  return false;
}

// Inserimento

Inserimento::Inserimento() {
  std::cout << "Buongiorno, inserisca i parametri iniziali relativi alla simulazione \n";

  std::cout << "Automa Cellulare: \n";

  std::cout << "Numero di cellule per ogni lato >> ";
  if (!IsInputGood(m_s_dimensione_lato) || m_s_dimensione_lato < 0 || m_s_dimensione_lato > 10) {
    throw std::runtime_error{"Il parametro dimensione lato deve essere un intero compreso tra 0 e 10"};
  }

  std::cout << "Parametro beta(relativo alla probabilità di contagio) >> ";
  if (!IsInputGood(m_s_parametro_beta) || m_s_parametro_beta < 0 || m_s_parametro_beta > 1) {
    throw std::runtime_error{"Il parametro beta deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Parametro gamma(relativo alla probabilità di guarigione) >> ";
  if (!IsInputGood(m_s_parametro_gamma) || m_s_parametro_gamma < 0 || m_s_parametro_gamma > 1) {
    throw std::runtime_error{"Il parametro gamma deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Parametro eta(relativo alla probabilità di morte) >> ";
  if (!IsInputGood(m_s_parametro_eta) || m_s_parametro_eta < 0 || m_s_parametro_eta > 1) {
    throw std::runtime_error{"Il parametro eta deve essere un decimale compreso tra 0 e 1"};
  }

  if (m_s_parametro_eta + m_s_parametro_gamma > 1) {
    throw std::runtime_error{"La somma delle probabilità di morire e guarire deve essere compresa tra 0 e 1"};
  }

  std::cout << "Persone infette >> ";
  if (!IsInputGood(m_s_infetti_iniziali) || m_s_infetti_iniziali < 0 || m_s_infetti_iniziali > m_s_dimensione_lato * m_s_dimensione_lato) {
    throw std::runtime_error{
        "Il numero di soggetti infetti deve essere un intero positivo e minore del numero massimo di persone presenti(lato*lato)"};
  }

  std::cout << "Persone rimosse(guarite) >> ";
  if (!IsInputGood(m_s_rimossi_iniziali) || m_s_rimossi_iniziali < 0 ||
      m_s_rimossi_iniziali > (m_s_dimensione_lato * m_s_dimensione_lato) - m_s_infetti_iniziali) {
    throw std::runtime_error{
        "Il numero di soggetti rimossi(guariti) deve essere un intero positivo e minore del numero massimo di persone ancora infettabili"};
  }

  std::cout << "Animazione: \n";
  std::cout << "Numero di persone totali >> ";
  if (!IsInputGood(m_d_numero_persone) || m_d_numero_persone < 0 || m_d_numero_persone > 100) {
    throw std::runtime_error{"Il parametro numero persone deve essere un intero compreso tra 0 e 100"};
  }

  std::cout << "Parametro beta(relativo alla probabilità di contagio) >> ";
  if (!IsInputGood(m_d_parametro_beta) || m_d_parametro_beta < 0 || m_d_parametro_beta > 1) {
    throw std::runtime_error{"Il parametro beta deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Parametro gamma(relativo alla probabilità di guarigione) >> ";
  if (!IsInputGood(m_d_parametro_gamma) || m_d_parametro_gamma < 0 || m_d_parametro_gamma > 1) {
    throw std::runtime_error{"Il parametro gamma deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Persone infette >> ";
  if (!IsInputGood(m_d_infetti_iniziali) || m_d_infetti_iniziali < 0 || m_d_infetti_iniziali > m_d_numero_persone) {
    throw std::runtime_error{"Il numero di soggetti infetti deve essere un intero positivo e minore del numero massimo di persone presenti"};
  }

  std::cout << "Persone rimosse(guarite) >> ";
  if (!IsInputGood(m_d_rimossi_iniziali) || m_d_rimossi_iniziali < 0 || m_d_rimossi_iniziali > m_d_numero_persone - m_d_infetti_iniziali) {
    throw std::runtime_error{
        "Il numero di soggetti rimossi(guariti) deve essere un intero positivo e minore del numero massimo di persone ancora infettabili"};
  }
}

// Mondo

Mondo::Mondo(Inserimento t_inserimento)
    : m_overlay(sf::Vector2f(70, 500)),
      m_dinamica(t_inserimento.m_d_numero_persone, t_inserimento.m_d_infetti_iniziali, t_inserimento.m_d_rimossi_iniziali,
                 t_inserimento.m_d_parametro_beta, t_inserimento.m_d_parametro_gamma),
      m_statica(sf::Vector2f(1800, 1800), sf::Vector2f(500, 500), t_inserimento.m_s_dimensione_lato, t_inserimento.m_s_parametro_beta,
                t_inserimento.m_s_parametro_gamma, t_inserimento.m_s_parametro_eta, t_inserimento.m_s_infetti_iniziali,
                t_inserimento.m_s_rimossi_iniziali),
      m_window("test", sf::Vector2u(900, 600), &m_overlay, &m_dinamica, &m_statica, m_dinamica.GetBordi(), m_statica.GetBordi()) {}

Finestra* Mondo::Prendi_finestra() { return &m_window; }

void Mondo::GestisciInput() {
  m_window.Update();  // gestisce gli eventi
  if (m_window.IsClosed()) {
    m_window.~Finestra();
  }
}

// Va avanti solo la simulazione inquadrata, l'altra si stoppa
void Mondo::Aggiorna() {
  if (m_window.GetVista() == Vista::Animazione) {
    if (!m_dinamica.IsStopped()) {
      m_dinamica.Aggiorna_Generale();

      m_statica.AzzeraOrologio();
    }

  } else {
    if (!m_statica.IsStopped()) {
      m_statica.Avanza();

      m_dinamica.AzzeraOrologio();
    }
  }
}

// Se la vista è sull'animazione disegna dinamica, sennò statica
void Mondo::Disegna() {
  m_window.Pulisci();
  if (m_window.GetVista() == Vista::Animazione) {
    m_window.Disegna(m_dinamica);

  } else {
    m_window.Disegna(m_statica);
  }

  // disegna la GUI
  m_window.Disegna(m_overlay);

  // finestra.display
  m_window.Mostra();
}
