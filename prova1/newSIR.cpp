#include "newSIR.hpp"

#include <cmath>

namespace epidemia {
std::vector<State> SIR::Riempimento() {
  std::vector<State> simulazione{m_stato_iniziale};

  // durata simulazione non include anche il giorno di inizio

  for (int i = 0; i < m_durata_simulazione; i++) {
    State stato = simulazione.back();
    State newState;

    newState.m_suscettibili = stato.m_suscettibili - (m_beta * stato.m_suscettibili * stato.m_infetti) / m_N;
    newState.m_infetti = stato.m_infetti + (m_beta * stato.m_suscettibili * stato.m_infetti) / m_N - m_gamma * stato.m_infetti;
    newState.m_rimossi = m_N - newState.m_suscettibili - newState.m_infetti;
    newState.giorno = i + 1;

    simulazione.push_back(newState);
  }

  return simulazione;
}

State SIR::Approx(State obj) {
  State stato;
  stato.m_suscettibili = static_cast<int>(obj.m_suscettibili);
  stato.m_infetti = static_cast<int>(obj.m_infetti);
  stato.m_rimossi = static_cast<int>(obj.m_rimossi);
  stato.giorno = obj.giorno;
  return stato;
}

std::vector<State> SIR::Convertitore(std::vector<State> const& vergine) {
  std::vector<State> risultato{vergine[0]};
  for (unsigned int i = 1; i < vergine.size(); ++i) {
    State stato_approssimato = Approx(vergine[i]);
    State precedente = risultato.back();

    if (precedente.m_infetti == 0) {
      stato_approssimato.m_suscettibili = precedente.m_suscettibili;
      stato_approssimato.m_rimossi = precedente.m_rimossi;
      risultato.push_back(stato_approssimato);
      continue;
    }

    int somma = stato_approssimato.m_suscettibili + stato_approssimato.m_infetti + stato_approssimato.m_rimossi;

    if (somma != m_N) {
      int a = m_N - somma;

      float r_m_suscettibili = std::modf(vergine[i].m_suscettibili, &stato_approssimato.m_suscettibili);
      float r_m_infetti = std::modf(vergine[i].m_infetti, &stato_approssimato.m_infetti);
      float r_m_rimossi = std::modf(vergine[i].m_rimossi, &stato_approssimato.m_rimossi);

      while (a != 0) {
        if ((r_m_suscettibili > r_m_infetti) && (r_m_suscettibili > r_m_rimossi) && (stato_approssimato.m_suscettibili < precedente.m_suscettibili)) {
          stato_approssimato.m_suscettibili += 1;
          r_m_suscettibili = 0;
        } else if ((r_m_infetti < r_m_rimossi) || (stato_approssimato.m_rimossi < precedente.m_rimossi)) {
          stato_approssimato.m_rimossi += 1;
          r_m_infetti = 0;
        } else {
          stato_approssimato.m_infetti += 1;
          r_m_rimossi = 0;
        }
        --a;
      }

      risultato.push_back(stato_approssimato);
    } else {
      risultato.push_back(stato_approssimato);
    }
  }

  return risultato;
}

void SIR::Print(std::vector<State> vettore) {
  std::cout << "   +-------------+-------------+-------------+-------------+ \n"
               "   |  T(giorni)  |      S      |      I      |      R      | \n"
               "   +-------------+-------------+-------------+-------------+ \n"
            << std::fixed;

  for (auto const& i : vettore) {
    std::cout << std::right << "   |" << std::setw(13) << std::setprecision(0) << i.giorno << "|" << std::setw(13) << i.m_suscettibili << "|"
              << std::setw(13) << i.m_infetti << "|" << std::setw(13) << i.m_rimossi << "|"
              << "\n";
  }

  std::cout << "   +-------------+-------------+-------------+-------------+ \n";
}

// Valori stampati separati da una virgola
void SIR::PrintSempliceVirgola(std::vector<State> vettore) {
  std::cout << "      T(giorni)         S             I             R        \n" << std::fixed;

  for (auto const& i : vettore) {
    std::cout << std::right << std::setw(13) << std::setprecision(0) << i.giorno << "," << std::setw(13) << i.m_suscettibili << "," << std::setw(13)
              << i.m_infetti << "," << std::setw(13) << i.m_rimossi << "\n";
  }
}

void SIR::PrintSempliceSpazio(std::vector<State> vettore) {
  std::cout << "      T(giorni)         S             I             R        \n" << std::fixed;

  for (auto const& i : vettore) {
    std::cout << std::right << std::setw(13) << std::setprecision(0) << i.giorno << " " << std::setw(13) << i.m_suscettibili << " " << std::setw(13)
              << i.m_infetti << " " << std::setw(13) << i.m_rimossi << "\n";
  }
}

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

// Funzione per stampare input e output
SIR Insert() {
  double beta;
  double gamma;
  int S_I;
  int I_I;
  int R_I;
  int giorni;

  std::cout << "Buongiorno, inserisca i parametri beta e gamma del modello SIR \n";

  std::cout << "beta >> ";
  if (!IsInputGood(beta) || beta < 0 || beta > 1) {
    throw std::runtime_error{"Il parametro beta deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "gamma >> ";
  if (!IsInputGood(gamma) || gamma < 0 || gamma > 1) {
    throw std::runtime_error{"Il parametro gamma deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Ora inserisca il numero iniziale di persone suscettibili nel "
               "nostro campione \n";
  if (!IsInputGood(S_I) || S_I < 0) {
    throw std::runtime_error{"Il numero di soggetti suscettibili deve essere un intero positivo"};
  }

  std::cout << "Ora inserisca il numero iniziale di persone infetti nel nostro "
               "campione \n";
  if (!IsInputGood(I_I) || I_I < 0) {
    throw std::runtime_error{"Il numero di soggetti infetti deve essere un intero positivo"};
  }

  std::cout << "Ora inserisca il numero iniziale di persone rimossi nel nostro "
               "campione \n";
  if (!IsInputGood(R_I) || R_I < 0) {
    throw std::runtime_error{"Il numero di soggetti rimossi deve essere un intero positivo"};
  }

  std::cout << "Ora inserisca la durata del nostro esperimento \n";
  if (!IsInputGood(giorni) || giorni < 0) {
    throw std::runtime_error{"Il numero di giorni deve essere un intero positivo"};
  }

  State s0;

  s0.m_suscettibili = S_I;
  s0.m_infetti = I_I;
  s0.m_rimossi = R_I;
  int NUMERO = S_I + I_I + R_I;
  return {s0, giorni, beta, gamma, NUMERO};
}
}  // namespace epidemia