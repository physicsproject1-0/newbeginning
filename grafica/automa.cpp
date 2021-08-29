#include "automa.hpp"

// Cellula

Cellula::Cellula(sf::Vector2f m_posizione, sf::Vector2f dimensione) : Bordi(dimensione, m_posizione), m_counter(0) {
  m_rettangolo.setOutlineColor(sf::Color::White);
  m_rettangolo.setOutlineThickness(2.f);
  AggiornaColore();
}

void Cellula::AggiornaColore() {
  switch (m_stato) {
    case (Stato::INFETTO):  // Carichiamo la red texture
      m_rettangolo.setFillColor(sf::Color::Red);
      break;

    case (Stato::GUARITO):  // carichiamo la blue texture
      m_rettangolo.setFillColor(sf::Color::Cyan);
      break;

    case (Stato::VULNERABILE):  // carichiamo la green texture

      m_rettangolo.setFillColor(sf::Color::Green);
      break;

    case (Stato::MORTO):  // carichiamo la white texture
      m_rettangolo.setFillColor(sf::Color::White);
      break;
  }
}

// Automa

Automa::Automa(sf::Vector2f t_posizione, sf::Vector2f t_dimensione, int t_numero, float t_probabilita_contagio, float t_probabilita_guarigione,
               float t_probabilita_morte, int t_infetti, int t_rimossi)
    : m_posizione{t_posizione},
      m_dimensioni{t_dimensione},
      m_numero_lato{t_numero},
      m_censimento{t_numero * t_numero - t_infetti - t_rimossi, t_infetti, t_rimossi, 0},
      m_probabilita_contagio{t_probabilita_contagio},
      m_probabilita_guarigione{t_probabilita_guarigione},
      m_probabilita_morte{t_probabilita_morte},
      m_limite{t_dimensione, t_posizione},
      m_is_stopped{true} {
  if (t_probabilita_guarigione + t_probabilita_morte > 1) {
    throw std::runtime_error("la somma delle probabilità di morire e di guarire deve essere massimo 1");
  }

  float t_lunghezza_x = m_dimensioni.x / m_numero_lato;
  float t_lunghezza_y = m_dimensioni.y / m_numero_lato;

  for (int i = 0; i < m_numero_lato; i++) {
    std::vector<Cellula> t_riga;
    for (int j = 0; j < m_numero_lato; j++) {
      sf::Vector2f m_posizionemovente(m_posizione.x + j * t_lunghezza_x, m_posizione.y + i * t_lunghezza_y);
      Cellula t_riempi(m_posizionemovente, sf::Vector2f(t_lunghezza_x, t_lunghezza_y));

      t_riga.push_back(t_riempi);
    }
    m_grid.push_back(t_riga);
  }
  Genera(t_infetti, t_rimossi);
}

void Automa::draw(sf::RenderTarget& target, sf::RenderStates) const {
  for (int i = 0; i < m_numero_lato; i++) {
    for (int j = 0; j < m_numero_lato; j++) {
      target.draw(m_grid[i][j]);
    }
  }
  target.draw(m_limite);
}

Bordi Automa::GetBordi() { return m_limite; }

// Posiziona randomicamente gli infetti e i rimossi sulla griglia
void Automa::Genera(int t_infetti, int t_rimossi) {
  assert(t_infetti + t_rimossi < m_numero_lato * m_numero_lato);

  for (int a = 0; a < t_infetti; a++) {
    int t_riga = Casuale() % m_numero_lato;
    int t_colonna = Casuale() % m_numero_lato;
    if (m_grid[t_riga][t_colonna].m_stato != Stato::VULNERABILE) {
      a--;
      continue;
    }
    m_grid[t_riga][t_colonna].m_stato = Stato::INFETTO;
    m_grid[t_riga][t_colonna].AggiornaColore();
  }

  for (int a = 0; a < t_rimossi; a++) {
    int t_riga = Casuale() % m_numero_lato;
    int t_colonna = Casuale() % m_numero_lato;
    if (m_grid[t_riga][t_colonna].m_stato != Stato::VULNERABILE) {
      a--;
      continue;
    }
    m_grid[t_riga][t_colonna].m_stato = Stato::GUARITO;
    m_grid[t_riga][t_colonna].AggiornaColore();
  }
}

bool Automa::Esiste(int i, int j) {
  if (i < 0 || i >= m_numero_lato || j < 0 || j >= m_numero_lato) {
    return false;
  }
  return true;
}

// Conteggio degli infetti vicini, sono controllate le otto cellule adiacenti
void Automa::Aggiorna_counter(int i, int j) {
  Cellula& t_cell = m_grid[i][j];

  for (int a = 0; a <= 2; a++) {
    if (Esiste(i - 1, j - 1 + a)) {
      if (m_grid[i - 1][j - 1 + a].m_stato == Stato::INFETTO) {
        t_cell.m_counter++;
      }
    }
    if (Esiste(i + 1, j - 1 + a)) {
      if (m_grid[i + 1][j - 1 + a].m_stato == Stato::INFETTO) {
        t_cell.m_counter++;
      }
    }
  }
  if (Esiste(i, j - 1)) {
    if (m_grid[i][j - 1].m_stato == Stato::INFETTO) {
      t_cell.m_counter++;
    }
  }
  if (Esiste(i, j + 1)) {
    if (m_grid[i][j + 1].m_stato == Stato::INFETTO) {
      t_cell.m_counter++;
    }
  }
}

// Algoritmo per evolvere la griglia
void Automa::Aggiorna() {
  m_censimento = {0, 0, 0, 0};
  for (int i = 0; i < m_numero_lato; i++) {
    for (int j = 0; j < m_numero_lato; j++) {
      Cellula& t_cell = m_grid[i][j];

      if (t_cell.m_stato == Stato::VULNERABILE) {
        int t_esponente = t_cell.m_counter;
        if (t_esponente == 0) {
        } else {
          float t_prob_sano = pow(1 - m_probabilita_contagio, t_esponente);  // probabilità calcolate
          float t_estrazione = Casuale() / 100.f;
          if (t_estrazione > t_prob_sano) {
            t_cell.m_stato = Stato::INFETTO;
            t_cell.AggiornaColore();
          }
        }
      } else if (t_cell.m_stato == Stato::INFETTO) {
        
        float t_estrazione = Casuale() / 100.f;
        if (t_estrazione < m_probabilita_guarigione) {
          t_cell.m_stato = Stato::GUARITO;
        } else if (t_estrazione > m_probabilita_guarigione && t_estrazione < m_probabilita_guarigione + m_probabilita_morte) {
          t_cell.m_stato = Stato::MORTO;
        }
        t_cell.AggiornaColore();
      }
      FaiCensimento(t_cell, m_censimento);
      t_cell.m_counter = 0;
    }
  }
}

// Aggiorna lo stato dell'automa ogni 3 secondi
void Automa::Avanza() {
  if (m_orologio.getElapsedTime().asSeconds() > 3) {
    for (int i = 0; i < m_numero_lato; i++) {
      for (int j = 0; j < m_numero_lato; j++) {
        Aggiorna_counter(i, j);
      }
    }
    Aggiorna();
    m_orologio.restart();
  }
}

std::pair<int, int> Automa::CheckMousePosition(sf::Vector2f t_coordinate_mouse) {
  for (int i = 0; i < m_numero_lato; i++) {
    for (int j = 0; j < m_numero_lato; j++) {
      if (m_grid[i][j].GetLimiti().contains(t_coordinate_mouse)) {
        return std::pair<int, int>{i, j};
      }
    }
  }
  return std::pair<int, int>{-1, -1};
}

// Permette il cambio stato delle singole cellule da parte dell'utente
void Automa::ChangeStatus(std::pair<int, int> t_coordinate, Stato t_stato) { m_grid[t_coordinate.first][t_coordinate.second].m_stato = t_stato; }

void Automa::AzzeraOrologio() { m_orologio.restart(); }

void Automa::StopAutoma() { m_is_stopped = true; }

void Automa::StartAutoma() {
  m_is_stopped = false;
  m_orologio.restart();
}

bool Automa::IsStopped() { return m_is_stopped; }

Censimento Automa::GetCensimento() { return m_censimento; }

void Automa::AggiornaSenzaAvanzare() {
  m_censimento = {0, 0, 0, 0};
  for (int i = 0; i < m_numero_lato; i++) {
    for (int j = 0; j < m_numero_lato; j++) {
      Cellula& cell = m_grid[i][j];
      FaiCensimento(cell, m_censimento);
      cell.m_counter = 0;
      cell.AggiornaColore();
    }
  }
}
