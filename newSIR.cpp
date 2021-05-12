#include "newSIR.hpp"


namespace epidemia {

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
}