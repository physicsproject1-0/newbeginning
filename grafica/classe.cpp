#include "classe.hpp"

#include <algorithm>

Mondo::Mondo(Inserimento t_inserimento)
    : m_overlay(sf::Vector2f(70, 500)),
      m_dinamica(t_inserimento.m_d_numero_persone, t_inserimento.m_d_infetti_iniziali, t_inserimento.m_d_rimossi_iniziali, t_inserimento.m_d_parametro_beta, t_inserimento.m_d_parametro_gamma),
      m_statica(sf::Vector2f(1800, 1800), sf::Vector2f(500, 500), t_inserimento.m_s_dimensione_lato, t_inserimento.m_s_parametro_beta, t_inserimento.m_s_parametro_gamma, t_inserimento.m_s_parametro_eta, t_inserimento.m_s_infetti_iniziali, t_inserimento.m_s_rimossi_iniziali),
      m_window("test", sf::Vector2u(900, 600), &m_overlay, &m_dinamica, &m_statica, m_dinamica.get_bordi(), m_statica.get_bordi()) {
  /*   if (!ominoprova.loadFromFile("uomoverde.png")) {
      throw std::runtime_error{"texture loading failed"};
    }
   */
}

Inserimento::Inserimento() {
  std::cout << "Buongiorno, inserisca i parametri iniziali relativi alla simulazione \n";
  std::cout << "Automa Cellulare: \n";
  std::cout << "Numero di cellule per ogni lato >> ";
  if (!(std::cin >> m_s_dimensione_lato) || m_s_dimensione_lato < 0 || m_s_dimensione_lato > 10) {
    throw std::runtime_error{"Il parametro dimensione lato deve essere un intero compreso tra 0 e 10"};
  }

  std::cout << "Parametro beta(relativo alla probabilità di contagio) >> ";
  if (!(std::cin >> m_s_parametro_beta) || m_s_parametro_beta < 0 || m_s_parametro_beta > 1) {
    throw std::runtime_error{"Il parametro beta deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Parametro gamma(relativo alla probabilità di guarigione) >> ";
  if (!(std::cin >> m_s_parametro_gamma) || m_s_parametro_gamma < 0 || m_s_parametro_gamma > 1) {
    throw std::runtime_error{"Il parametro gamma deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Parametro eta(relativo alla probabilità di morte) >> ";
  if (!(std::cin >> m_s_parametro_eta) || m_s_parametro_eta < 0 || m_s_parametro_eta > 1) {
    throw std::runtime_error{"La somma delle probabilità di morire e guarire deve essere compresa tra 0 e 1"};
  }

  std::cout << "Persone infette \n";
  if (!(std::cin >> m_s_infetti_iniziali) || m_s_infetti_iniziali < 0 || m_s_infetti_iniziali > m_s_dimensione_lato * m_s_dimensione_lato) {
    throw std::runtime_error{
        "Il numero di soggetti infetti deve essere un intero positivo e minore del numero massimo di persone presenti(lato*lato)"};
  }

  std::cout << "Persone rimosse(guarite) \n";
  if (!(std::cin >> m_s_rimossi_iniziali) || m_s_rimossi_iniziali < 0 ||
      m_s_rimossi_iniziali > m_s_dimensione_lato * m_s_dimensione_lato - m_s_infetti_iniziali) {
    throw std::runtime_error{
        "Il numero di soggetti rimossi(guariti) deve essere un intero positivo e minore del numero massimo di persone ancora infettabili"};
  }

  std::cout << "Animazione: \n";

  if (!(std::cin >> m_d_numero_persone) || m_d_numero_persone < 0 || m_d_numero_persone > 100) {
    throw std::runtime_error{"Il parametro numero persone deve essere un intero compreso tra 0 e 100"};
  }

  std::cout << "Parametro beta(relativo alla probabilità di contagio) >> ";
  if (!(std::cin >> m_d_parametro_beta) || m_d_parametro_beta < 0 || m_d_parametro_beta > 1) {
    throw std::runtime_error{"Il parametro beta deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Parametro gamma(relativo alla probabilità di guarigione) >> ";
  if (!(std::cin >> m_d_parametro_gamma) || m_d_parametro_gamma < 0 || m_d_parametro_gamma > 1) {
    throw std::runtime_error{"Il parametro gamma deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Persone infette \n";
  if (!(std::cin >> m_d_infetti_iniziali) || m_d_infetti_iniziali < 0 || m_d_infetti_iniziali > m_d_numero_persone) {
    throw std::runtime_error{
        "Il numero di soggetti infetti deve essere un intero positivo e minore del numero massimo di persone presenti"};
  }

  std::cout << "Persone rimosse(guarite) \n";
  if (!(std::cin >> m_d_rimossi_iniziali) || m_d_rimossi_iniziali < 0 ||
      m_d_rimossi_iniziali > m_d_numero_persone - m_d_infetti_iniziali) {
    throw std::runtime_error{
        "Il numero di soggetti rimossi(guariti) deve essere un intero positivo e minore del numero massimo di persone ancora infettabili"};
  }
}