#include<random>

#ifndef STRUCTANDENUM_HPP
#define STRUCTANDENUM_HPP

enum class Vista { Automa, Animazione };

enum class MousePos {
  None,
  CheckboxAnimazione,
  CheckboxAutoma,
  PulsantePlay,
  PulsantePausa,
  PennelloVulnerabile,
  PennelloInfetto,
  PennelloGuariti,
  PennelloMorti
};

enum class Stato { VULNERABILE, INFETTO, MORTO, GUARITO };

struct Censimento {
  int m_suscettibili;
  int m_infetti;
  int m_guariti;
  int m_morti;
};

enum class TipoPulsante { Play, Pausa };

//aggiorna le struct con i dati riguardo le simulazioni in real time
template <typename C>     
void FaiCensimento(C const& t_cell, Censimento& t_popolazione) {
  switch (t_cell.m_stato) {
    case (Stato::VULNERABILE):
      t_popolazione.m_suscettibili++;
      break;

    case (Stato::INFETTO):
      t_popolazione.m_infetti++;
      break;

    case (Stato::GUARITO):
      t_popolazione.m_guariti++;
      break;
      
    case (Stato::MORTO):
      t_popolazione.m_morti++;
      break;
  }
}

//estrae numero tra 1 e 100 compresi
inline int Casuale() {   
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(1, 100);

  return dist(gen);
}

#endif