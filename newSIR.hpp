#ifndef NEWSIR_HPP
#define NEWSIR_HPP

#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

namespace epidemia {

struct State {
  double suscettibili;
  double infetti;
  double rimossi;
  int giorno = 0;

  // State()=default; //Costruttore di default per newState perchè non funziona
  // newstate,
};

// nelle struct non serve il costruttore
// se uso le graffe mi inizializza tutto al valore di default, chiamare
// costruttore con tonde senza argomenti non va bene perchè mi definisce una
// funzione

class SIR {
 private:
  State m_stato_iniziale;
  int m_durata_simulazione;
  double m_beta;
  double m_gamma;

  // boh forse, problema n int? per tirarlo fuori
  const int N = m_stato_iniziale.suscettibili + m_stato_iniziale.infetti + m_stato_iniziale.rimossi;

 public:
  SIR(State const& stato_iniziale, int durata_simulazione, double beta,
      double gamma)  // costruttore
  : m_stato_iniziale{stato_iniziale}, m_durata_simulazione{durata_simulazione}, m_beta{beta}, m_gamma{gamma} {}

  // SIR()=default;

  std::vector<State> riempimento();
  void print(std::vector<State> vettore);
  void print_semplice(std::vector<State> vettore);
};

SIR insert();

}  // namespace epidemia

#endif