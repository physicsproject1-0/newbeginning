#ifndef NEWSIR_HPP
#define NEWSIR_HPP

#include <iomanip>
#include <iostream>
#include <vector>

namespace epidemia {

struct State {
  double suscettibili;
  double infetti;
  double rimossi;
  int giorno = 0;
};

class SIR {
 private:
  State m_stato_iniziale;
  int m_durata_simulazione;
  double m_beta;
  double m_gamma;
  int const N;

 public:
  SIR(State const& stato_iniziale, int durata_simulazione, double beta, double gamma, int const m_N)
      : m_stato_iniziale{stato_iniziale}, m_durata_simulazione{durata_simulazione}, m_beta{beta}, m_gamma{gamma}, N{m_N} {}

  std::vector<State> riempimento();
  State approx(State obj);
  std::vector<State> convertitore(std::vector<State> const& vergine);
  void print(std::vector<State> vettore);
  void print_semplice_virgola(std::vector<State> vettore);
  void print_semplice_spazio(std::vector<State> vettore);
  void print_grafico(std::vector<State> vettore);
};

SIR insert();
void grafico(std::vector<double> x, std::vector<double> y, std::vector<double> z);

}  // namespace epidemia

#endif