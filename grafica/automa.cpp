#include "automa.hpp"

#include "classe.hpp"
#include "finestra.hpp"
#include "gui.hpp"

void Cellula::Aggiorna_colore() {
  switch (m_S) {
    case (Stato::INFETTO):  // Carichiamo la red texture...
      m_rettangolo.setFillColor(sf::Color::Red);
      break;

    case (Stato::GUARITO):  // carichiamo la white texture
      m_rettangolo.setFillColor(sf::Color::Cyan);
      break;

    case (Stato::VULNERABILE):  // carichiamo la green texture

      m_rettangolo.setFillColor(sf::Color::Green);
      break;
    
    case (Stato::MORTO):
      m_rettangolo.setFillColor(sf::Color::White);
      break;
  }
}

void Automa::Genera(int infette, int rimosse) {
  assert(infette + rimosse < m_numero_lato * m_numero_lato);

  for (int a = 0; a < infette; a++) {
    int riga = Casuale() % m_numero_lato;
    int colonna = Casuale() % m_numero_lato;
    if (m_grid[riga][colonna].m_S != Stato::VULNERABILE) {
      a--;
      continue;
    }
    m_grid[riga][colonna].m_S = Stato::INFETTO;
    m_grid[riga][colonna].Aggiorna_colore();
  }

  for (int a = 0; a < rimosse; a++) {
    int riga = Casuale() % m_numero_lato;
    int colonna = Casuale() % m_numero_lato;
    if (m_grid[riga][colonna].m_S != Stato::VULNERABILE) {
      a--;
      continue;
    }
    m_grid[riga][colonna].m_S = Stato::GUARITO;
    m_grid[riga][colonna].Aggiorna_colore();
  }
}

bool Automa::Esiste(int i, int j) {
  if (i < 0 || i >= m_numero_lato || j < 0 || j >= m_numero_lato) {
    return false;
  }
  return true;
}

void Automa::Aggiorna_counter(int i, int j) {
  Cellula& cell = m_grid[i][j];

  for (int a = 0; a <= 2; a++) {
    if (Esiste(i - 1, j - 1 + a)) {
      if (m_grid[i - 1][j - 1 + a].m_S == Stato::INFETTO) {
        cell.m_counter++;
      }
    }
    if (Esiste(i + 1, j - 1 + a)) {
      if (m_grid[i + 1][j - 1 + a].m_S == Stato::INFETTO) {
        cell.m_counter++;
      }
    }
  }
  if (Esiste(i, j - 1)) {
    if (m_grid[i][j - 1].m_S == Stato::INFETTO) {
      cell.m_counter++;
    }
  }
  if (Esiste(i, j + 1)) {
    if (m_grid[i][j + 1].m_S == Stato::INFETTO) {
      cell.m_counter++;
    }
  }
}

void Automa::Aggiorna() {
  m_giorni++;
  popolazione = {0, 0, 0, 0};
  for (int i = 0; i < m_numero_lato; i++) {
    for (int j = 0; j < m_numero_lato; j++) {
      Cellula& cell = m_grid[i][j];
      /*  censimento(cell); */

      if (cell.m_S == Stato::VULNERABILE) {
        int esponente = cell.m_counter;
        // cell.m_numero.setString(std::to_string(esponente));
        if (esponente == 0) {
          continue;
        } else {
          float prob_sano = pow(1 - m_probabilita_contagio, esponente);  // beta o gamma? beta sicuro

          float estrazione = (Casuale() % 101) / 100.f;  // IL .F è FONDAMENTALE

          if (estrazione > prob_sano) {
            cell.m_S = Stato::INFETTO;

            cell.Aggiorna_colore();
          }
        }
      }

      else if (cell.m_S == Stato::INFETTO) {
        cell.m_infection_days++;
        if (Casuale() / 100.f < m_probabilita_guarigione) {
          {cell.m_S = Stato::MORTO;}
          
          cell.Aggiorna_colore();
          // qua forse ci sta fare così
        }
      }

      cell.m_counter = 0;
    }
  }
}

void Automa::Avanza() {
  if (m_orologio.getElapsedTime().asSeconds() > 3) {
    for (int i = 0; i < m_numero_lato; i++) {
      for (int j = 0; j < m_numero_lato; j++) {
        Aggiorna_counter(i, j);
      }
    }
    Aggiorna();
    // contatore();
    m_orologio.restart();
  }
}

std::pair<int, int> Automa::CheckMousePosition(sf::Vector2f t_coordinate_mouse) {
  for (int i = 0; i < m_numero_lato; i++) {
    for (int j = 0; j < m_numero_lato; j++) {
      if (m_grid[i][j].getlimiti().contains(t_coordinate_mouse)) {
        return std::pair<int, int>{i, j};
      }
    }
  }
  return std::pair<int, int>{-1, -1};
}

void Automa::ChangeStatus(std::pair<int, int> t_coordinate, Stato t_stato) { m_grid[t_coordinate.first][t_coordinate.second].m_S = t_stato; }

void Automa::AzzeraOrologio() { m_orologio.restart(); }

void Automa::StopAutoma() { m_is_stopped = true; }

void Automa::StartAutoma() {
  m_is_stopped = false;
  m_orologio.restart();
}

bool Automa::IsStopped() { return m_is_stopped; }
