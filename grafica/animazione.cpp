#include "animazione.hpp"

#include <SFML/Graphics.hpp>

#include "gui.hpp"

Animazione::Animazione(int t_d_numero_persone, int t_d_infetti_iniziali, int t_d_rimossi_iniziali, float t_d_parametro_beta,
                       float t_d_parametro_gamma)
    : m_limiti(sf::Vector2f(600, 400), sf::Vector2f(100, 100)),
      m_is_stopped{true},
      m_orologi_stanno_andando{false},  // controllare!!!!
      m_d_numero_persone{t_d_numero_persone},
      m_d_infetti_iniziali{t_d_infetti_iniziali},
      m_d_rimossi_iniziali{t_d_rimossi_iniziali},
      m_d_parametro_beta{t_d_parametro_beta},
      m_d_parametro_gamma{t_d_parametro_gamma},
      m_censimento{m_d_numero_persone - m_d_infetti_iniziali - m_d_rimossi_iniziali, m_d_infetti_iniziali, m_d_rimossi_iniziali, 0} {
  if (!m_ominoprova.loadFromFile("uomini.png")) {
    throw std::runtime_error{"texture loading failed"};
  }

  Persona m_prova;

  // Riempio la mappa (m_popolazione) di persone in posizioni e velocità casuali
  for (int i = 0; i < m_d_numero_persone; i++) {
    m_prova.m_raggio = 13.f;
    m_prova.m_centro =
        sf::Vector2f((Casuale() / 100.f) * (m_limiti.GetLimiti().width - 2 * m_prova.m_raggio) + m_limiti.GetLimiti().left + m_prova.m_raggio,
                     (Casuale() / 100.f) * (m_limiti.GetLimiti().height - 2 * m_prova.m_raggio) + m_limiti.GetLimiti().top + m_prova.m_raggio);
    m_prova.m_vel = sf::Vector2f(Casuale() % 50 - 25.f, Casuale() % 50 - 25.f);

    m_prova.m_valore_casuale = (Casuale() / 100.f) * 20;

    m_popolazione[i] = m_prova;
    m_popolazione[i].m_stato = Stato::VULNERABILE;
  }
  for (int i = 0; i < m_d_infetti_iniziali; i++) {
    m_popolazione[static_cast<int>((Casuale() / 100.f) * m_popolazione.size())].m_stato = Stato::INFETTO;
  }
  for (int i = 0; i < m_d_rimossi_iniziali; i++) {
    m_popolazione[static_cast<int>((Casuale() / 100.f) * m_popolazione.size())].m_stato = Stato::GUARITO;
  }

  // Ogni persona è rappresentata da tre vertici
  m_struttura.resize(m_popolazione.size() * 3);
  m_struttura.setPrimitiveType(sf::Triangles);

  SetAllTextures();
  AggiornaGriglia();
}

// Controlla le collisioni e cambia lo stato
void Animazione::Collisione() {
  m_censimento = {0, 0, 0, 0};
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    FaiCensimento(PallinaA, m_censimento);
    if (PallinaA.m_stato == Stato::VULNERABILE) {
      for (long unsigned int j = 0; j < m_popolazione.size(); j++) {
        Persona& PallinaB = m_popolazione[j];
        // Contagiosita' dipende dal parametro beta
        if ((i != j) && (PallinaB.m_stato == Stato::INFETTO)) {
          if (Modulo(PallinaA.m_centro - PallinaB.m_centro) <= 1.5 * PallinaB.m_raggio) {
            if (Casuale() * m_d_parametro_beta < 40) {
              PallinaA.m_stato = Stato::INFETTO;
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

void Animazione::AzzeraOrologiVulnerabili() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    if (PallinaA.m_stato == Stato::VULNERABILE) {
      PallinaA.m_orologio_personale.restart().asSeconds();
      PallinaA.m_tempo_trascorso = PallinaA.m_orologio_personale.getElapsedTime();
    } else {
      /* PallinaA.m_tempo_trascorso += PallinaA.m_orologio_personale.restart();
      std::cout << "infetto " << PallinaA.m_tempo_trascorso.asSeconds() << '\n'; */
      continue;
    }
  }
}

// Al 30 % la persona muore, al 70% guarisce
void Animazione::MortePersona() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    if ((PallinaA.m_stato == Stato::INFETTO)) {
      /* PallinaA.m_tempo_trascorso+=PallinaA.m_orologio_personale.getElapsedTime(); */

      if (PallinaA.m_tempo_trascorso.asSeconds() + PallinaA.m_orologio_personale.getElapsedTime().asSeconds() >
          4 / m_d_parametro_gamma + PallinaA.m_valore_casuale) {
        PallinaA.m_stato = Stato::MORTO;
        if (Casuale() > 30) {
          PallinaA.m_stato = Stato::GUARITO;
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
      if ((i != j) && (PallinaA.m_stato == Stato::INFETTO)) {
        if ((Modulo(PallinaA.m_centro - PallinaB.m_centro) >= PallinaB.m_raggio) &&
            (Modulo(PallinaA.m_centro - PallinaB.m_centro) <= 1.5f * PallinaB.m_raggio)) {

          float estrazione = Casuale() / 100.f ;

          if (estrazione < m_d_parametro_gamma) {
            PallinaA.m_numero_contatti++;
          }
        }
          float estrazione = Casuale() / 100.f;
          std::cout << estrazione << '\n';

          if (estrazione < m_d_parametro_gamma) {
            PallinaA.m_numero_contatti++;
            std::cout << "pallina numero" << i << " "
                      << "aumento contatti\n";
          }
        }
        std::cout << "pallina numero" << i << " " << PallinaA.m_numero_contatti << '\n';

      } else {
        continue;
      }
    }
  }
}

// Al 30 % la persona muore, al 70% guarisce
void Animazione::MortePersona() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    if ((PallinaA.m_stato == Stato::INFETTO) && (PallinaA.m_numero_contatti >= 35)) {
      PallinaA.m_stato = Stato::MORTO;
      if (Casuale() > 30) {
        PallinaA.m_stato = Stato::GUARITO;
      }
    } else {
      continue;
    }
  }
}
 */

// Assegna la texture corretta ad ogni stato
void Animazione::SetAllTextures() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    sf::Vertex* iter = &m_struttura[i * 3];

    switch (PallinaA.m_stato) {
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

Bordi Animazione::GetBordi() { return m_limiti; }

// Assegna una nuova coordinata ad ogni persona
void Animazione::Aggiorna_lista() {
  float deltat2 = m_orologio2.restart().asSeconds();
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    m_popolazione[i].m_centro += m_popolazione[i].m_vel * deltat2;
  }
}

// Implementa le nuove coordinate sulla finestra grafica
void Animazione::AggiornaGriglia() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    sf::Vertex* iter = &m_struttura[i * 3];
    iter[0].position = sf::Vector2f(m_popolazione[i].m_centro.x, m_popolazione[i].m_centro.y - m_popolazione[i].m_raggio);
    iter[1].position = sf::Vector2f(m_popolazione[i].m_centro.x - m_popolazione[i].m_raggio * (1.7f / 2),
                                    m_popolazione[i].m_centro.y + (m_popolazione[i].m_raggio / 2));
    iter[2].position = sf::Vector2f(m_popolazione[i].m_centro.x + m_popolazione[i].m_raggio * (1.7f / 2),
                                    m_popolazione[i].m_centro.y + (m_popolazione[i].m_raggio / 2));
  }
}

// Controlla le collisioni con i bordi e inverte le velocita'
void Animazione::CheckBorders() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    if (m_popolazione[i].m_centro.x < m_limiti.GetLimiti().left + m_popolazione[i].m_raggio ||
        m_popolazione[i].m_centro.x > m_limiti.GetLimiti().left + m_limiti.GetLimiti().width - m_popolazione[i].m_raggio) {
      m_popolazione[i].m_vel.x = -m_popolazione[i].m_vel.x;
    }
    if (m_popolazione[i].m_centro.y < m_limiti.GetLimiti().top + m_popolazione[i].m_raggio ||
        m_popolazione[i].m_centro.y > m_limiti.GetLimiti().top + m_limiti.GetLimiti().height - m_popolazione[i].m_raggio) {
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
    if (PallinaA.m_stato == Stato::INFETTO) {
      std::cout << "tempo trascorso" << PallinaA.m_tempo_trascorso.asSeconds() << '\n';
    }
  }
  m_orologi_stanno_andando = true;
}
void Animazione::StopOrologi() {
  std::cout << "stop animazione'\n";
  ImmagazzinaTempo();
  m_orologi_stanno_andando = false;
}
void Animazione::AzzeraOrologiPersone() {
  for (long unsigned int i = 0; i < m_popolazione.size(); i++) {
    Persona& PallinaA = m_popolazione[i];
    PallinaA.m_orologio_personale.restart();
    PallinaA.m_tempo_trascorso = PallinaA.m_tempo_trascorso + PallinaA.m_orologio_personale.restart();
  }
}

bool Animazione::GetStatusOrologi() { return m_orologi_stanno_andando; }
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
Censimento Animazione::GetCensimento() { return m_censimento; }

// Chiama tutte le funzioni precedenti
void Animazione::Aggiorna_Generale() {
  CheckBorders();

  AzzeraOrologiVulnerabili();

  Collisione();

  MortePersona();

  Aggiorna_lista();

  AggiornaGriglia();

  SetAllTextures();
}