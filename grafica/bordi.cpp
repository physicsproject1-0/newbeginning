#include "bordi.hpp"

// Bordi

void Bordi::draw(sf::RenderTarget& target, sf::RenderStates) const { target.draw(m_rettangolo); }  // metterci anche states altrimenti rompe il casso

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

void Bordi::ridimensiona(sf::Vector2f dimensione) {
  m_rettangolo.setSize(dimensione);
  m_rettangolo_esterno.width = dimensione.x;
  m_rettangolo_esterno.height = dimensione.y;
}

void Bordi::set_color(sf::Color Colore) { m_rettangolo.setOutlineColor(Colore); }

sf::FloatRect Bordi::getlimiti() const { return m_rettangolo_esterno; };

// Checkbox

Checkbox::Checkbox(sf::Vector2f dimensione) : Bordi(dimensione) {
  m_rettangolo.setOrigin(sf::Vector2f(dimensione.x / 2, dimensione.y / 2));
  m_rettangolo.setFillColor(sf::Color(128, 128, 128));
};

void Checkbox::set_posizione(sf::Vector2f posizione) {
  m_rettangolo.setPosition(posizione);

  m_rettangolo_esterno.left = posizione.x - m_rettangolo_esterno.width / 2;
  m_rettangolo_esterno.top = posizione.y - m_rettangolo_esterno.height / 2;  // aggiustato cambio coordinate
}
bool Checkbox::return_status() { return m_clicked; }
void Checkbox::change_status(bool var) { m_clicked = var; }
void Checkbox::setinterncolor(sf::Color colore) { m_rettangolo.setFillColor(colore); }
void Checkbox::ImpostaOrigine(sf::Vector2f t_nuova_origine) { m_rettangolo.setOrigin(t_nuova_origine); }

// Textbox

Textbox::Textbox(int dimensione_carattere, sf::Font* t_font, sf::Color colore_testo)
    : Bordi(sf::Vector2f(dimensione_carattere, dimensione_carattere)), m_font{t_font}, m_fattore_conversione{0.6} {  // chiamare il costruttore di
                                                                                                                     // bordi
  m_testo.setFillColor(colore_testo);
  m_testo.setFont(*m_font);
  m_testo.setCharacterSize(dimensione_carattere /* 4 */);

  m_rettangolo.setFillColor(sf::Color(211, 211, 211));

  m_rettangolo.setOutlineThickness(1);  // lo spessore più piccolo altrimenti non si vede
}

void Textbox::set_posizione(sf::Vector2f posizione) {
  m_rettangolo.setPosition(posizione);
  m_testo.setPosition(posizione);
}

void Textbox::scrivi(std::string stringa) {
  m_testo.setString(stringa);
  sf::Vector2f dimensioni_testo(m_testo.getGlobalBounds().width + 1,    // fattore di conversione per dimensione x
                                m_testo.getGlobalBounds().height + 3);  // ci sono anche le funzioni getlocal e... ma sono imprecise
  m_testo.setOrigin(dimensioni_testo.x / 2, dimensioni_testo.y / 2);

  ridimensiona(dimensioni_testo);

  m_rettangolo.setOrigin(dimensioni_testo.x / 2, dimensioni_testo.y / 2);
}

void Textbox::draw(sf::RenderTarget& target, sf::RenderStates) const {
  target.draw(m_rettangolo);
  target.draw(m_testo);
}

// Pulsante

Pulsante::Pulsante(float lato, TipoPulsante t_tipopulsante) : Checkbox(sf::Vector2f(lato, lato)), m_tipo_pulsante{t_tipopulsante} {
  m_simbolo.setRadius(lato / 4);
  if (m_tipo_pulsante == TipoPulsante::Play) {
    m_simbolo.setPointCount(3);
    m_simbolo.setRotation(90);  // senso orario
  } else {
    m_simbolo.setPointCount(4);
    m_simbolo.setRotation(-45);
  }
  m_simbolo.setFillColor(sf::Color::Black);
  m_simbolo.setOrigin(sf::Vector2f(lato / 4, lato / 4));
}

void Pulsante::set_posizione(sf::Vector2f posizione) {
  m_rettangolo.setPosition(posizione);
  m_simbolo.setPosition(posizione);

  m_rettangolo_esterno.left = posizione.x - m_rettangolo_esterno.width / 2;
  m_rettangolo_esterno.top = posizione.y - m_rettangolo_esterno.height / 2;
}

void Pulsante::AggiornaStato() {  // se cliccato
}
void Pulsante::draw(sf::RenderTarget& target, sf::RenderStates) const {
  target.draw(m_rettangolo);
  target.draw(m_simbolo);
}
