#include <SFML/Graphics.hpp>

#include "struct_enum.hpp"
/* #include "classe.hpp" */
/* #include "finestra.hpp" */
#include "animazione.hpp"
#include "gui.hpp"

void Animazione::Collisione() {
  popolazione = {0, 0, 0, 0};
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];

    censimento(PallinaA, popolazione);
    if (PallinaA.m_S == Stato::VULNERABILE) {
      for (int j = 0; j < m_popolazione.size(); j++) {
        Persona& PallinaB = m_popolazione[j];

        // Contagiosita' del 40%
        if ((i != j) && (PallinaB.m_S == Stato::INFETTO)) {
          if (Modulo(PallinaA.m_centro - PallinaB.m_centro) <= 1.5 * PallinaB.m_raggio) {
            if (Casuale() * m_d_parametro_beta < 40) {
              PallinaA.m_S = Stato::INFETTO;
            } else {
              continue;
            }
          }
        } else {
          continue;
        }
      }
    } else {
      continue;
    }
  }
}

void Animazione::Conteggio_contatti() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    if (PallinaA.m_S == Stato::VULNERABILE) {
      PallinaA.m_orologio_personale.restart().asSeconds();
      PallinaA.m_tempo_trascorso = PallinaA.m_orologio_personale.getElapsedTime();
    } else {
      /* PallinaA.m_tempo_trascorso += PallinaA.m_orologio_personale.restart();
      std::cout << "infetto " << PallinaA.m_tempo_trascorso.asSeconds() << '\n'; */

      continue;
    }
  }
}
void Animazione::Morte_persona() {  // al 30 % la persona muore, al 70% guarisce
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];

    if ((PallinaA.m_S == Stato::INFETTO)) {
      /* PallinaA.m_tempo_trascorso+=PallinaA.m_orologio_personale.getElapsedTime(); */

      if (PallinaA.m_tempo_trascorso.asSeconds() + PallinaA.m_orologio_personale.getElapsedTime().asSeconds() >
          4 / m_d_parametro_gamma + PallinaA.m_valore_casuale) {
        std::cout << "infetto " << PallinaA.m_tempo_trascorso.asSeconds() + PallinaA.m_orologio_personale.getElapsedTime().asSeconds() << '\n';

        PallinaA.m_S = Stato::MORTO;
        if (Casuale() > 30) {
          PallinaA.m_S = Stato::GUARITO;
        }
      } else {
        continue;
      }
    }
  }
}

/* void Animazione::Conteggio_contatti() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    for (long unsigned int j = 0; j < m_popolazione.size(); j++) {
      Persona& PallinaB = m_popolazione[j];
      if ((i != j) && (PallinaA.m_S == Stato::INFETTO)) {
        if ((Modulo(PallinaA.m_centro - PallinaB.m_centro) >= PallinaB.m_raggio) &&
            (Modulo(PallinaA.m_centro - PallinaB.m_centro) <= 1.5f * PallinaB.m_raggio)) {

          float estrazione = Casuale() / 100.f ;

          if (estrazione < m_d_parametro_gamma) {
            PallinaA.m_numero_contatti++;
          }
        }

      } else {
        continue;
      }
    }
  }
}

void Animazione::Morte_persona() {                                //al 30 % la persona muore, al 70% guarisce
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    if ((PallinaA.m_S == Stato::INFETTO) && (PallinaA.m_numero_contatti >= 35)) {
      PallinaA.m_S = Stato::MORTO;
      if (Casuale() > 30) {
        PallinaA.m_S = Stato::GUARITO;
      }
    } else {
      continue;
    }
  }
}
 */

void Animazione::SetAllTextures() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    sf::Vertex* iter = &m_struttura[i * 3];

    switch (PallinaA.m_S) {
      case Stato::VULNERABILE:

        iter[0].texCoords = sf::Vector2f(110.f, 20.f);  // coordinate pupini verdi
        iter[1].texCoords = sf::Vector2f(20.f, 210.f);
        iter[2].texCoords = sf::Vector2f(205.f, 210.f);
        break;

      case Stato::INFETTO:
        iter[0].texCoords = sf::Vector2f(315.f, 20.f);  // coordinate pupini rossi
        iter[1].texCoords = sf::Vector2f(230.f, 210.f);
        iter[2].texCoords = sf::Vector2f(410.f, 210.f);
        break;

      case Stato::MORTO:
        iter[0].texCoords = sf::Vector2f(520.f, 20.f);  // coordinate pupini grigi
        iter[1].texCoords = sf::Vector2f(430.f, 210.f);
        iter[2].texCoords = sf::Vector2f(615.f, 210.f);
        break;

      case Stato::GUARITO:
        iter[0].texCoords = sf::Vector2f(730.f, 20.f);  // coordinate pupini azzurri
        iter[1].texCoords = sf::Vector2f(635.f, 210.f);
        iter[2].texCoords = sf::Vector2f(825.f, 210.f);
        break;
    }
  }
}

Bordi Animazione::get_bordi() { return m_limiti; }

void Animazione::Aggiorna_griglia() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    sf::Vertex* iter = &m_struttura[i * 3];
    iter[0].position = sf::Vector2f(m_popolazione[i].m_centro.x, m_popolazione[i].m_centro.y - m_popolazione[i].m_raggio);  // strane coord
    iter[1].position = sf::Vector2f(m_popolazione[i].m_centro.x - m_popolazione[i].m_raggio * (1.7f / 2),
                                    m_popolazione[i].m_centro.y + (m_popolazione[i].m_raggio / 2));
    iter[2].position = sf::Vector2f(m_popolazione[i].m_centro.x + m_popolazione[i].m_raggio * (1.7f / 2),
                                    m_popolazione[i].m_centro.y + (m_popolazione[i].m_raggio / 2));
    // bisognerà cancellare il puntatore?
  }
}

void Animazione::Aggiorna_lista() {
  // metto qua perchè se lo chiamo in punti diversi magari sono leggemente diversi
  float deltat2 = m_orologio2.restart().asSeconds();

  // std::cout << "tempo1 " << deltat << '\n';
  // std::cout << "tempo2 " << deltat2 << '\n';
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    m_popolazione[i].m_centro += m_popolazione[i].m_vel * deltat2;
  }
}

void Animazione::Check_borders() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    if (m_popolazione[i].m_centro.x < m_limiti.getlimiti().left + m_popolazione[i].m_raggio ||
        m_popolazione[i].m_centro.x > m_limiti.getlimiti().left + m_limiti.getlimiti().width - m_popolazione[i].m_raggio) {
      m_popolazione[i].m_vel.x = -m_popolazione[i].m_vel.x;
    }
    if (m_popolazione[i].m_centro.y < m_limiti.getlimiti().top + m_popolazione[i].m_raggio ||
        m_popolazione[i].m_centro.y > m_limiti.getlimiti().top + m_limiti.getlimiti().height - m_popolazione[i].m_raggio) {
      m_popolazione[i].m_vel.y = -m_popolazione[i].m_vel.y;
    }
  }
}
void Animazione::AzzeraOrologio() { m_orologio2.restart(); }

void Animazione::StartOrologi() {
  std::cout << "start animazione'\n";

  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    PallinaA.m_orologio_personale.restart();
    if (PallinaA.m_S == Stato::INFETTO) {
      std::cout << "tempo trascorso" << PallinaA.m_tempo_trascorso.asSeconds() << '\n';
    }
  }
  m_orologi_stanno_andando =true;
}
void Animazione::StopOrologi() {
  std::cout << "stop animazione'\n";
  ImmagazzinaTempo();
  m_orologi_stanno_andando =false;
}
void Animazione::AzzeraOrologiPersone() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    PallinaA.m_orologio_personale.restart();
    PallinaA.m_tempo_trascorso = PallinaA.m_tempo_trascorso + PallinaA.m_orologio_personale.restart();
  }
}


bool Animazione::GetStatusOrologi(){
  return m_orologi_stanno_andando;
}
double Animazione::Modulo(sf::Vector2f const& vettore) { return sqrt(pow(vettore.x, 2) + pow(vettore.y, 2)); }

void Animazione::StopAnimazione() {
  /* std::cout << "stop animazione'\n"; */

  /* ImmagazzinaTempo(); */
  m_is_stopped = true;
}

void Animazione::ImmagazzinaTempo() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    PallinaA.m_tempo_trascorso = PallinaA.m_tempo_trascorso + PallinaA.m_orologio_personale.restart();
  }
}

void Animazione::StartAnimazione() {
  m_is_stopped = false;
  m_orologio2.restart();
}

bool Animazione::IsStopped() { return m_is_stopped; }

void Animazione::Aggiorna_Generale() {
  Check_borders();

  Conteggio_contatti();

  Collisione();

  Morte_persona();

  Aggiorna_lista();

  Aggiorna_griglia();

  SetAllTextures();
}