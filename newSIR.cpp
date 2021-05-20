#include "newSIR.hpp"

#include <cmath>

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

// Approssima ad int, passare by ref
State SIR::approx(State obj) {
  State stato;
  stato.suscettibili = static_cast<int>(obj.suscettibili);
  stato.infetti = static_cast<int>(obj.infetti);
  stato.rimossi = static_cast<int>(obj.rimossi);
  stato.giorno = obj.giorno;
  return stato;
}

std::vector<State> SIR::convertitore(std::vector<State> vergine) {
  std::vector<State> risultato{vergine[0]};
  for (unsigned int i = 1; i < vergine.size(); ++i) {
    State stato_approssimato = approx(vergine[i]);
    State precedente = risultato.back();

    if (stato_approssimato.suscettibili + stato_approssimato.infetti + stato_approssimato.rimossi != N) {
      int a = N - (stato_approssimato.suscettibili + stato_approssimato.infetti + stato_approssimato.rimossi);
      float r_suscettibili =
          std::modf(vergine[i].suscettibili, &stato_approssimato.suscettibili);  // modf() mi dà la parte decimale di un numero double o float
      float r_infetti = std::modf(vergine[i].infetti, &stato_approssimato.infetti);
      float r_rimossi = std::modf(vergine[i].rimossi, &stato_approssimato.rimossi);

      while (a != 0) {
        if ((r_suscettibili > r_infetti) && (r_suscettibili > r_rimossi) && (stato_approssimato.suscettibili < precedente.suscettibili)) {
          stato_approssimato.suscettibili += 1;
          r_suscettibili = 0;
        } else if ((r_infetti < r_rimossi) && (stato_approssimato.rimossi > precedente.rimossi)) {
          stato_approssimato.rimossi += 1;
          r_infetti = 0;
        } else {
          stato_approssimato.infetti += 1;
          r_rimossi = 0;
        }
        a--;
      }
      risultato.push_back(stato_approssimato);
    } else {
      risultato.push_back(stato_approssimato);
    }
  }

  return risultato;
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
  int NUMERO = S_I + I_I + R_I;  // testare anche l'altra
  return {s0, giorni, beta, gamma, NUMERO};
}
}  // namespace epidemia