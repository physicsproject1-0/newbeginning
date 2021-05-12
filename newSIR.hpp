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
  std::vector<State> simulazione_finale;

  // boh forse, problema n int? per tirarlo fuori
  const int N = m_stato_iniziale.suscettibili + m_stato_iniziale.infetti + m_stato_iniziale.rimossi;

 public:
  SIR(State const& stato_iniziale, int durata_simulazione, double beta,
      double gamma)  // costruttore
  : m_stato_iniziale{stato_iniziale}, m_durata_simulazione{durata_simulazione}, m_beta{beta}, m_gamma{gamma} {}

  // SIR()=default;

  void riempimento() {
    std::vector<State> simulazione{m_stato_iniziale};

    // durata simulazione non include anche il giorno di inizio

    for (int i = 0; i < m_durata_simulazione; i++) {
      State stato = simulazione.back();
      State newState;  // non chiamare il costruttore con le tonde, è ambiguità

      newState.suscettibili = stato.suscettibili - (m_beta * stato.suscettibili * stato.infetti) / N;
      newState.infetti = stato.infetti + (m_beta * stato.suscettibili * stato.infetti) / N - m_gamma * stato.infetti;
      newState.rimossi = stato.rimossi + m_gamma * stato.infetti;
      newState.giorno = i + 1;

      simulazione.push_back(newState);
    }

    simulazione_finale = simulazione;
  }

  void print() {
    std::cout << "   +-------------+-------------+-------------+-------------+ \n"
                 "   |  T(giorni)  |      S      |      I      |      R      | \n"
                 "   +-------------+-------------+-------------+-------------+ \n"
              << std::fixed;

    for (auto const& i : simulazione_finale) {
      std::cout << std::right << "   |" << std::setw(13) << i.giorno << "|" << std::setw(13) << i.suscettibili << "|" << std::setw(13) << i.infetti
                << "|" << std::setw(13) << i.rimossi << "|"
                << "\n";
    }

    std::cout << "   +-------------+-------------+-------------+-------------+ \n";
  }

  void print_semplice() {
    std::cout << std::setw(13) << "T" << std::setw(13) << "S" << std::setw(13) << "I" << std::setw(13) << "R" << '\n';

    for (auto const& i : simulazione_finale) {
      std::cout << std::right << std::setw(13) << i.giorno << std::setw(13) << i.suscettibili << std::setw(13) << i.infetti << std::setw(13)
                << i.rimossi << "\n";
    }
  }
};

// Funzione per stampare input e output
SIR insert() {
  double beta;
  double gamma;
  int S_I;
  int I_I;
  int R_I;
  int giorni;

  std::cout << "Buongiorno, inserisca i parametri beta e gamma del modello SIR \n";

  std::cout << "beta >> ";
  std::cin >> beta;
  if (beta < 0 || beta > 1) {
    throw std::runtime_error{"Il parametro gamma deve essere compreso tra 0 e 1"};
  }

  std::cout << "gamma >> ";
  std::cin >> gamma;
  if (gamma < 0 || gamma > 1) {
    throw std::runtime_error{"Il parametro gamma deve essere compreso tra 0 e 1"};
  }

  std::cout << "Ora inserisca il numero iniziale di persone suscettibili nel "
               "nostro campione \n";
  std::cin >> S_I;
  if (S_I < 0) {
    throw std::runtime_error{"Il numero di soggetti suscettibili deve essere positivo"};
  }

  std::cout << "Ora inserisca il numero iniziale di persone infetti nel nostro "
               "campione \n";
  std::cin >> I_I;
  if (I_I < 0) {
    throw std::runtime_error{"Il numero di soggetti infetti deve essere positivo"};
  }

  std::cout << "Ora inserisca il numero iniziale di persone rimossi nel nostro "
               "campione \n";
  std::cin >> R_I;
  if (R_I < 0) {
    throw std::runtime_error{"Il numero di soggetti rimossi deve essere positivo"};
  }

  std::cout << "Ora inserisca la durata del nostro esperimento \n";
  std::cin >> giorni;
  if (giorni < 0) {
    throw std::runtime_error{"Il numero di soggetti suscettibili deve essere positivo"};
  }

  epidemia::State s0;

  s0.suscettibili = S_I;
  s0.infetti = I_I;
  s0.rimossi = R_I;

  return {s0, giorni, beta, gamma};
}

}  // namespace epidemia

#endif