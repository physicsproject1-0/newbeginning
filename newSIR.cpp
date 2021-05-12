#include "newSIR.hpp"

namespace epidemia {

std::vector<State> SIR::riempimento() {
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

  return simulazione;
}

void SIR::print(std::vector<State> vettore) {
  std::cout << "   +-------------+-------------+-------------+-------------+ \n"
               "   |  T(giorni)  |      S      |      I      |      R      | \n"
               "   +-------------+-------------+-------------+-------------+ \n"
            << std::fixed;

  for (auto const& i : vettore) {
    std::cout << std::right << "   |" << std::setw(13) << i.giorno << "|" << std::setw(13) << i.suscettibili << "|" << std::setw(13) << i.infetti
              << "|" << std::setw(13) << i.rimossi << "|"
              << "\n";
  }

  std::cout << "   +-------------+-------------+-------------+-------------+ \n";
}

void SIR::print_semplice(std::vector<State> vettore) {
  std::cout << std::setw(13) << "T" << std::setw(13) << "S" << std::setw(13) << "I" << std::setw(13) << "R" << '\n';

  for (auto const& i : vettore) {
    std::cout << std::right << std::setw(13) << i.giorno << std::setw(13) << i.suscettibili << std::setw(13) << i.infetti << std::setw(13)
              << i.rimossi << "\n";
  }
}

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