#ifndef NEWSIR_HPP
#define NEWSIR_HPP

#include <iomanip>
#include <iostream>
#include <vector>

namespace epidemia {

struct State {
  double m_suscettibili;
  double m_infetti;
  double m_rimossi;
  int giorno = 0;
};

class SIR {
 private:
  State m_stato_iniziale;
  int m_durata_simulazione;
  double m_beta;
  double m_gamma;
  int const m_N;

 public:
  SIR(State const& stato_iniziale, int durata_simulazione, double beta, double gamma, int const N)
      : m_stato_iniziale{stato_iniziale}, m_durata_simulazione{durata_simulazione}, m_beta{beta}, m_gamma{gamma}, m_N{N} {}

  std::vector<State> Riempimento();
  State Approx(State obj);
  std::vector<State> Convertitore(std::vector<State> const& vergine);
  void Print(std::vector<State> vettore);
  void PrintSempliceVirgola(std::vector<State> vettore);
  void PrintSempliceSpazio(std::vector<State> vettore);
  void PrintGrafico(std::vector<State> vettore);
};

SIR Insert();
void Grafico(std::vector<double> x, std::vector<double> y, std::vector<double> z);

}  // namespace epidemia

#endif