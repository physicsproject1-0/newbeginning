#include "riquadro_informazioni.hpp"

// Plot

Plot::Plot(sf::Vector2f t_dimensione_grafico, sf::Vector2f t_posizione_alto_destra) : m_max_value_y{0} {
  m_sfondo.setFillColor(sf::Color(128, 128, 128));
  m_sfondo.setOutlineColor(sf::Color::White);
  m_sfondo.setOutlineThickness(3.f);
  m_sfondo.setSize(t_dimensione_grafico);
  m_sfondo.setOrigin(sf::Vector2f(t_dimensione_grafico.x, 0));
  m_sfondo.setPosition(t_posizione_alto_destra);

  m_assi.resize(3);
  m_assi.setPrimitiveType(sf::LineStrip);
  AggiornaPosizioni(t_posizione_alto_destra);
  m_assi[0].color = sf::Color::White;
  m_assi[1].color = sf::Color::White;
  m_assi[2].color = sf::Color::White;
}

Plot::Plot(sf::Vector2f t_dimensione_grafico) : m_max_value_y{0} {
  m_sfondo.setFillColor(sf::Color(128, 128, 128));
  m_sfondo.setOutlineColor(sf::Color::White);
  m_sfondo.setOutlineThickness(3.f);
  m_sfondo.setSize(t_dimensione_grafico);
  m_sfondo.setOrigin(sf::Vector2f(t_dimensione_grafico.x, 0));

  m_assi.resize(3);
  m_assi.setPrimitiveType(sf::LineStrip);
  m_assi[0].color = sf::Color::White;
  m_assi[1].color = sf::Color::White;
  m_assi[2].color = sf::Color::White;
}

void Plot::draw(sf::RenderTarget& target, sf::RenderStates) const {
  target.draw(m_sfondo);
  for (long unsigned int i = 0; i < m_lista_linee.size(); i++) {
    target.draw(m_lista_linee[i].first);
  }
  target.draw(m_assi);
}

void Plot::AggiornaPosizioni(sf::Vector2f t_punto_in_alto_destra) {
  m_sfondo.setPosition(t_punto_in_alto_destra);

  m_assi[0].position =
      sf::Vector2f(t_punto_in_alto_destra.x - ((m_sfondo.getSize().x / 10) * 9), t_punto_in_alto_destra.y + (m_sfondo.getSize().y / 10));
  m_assi[1].position =
      sf::Vector2f(t_punto_in_alto_destra.x - ((m_sfondo.getSize().x / 10) * 9), t_punto_in_alto_destra.y + ((m_sfondo.getSize().y / 10) * 9));
  m_assi[2].position =
      sf::Vector2f(t_punto_in_alto_destra.x - (m_sfondo.getSize().x / 10), t_punto_in_alto_destra.y + (m_sfondo.getSize().y / 10) * 9);

  RiscalaPunti();
}

void Plot::NumeroLineee(int t_darmi_il_numero) {
  m_lista_linee.resize(t_darmi_il_numero);
  m_colori_linee.resize(t_darmi_il_numero);
  for (long unsigned int i = 0; i < m_lista_linee.size(); i++) {
    m_lista_linee[i].first.setPrimitiveType(sf::LineStrip);
  }
}

void Plot::DefinisciColoreLinea(int t_linea, sf::Color t_colore) { m_colori_linee[t_linea] = t_colore; }

void Plot::AggiungiPunto(int t_numero_linea, float t_valore) {
  m_lista_linee[t_numero_linea].first.append(
      sf::Vertex(sf::Vector2f(m_lista_linee[t_numero_linea].second.size(), t_valore), m_colori_linee[t_numero_linea]));
  m_lista_linee[t_numero_linea].second.push_back(t_valore);
  m_max_value_y = std::max(m_max_value_y, t_valore);
  RiscalaPunti();
}

void Plot::RiscalaPunti() {
  sf::Vector2f t_posizione_origine = m_assi[1].position;
  for (long unsigned int j = 0; j < m_lista_linee.size(); j++) {
    for (long unsigned int i = 0; i < m_lista_linee[j].second.size(); i++) {
      sf::Vector2f t_posizione_scalata(((m_assi[2].position.x - m_assi[1].position.x) / m_lista_linee[j].second.size()) * i,
                                       ((m_assi[1].position.y - m_assi[0].position.y) / m_max_value_y) * m_lista_linee[j].second[i]);
      m_lista_linee[j].first[i].position = sf::Vector2f(t_posizione_origine.x + t_posizione_scalata.x, t_posizione_origine.y - t_posizione_scalata.y);
    }
  }
}

// Informazioni

Informazioni::Informazioni(sf::Vector2f t_dimensioni, sf::Font* t_font)
    : m_grafico_automa{t_dimensioni},
      m_grafico_animazione{t_dimensioni},
      m_animazione_attiva{true},
      m_automa_attivo{false},
      m_appena_azzerato{false} {
  m_riquadro_esterno.setFillColor(sf::Color(128, 128, 128));
  m_riquadro_esterno.setOutlineColor(sf::Color::White);
  m_riquadro_esterno.setOutlineThickness(3.f);
  m_riquadro_esterno.setSize(t_dimensioni);
  m_riquadro_esterno.setOrigin(sf::Vector2f(t_dimensioni.x, 0));

  m_testo_vulnerabili.setFont(*t_font);
  m_testo_infetti.setFont(*t_font);
  m_testo_guariti.setFont(*t_font);
  m_testo_morti.setFont(*t_font);

  m_testo_vulnerabili.setCharacterSize(10);
  m_testo_infetti.setCharacterSize(10);
  m_testo_guariti.setCharacterSize(10);
  m_testo_morti.setCharacterSize(10);

  m_testo_vulnerabili.setFillColor(sf::Color::Black);
  m_testo_infetti.setFillColor(sf::Color::Black);
  m_testo_guariti.setFillColor(sf::Color::Black);
  m_testo_morti.setFillColor(sf::Color::Black);

  m_testo_vulnerabili.setString("Vulnerabili: ");
  m_testo_infetti.setString("Infetti: ");
  m_testo_guariti.setString("Guariti: ");
  m_testo_morti.setString("Morti: ");

  m_grafico_animazione.NumeroLineee(4);
  m_grafico_animazione.DefinisciColoreLinea(0, sf::Color::Green);
  m_grafico_animazione.DefinisciColoreLinea(1, sf::Color::Red);
  m_grafico_animazione.DefinisciColoreLinea(2, sf::Color::Cyan);
  m_grafico_animazione.DefinisciColoreLinea(3, sf::Color::White);

  m_grafico_automa.NumeroLineee(4);
  m_grafico_automa.DefinisciColoreLinea(0, sf::Color::Green);
  m_grafico_automa.DefinisciColoreLinea(1, sf::Color::Red);
  m_grafico_automa.DefinisciColoreLinea(2, sf::Color::Cyan);
  m_grafico_automa.DefinisciColoreLinea(3, sf::Color::White);
}

void Informazioni::draw(sf::RenderTarget& target, sf::RenderStates) const {
  target.draw(m_riquadro_esterno);

  if (m_automa_attivo) {
    target.draw(m_grafico_automa);

  } else {
    target.draw(m_grafico_animazione);
  }

  target.draw(m_testo_vulnerabili);
  target.draw(m_testo_infetti);
  target.draw(m_testo_guariti);
  target.draw(m_testo_morti);
}

void Informazioni::AggiornaPosizione(sf::Vector2f t_posizione_in_alto_dx) {
  m_riquadro_esterno.setPosition(t_posizione_in_alto_dx);
  m_testo_vulnerabili.setPosition(
      sf::Vector2f(t_posizione_in_alto_dx.x - m_riquadro_esterno.getSize().x + 10, t_posizione_in_alto_dx.y + m_riquadro_esterno.getSize().y / 5));
  m_testo_infetti.setPosition(sf::Vector2f(t_posizione_in_alto_dx.x - m_riquadro_esterno.getSize().x + 10,
                                           t_posizione_in_alto_dx.y + (m_riquadro_esterno.getSize().y / 5) * 2));
  m_testo_guariti.setPosition(sf::Vector2f(t_posizione_in_alto_dx.x - m_riquadro_esterno.getSize().x + 10,
                                           t_posizione_in_alto_dx.y + (m_riquadro_esterno.getSize().y / 5) * 3));
  m_testo_morti.setPosition(sf::Vector2f(t_posizione_in_alto_dx.x - m_riquadro_esterno.getSize().x + 10,
                                         t_posizione_in_alto_dx.y + (m_riquadro_esterno.getSize().y / 5) * 4));
  m_grafico_animazione.AggiornaPosizioni(sf::Vector2f(t_posizione_in_alto_dx.x, t_posizione_in_alto_dx.y + m_riquadro_esterno.getSize().y));
  m_grafico_automa.AggiornaPosizioni(sf::Vector2f(t_posizione_in_alto_dx.x, t_posizione_in_alto_dx.y + m_riquadro_esterno.getSize().y));
}

void Informazioni::AggiungiStatoAutoma(Censimento t_stato_automa) {
  if (m_appena_azzerato) {
    m_successione_stati_automa[0] = t_stato_automa;
    m_grafico_automa.AggiungiPunto(0, t_stato_automa.m_suscettibili);
    m_grafico_automa.AggiungiPunto(1, t_stato_automa.m_infetti);
    m_grafico_automa.AggiungiPunto(2, t_stato_automa.m_guariti);
    m_grafico_automa.AggiungiPunto(3, t_stato_automa.m_morti);
    m_appena_azzerato = false;
  } else {
    m_successione_stati_automa.push_back(t_stato_automa);
    m_grafico_automa.AggiungiPunto(0, t_stato_automa.m_suscettibili);
    m_grafico_automa.AggiungiPunto(1, t_stato_automa.m_infetti);
    m_grafico_automa.AggiungiPunto(2, t_stato_automa.m_guariti);
    m_grafico_automa.AggiungiPunto(3, t_stato_automa.m_morti);
  }
}

void Informazioni::AggiungiStatoAnimazione(Censimento t_stato_animazione) {
  m_successione_stati_animazione.push_back(t_stato_animazione);
  m_grafico_animazione.AggiungiPunto(0, t_stato_animazione.m_suscettibili);
  m_grafico_animazione.AggiungiPunto(1, t_stato_animazione.m_infetti);
  m_grafico_animazione.AggiungiPunto(2, t_stato_animazione.m_guariti);
  m_grafico_animazione.AggiungiPunto(3, t_stato_animazione.m_morti);
}

void Informazioni::MostraDatiAutoma() {
  m_automa_attivo = true;
  m_animazione_attiva = false;
}
void Informazioni::MostraDatiAnimazione() {
  m_automa_attivo = false;
  m_animazione_attiva = true;
}
void Informazioni::AggiornaScritte() {
  if (m_automa_attivo) {
    if (!m_successione_stati_automa.empty()) {
      m_testo_vulnerabili.setString("Vulnerabili: " + std::to_string(m_successione_stati_automa.back().m_suscettibili));
      m_testo_infetti.setString("Infetti: " + std::to_string(m_successione_stati_automa.back().m_infetti));
      m_testo_guariti.setString("Guariti: " + std::to_string(m_successione_stati_automa.back().m_guariti));
      m_testo_morti.setString("Morti: " + std::to_string(m_successione_stati_automa.back().m_morti));
    }
  } else {
    if (!m_successione_stati_animazione.empty()) {
      m_testo_vulnerabili.setString("Vulnerabili: " + std::to_string(m_successione_stati_animazione.back().m_suscettibili));
      m_testo_infetti.setString("Infetti: " + std::to_string(m_successione_stati_animazione.back().m_infetti));
      m_testo_guariti.setString("Guariti: " + std::to_string(m_successione_stati_animazione.back().m_guariti));
      m_testo_morti.setString("Morti: " + std::to_string(m_successione_stati_animazione.back().m_morti));
    }
  }
}
