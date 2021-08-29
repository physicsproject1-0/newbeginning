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

template <typename C>
void censimento(C const& cell, Censimento& popolazione) {
  switch (cell.m_S) {
    case (Stato::VULNERABILE):
      popolazione.m_suscettibili++;
      break;

    case (Stato::INFETTO):
      popolazione.m_infetti++;
      break;

    case (Stato::GUARITO):
      popolazione.m_guariti++;
      break;
    case (Stato::MORTO):
      popolazione.m_morti++;
      break;
  }
}

inline int Casuale() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(1, 100);

  return dist(gen);
}

#endif