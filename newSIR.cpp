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
    newState.rimossi = N - newState.suscettibili - newState.infetti;
    newState.giorno = i + 1;

    simulazione.push_back(newState);
  }

  return simulazione;
}

State SIR::approx(State obj) {
  State stato;
  stato.suscettibili = static_cast<int>(obj.suscettibili);
  stato.infetti = static_cast<int>(obj.infetti);
  stato.rimossi = static_cast<int>(obj.rimossi);
  stato.giorno = obj.giorno;
  return stato;
}

std::vector<State> SIR::convertitore(std::vector<State> const& vergine) {
  std::vector<State> risultato{vergine[0]};
  for (unsigned int i = 1; i < vergine.size(); ++i) {
    State stato_approssimato = approx(vergine[i]);
    State precedente = risultato.back();
    
    if (precedente.infetti == 0) {
      stato_approssimato.suscettibili = precedente.suscettibili;
      stato_approssimato.rimossi = precedente.rimossi;
      risultato.push_back(stato_approssimato);
    } else {   

    int somma = stato_approssimato.suscettibili + stato_approssimato.infetti + stato_approssimato.rimossi;
    
    if (somma != N) {
      int a = N - somma;

      float r_suscettibili = std::modf(vergine[i].suscettibili, &stato_approssimato.suscettibili);  // modf() mi dà la parte decimale di un numero double o float
      float r_infetti = std::modf(vergine[i].infetti, &stato_approssimato.infetti);
      float r_rimossi = std::modf(vergine[i].rimossi, &stato_approssimato.rimossi);

      while (a != 0) {
        if ((r_suscettibili > r_infetti) && (r_suscettibili > r_rimossi) && (stato_approssimato.suscettibili < precedente.suscettibili)) {
          stato_approssimato.suscettibili += 1;
          r_suscettibili = 0;
        } else if ((r_infetti < r_rimossi) || (stato_approssimato.rimossi < precedente.rimossi)) {
          stato_approssimato.rimossi += 1;
          r_infetti = 0;
        } else {
          stato_approssimato.infetti += 1;
          r_rimossi = 0;
        }
        --a;
      }

      risultato.push_back(stato_approssimato);
    } else {
      risultato.push_back(stato_approssimato);
    }
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
    std::cout << std::right << "   |" << std::setw(13) << std::setprecision(0) << i.giorno << "|" << std::setw(13) << i.suscettibili << "|"
              << std::setw(13) << i.infetti << "|" << std::setw(13) << i.rimossi << "|"
              << "\n";
  }

  std::cout << "   +-------------+-------------+-------------+-------------+ \n";
}

// qua deve printare i valori separati solo da uno spazio o una virgola, controllare che li stampa con la notazione scientifica
void SIR::print_semplice_virgola(std::vector<State> vettore) {
  std::cout << "      T(giorni)         S             I             R        \n" << std::fixed;

  for (auto const& i : vettore) {
    std::cout << std::right << std::setw(13) << std::setprecision(0) << i.giorno << "," << std::setw(13) << i.suscettibili << "," << std::setw(13)
              << i.infetti << "," << std::setw(13) << i.rimossi << "\n";
  }
}

void SIR::print_semplice_spazio(std::vector<State> vettore) {
  std::cout << "      T(giorni)         S             I             R        \n" << std::fixed;

  for (auto const& i : vettore) {
    std::cout << std::right << std::setw(13) << std::setprecision(0) << i.giorno << " " << std::setw(13) << i.suscettibili << " " << std::setw(13)
              << i.infetti << " " << std::setw(13) << i.rimossi << "\n";
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
  if (!(std::cin >> beta) || beta < 0 || beta > 1) {
    throw std::runtime_error{"Il parametro gamma deve essere un decimale compreso tra 0 e 1"};
  }
  // errore strano se provo ad inserire un carattere, va avanti fino alla fine della funzione

  std::cout << "gamma >> ";
  if (!(std::cin >> gamma) || gamma < 0 || gamma > 1) {
    throw std::runtime_error{"Il parametro gamma deve essere un decimale compreso tra 0 e 1"};
  }

  std::cout << "Ora inserisca il numero iniziale di persone suscettibili nel "
               "nostro campione \n";
  if (!(std::cin >> S_I) || S_I < 0) {
    throw std::runtime_error{"Il numero di soggetti suscettibili deve essere un intero positivo"};
  }

  std::cout << "Ora inserisca il numero iniziale di persone infetti nel nostro "
               "campione \n";
  if (!(std::cin >> I_I) || I_I < 0) {
    throw std::runtime_error{"Il numero di soggetti infetti deve essere un intero positivo"};
  }

  std::cout << "Ora inserisca il numero iniziale di persone rimossi nel nostro "
               "campione \n";
  if (!(std::cin >> R_I) || R_I < 0) {
    throw std::runtime_error{"Il numero di soggetti rimossi deve essere un intero positivo"};
  }

  std::cout << "Ora inserisca la durata del nostro esperimento \n";
  if (!(std::cin >> giorni) || giorni < 0) {
    throw std::runtime_error{"Il numero di soggetti suscettibili deve essere un intero positivo"};
  }

  State s0;

  s0.suscettibili = S_I;
  s0.infetti = I_I;
  s0.rimossi = R_I;
  int NUMERO = S_I + I_I + R_I;
  return {s0, giorni, beta, gamma, NUMERO};
}
}  // namespace epidemia