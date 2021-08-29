#include "bordi.hpp"

// Bordi

Bordi::Bordi(sf::Vector2f t_dimensione, sf::Vector2f t_posizione) {
  m_rettangolo.setSize(t_dimensione);
  m_rettangolo.setFillColor(sf::Color::Transparent);
  m_rettangolo.setOutlineColor(sf::Color::White);
  m_rettangolo.setOutlineThickness(3);
  m_rettangolo.setPosition(t_posizione);

  m_rettangolo_esterno.left = t_posizione.x;
  m_rettangolo_esterno.top = t_posizione.y;
  m_rettangolo_esterno.width = t_dimensione.x;
  m_rettangolo_esterno.height = t_dimensione.y;
}

Bordi::Bordi(sf::Vector2f t_dimensione) {
  m_rettangolo.setSize(t_dimensione);
  m_rettangolo.setFillColor(sf::Color::Transparent);
  m_rettangolo.setOutlineColor(sf::Color::White);
  m_rettangolo.setOutlineThickness(3);
  m_rettangolo_esterno.width = t_dimensione.x;
  m_rettangolo_esterno.height = t_dimensione.y;
}

void Bordi::draw(sf::RenderTarget& target, sf::RenderStates) const { target.draw(m_rettangolo); }

void Bordi::SetPosizione(sf::Vector2f t_posizione) {
  m_rettangolo.setPosition(t_posizione);

  m_rettangolo_esterno.left = t_posizione.x;
  m_rettangolo_esterno.top = t_posizione.y;
}

void Bordi::Ridimensiona(sf::Vector2f t_dimensione) {
  m_rettangolo.setSize(t_dimensione);
  m_rettangolo_esterno.width = t_dimensione.x;
  m_rettangolo_esterno.height = t_dimensione.y;
}

/* void Bordi::set_color(sf::Color Colore) { m_rettangolo.setOutlineColor(Colore); } */

sf::FloatRect Bordi::GetLimiti() const { return m_rettangolo_esterno; };

// Textbox

Textbox::Textbox(int t_dimensione_carattere, sf::Font* t_font, sf::Color t_colore_testo)
    : Bordi(sf::Vector2f(t_dimensione_carattere, t_dimensione_carattere)), m_font{t_font} {  
  m_testo.setFillColor(t_colore_testo);
  m_testo.setFont(*m_font);
  m_testo.setCharacterSize(t_dimensione_carattere);

  m_rettangolo.setFillColor(sf::Color(211, 211, 211));  //grigio scuro

  m_rettangolo.setOutlineThickness(1);  
}

void Textbox::SetPosizione(sf::Vector2f t_posizione) {
  m_rettangolo.setPosition(t_posizione);
  m_testo.setPosition(t_posizione);
}

void Textbox::Scrivi(std::string t_stringa) {
  m_testo.setString(t_stringa);
  sf::Vector2f t_dimensioni_testo(m_testo.getGlobalBounds().width + 1,
                                m_testo.getGlobalBounds().height + 3);  
  m_testo.setOrigin(t_dimensioni_testo.x / 2, t_dimensioni_testo.y / 2);

  Ridimensiona(t_dimensioni_testo);

  m_rettangolo.setOrigin(t_dimensioni_testo.x / 2, t_dimensioni_testo.y / 2);
}

void Textbox::draw(sf::RenderTarget& target, sf::RenderStates) const {
  target.draw(m_rettangolo);
  target.draw(m_testo);
}

// Checkbox

Checkbox::Checkbox(sf::Vector2f t_dimensione) : Bordi(t_dimensione) {
  m_rettangolo.setOrigin(sf::Vector2f(t_dimensione.x / 2, t_dimensione.y / 2));
  m_rettangolo.setFillColor(sf::Color(128, 128, 128));
};

void Checkbox::SetPosizione(sf::Vector2f t_posizione) {
  m_rettangolo.setPosition(t_posizione);

  m_rettangolo_esterno.left = t_posizione.x - m_rettangolo_esterno.width / 2;
  m_rettangolo_esterno.top = t_posizione.y - m_rettangolo_esterno.height / 2;
}

bool Checkbox::ReturnStatus() { return m_clicked; }
void Checkbox::ChangeStatus(bool t_var) { m_clicked = t_var; }
void Checkbox::SetColoreInterno(sf::Color t_colore) { m_rettangolo.setFillColor(t_colore); }
void Checkbox::ImpostaOrigine(sf::Vector2f t_nuova_origine) { m_rettangolo.setOrigin(t_nuova_origine); }

// Pulsante

Pulsante::Pulsante(float t_lato, TipoPulsante t_tipopulsante) : Checkbox(sf::Vector2f(t_lato, t_lato)), m_tipo_pulsante{t_tipopulsante} {
  m_simbolo.setRadius(t_lato / 4);
  if (m_tipo_pulsante == TipoPulsante::Play) {
    m_simbolo.setPointCount(3);
    m_simbolo.setRotation(90);  // senso orario
  } else {
    m_simbolo.setPointCount(4);
    m_simbolo.setRotation(-45);
  }
  m_simbolo.setFillColor(sf::Color::Black);
  m_simbolo.setOrigin(sf::Vector2f(t_lato / 4, t_lato / 4));
}

void Pulsante::draw(sf::RenderTarget& target, sf::RenderStates) const {
  target.draw(m_rettangolo);
  target.draw(m_simbolo);
}

void Pulsante::SetPosizione(sf::Vector2f t_posizione) {
  m_rettangolo.setPosition(t_posizione);
  m_simbolo.setPosition(t_posizione);

  m_rettangolo_esterno.left = t_posizione.x - m_rettangolo_esterno.width / 2;
  m_rettangolo_esterno.top = t_posizione.y - m_rettangolo_esterno.height / 2;
}

