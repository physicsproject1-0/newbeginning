#include "gui.hpp"

// Bordi

void Bordi::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(m_rettangolo);
}  // metterci anche states altrimenti rompe il casso

Bordi::Bordi(sf::Vector2f dimensione, sf::Vector2f posizione) {
  m_rettangolo.setSize(dimensione);
  m_rettangolo.setFillColor(sf::Color::Transparent);
  m_rettangolo.setOutlineColor(sf::Color::White);
  m_rettangolo.setOutlineThickness(3);
  m_rettangolo.setPosition(posizione);

  m_rettangolo_esterno.left = posizione.x;
  m_rettangolo_esterno.top = posizione.y;
  m_rettangolo_esterno.width = dimensione.x;
  m_rettangolo_esterno.height = dimensione.y;
}

Bordi::Bordi(sf::Vector2f dimensione) {
  m_rettangolo.setSize(dimensione);
  m_rettangolo.setFillColor(sf::Color::Transparent);
  m_rettangolo.setOutlineColor(sf::Color::White);
  m_rettangolo.setOutlineThickness(3);
  m_rettangolo_esterno.width = dimensione.x;
  m_rettangolo_esterno.height = dimensione.y;
}

void Bordi::set_posizione(sf::Vector2f posizione) {
  m_rettangolo.setPosition(posizione);

  m_rettangolo_esterno.left = posizione.x;
  m_rettangolo_esterno.top = posizione.y;
}

void Bordi::set_color(sf::Color Colore) { m_rettangolo.setOutlineColor(Colore); }

sf::FloatRect Bordi::getlimiti() const { return m_rettangolo_esterno; };

// Checkbox

Checkbox::Checkbox(sf::Vector2f dimensione) : Bordi(dimensione) {
  m_rettangolo.setOrigin(sf::Vector2f(dimensione.x / 2, dimensione.y / 2));
  m_rettangolo.setFillColor(sf::Color(128, 128, 128));
};

void Checkbox::setinterncolor(sf::Color colore) { m_rettangolo.setFillColor(colore); }

// GUI

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(m_sfondo_grigio);
  target.draw(m_limiti_sfondo_grigio);
  target.draw(m_casella_animazione);
  target.draw(m_casella_automa);
  target.draw(m_testo_animazione);
  target.draw(m_testo_automa);
  target.draw(m_pulsante_play);
  target.draw(m_pulsante_pausa);
  target.draw(m_paletta_colori);
}

GUI::GUI(sf::Vector2f dimensione)
    : m_limiti_sfondo_grigio(sf::Vector2f(dimensione)),
      m_testo_animazione(10, &m_font, sf::Color::Black),
      m_testo_automa(10, &m_font, sf::Color::Black),
      m_casella_animazione(sf::Vector2f(40, 40)),
      m_casella_automa(sf::Vector2f(40, 40)),
      m_pulsante_play(30, TipoPulsante::Play),
      m_pulsante_pausa(30, TipoPulsante::Pausa),
      m_paletta_colori(sf::Vector2f(dimensione.x / 1.3, dimensione.y*0.4), &m_font), 
      m_posizione_mouse(MousePos::None) {
  if (!m_font.loadFromFile("Arial.ttf")) {
    throw std::runtime_error("font non caricato");
  }

  m_sfondo_grigio.setFillColor(sf::Color(128, 128, 128));  // Grey
  m_sfondo_grigio.setSize(dimensione);

  // la prima vista è animazione, all'inizio la simulazione è ferma?
  m_casella_animazione.change_status(true);
  m_casella_animazione.setinterncolor(sf::Color::Red);
  m_pulsante_pausa.change_status(true);
  m_pulsante_pausa.setinterncolor(sf::Color::Red);

  m_testo_animazione.scrivi("Animazione");
  m_testo_automa.scrivi("Automa");

  m_paletta_colori.Scrivi();

  if (m_testo_animazione.getlimiti().width > m_limiti_sfondo_grigio.getlimiti().width ||
      m_testo_automa.getlimiti().width > m_limiti_sfondo_grigio.getlimiti().width) {
    throw std::runtime_error("la larghezza della GUI non è sufficiente ad ospitare le caselle di testo");
  }
}

void GUI::aggiorna_posizione(sf::Vector2f punto_in_altosx, sf::Vector2f dimensioni_finestra) {  // da usare nel resize
  sf::Vector2f t_posizione_aggancio_alto_sx(
      punto_in_altosx.x,
      punto_in_altosx.y + dimensioni_finestra.y / 2 - m_limiti_sfondo_grigio.getlimiti().height / 2);  // punta al vertice alto sx dello sfondo grigio

  float t_altezza_sfondo_grigio = m_limiti_sfondo_grigio.getlimiti().height;
  float t_larghezza_sfondo_grigio = m_limiti_sfondo_grigio.getlimiti().width;

  m_sfondo_grigio.setPosition(t_posizione_aggancio_alto_sx);
  m_limiti_sfondo_grigio.set_posizione(t_posizione_aggancio_alto_sx);

  m_casella_animazione.set_posizione(
      sf::Vector2f(punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 6));
  m_casella_automa.set_posizione(sf::Vector2f(punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 3));

  m_testo_animazione.set_posizione(
      sf::Vector2f(punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, m_casella_animazione.getlimiti().top - 10));  // riferiti alle checkbox
  m_testo_automa.set_posizione(sf::Vector2f(punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, m_casella_automa.getlimiti().top - 10));

  m_pulsante_play.set_posizione(sf::Vector2f(t_posizione_aggancio_alto_sx.x + t_larghezza_sfondo_grigio / 4, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 2));
  m_pulsante_pausa.set_posizione(
      sf::Vector2f(t_posizione_aggancio_alto_sx.x + (t_larghezza_sfondo_grigio / 4) * 3, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 2));

  m_paletta_colori.ImpostaPosizione(sf::Vector2f(t_posizione_aggancio_alto_sx.x+t_larghezza_sfondo_grigio/2, t_posizione_aggancio_alto_sx.y +(t_altezza_sfondo_grigio/4)*3));
}

double Animazione::Modulo(sf::Vector2f const& vettore) { return sqrt(pow(vettore.x, 2) + pow(vettore.y, 2)); }

int Animazione::Check_occur(Persona const& persona, int raggio) {  // decidere un raggio accettabile
  int occur = 0;
  for (int i = 0; i < m_popolazione.size(); i++) {
    if (Modulo(persona.m_centro - m_popolazione[i].m_centro) <= raggio) {
      occur++;
    }
  }
  return occur;
}