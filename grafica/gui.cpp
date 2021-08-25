#include "gui.hpp"

void Bordi::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(rettangolo);
}  // metterci anche states altrimenti rompe il casso

Bordi::Bordi(sf::Vector2f dimensione, sf::Vector2f posizione) {
  rettangolo.setSize(dimensione);
  rettangolo.setFillColor(sf::Color::Transparent);
  rettangolo.setOutlineColor(sf::Color::White);
  rettangolo.setOutlineThickness(3);
  rettangolo.setPosition(posizione);
  

  bordo_collisioni.left = posizione.x;
  bordo_collisioni.top = posizione.y;
  bordo_collisioni.width = dimensione.x;  // serve davvero?
  bordo_collisioni.height = dimensione.y;
}

Bordi::Bordi(sf::Vector2f dimensione) {
  rettangolo.setSize(dimensione);
  rettangolo.setFillColor(sf::Color::Transparent);
  rettangolo.setOutlineColor(sf::Color::White);
  rettangolo.setOutlineThickness(3);
  bordo_collisioni.width = dimensione.x;
  bordo_collisioni.height = dimensione.y;
}

void Bordi::set_posizione(sf::Vector2f posizione) {
  rettangolo.setPosition(posizione);

  bordo_collisioni.left = posizione.x;
  bordo_collisioni.top = posizione.y;
}

void Bordi::set_color(sf::Color Colore) { rettangolo.setOutlineColor(Colore); }

sf::FloatRect Bordi::getlimiti() { return bordo_collisioni; };


void Checkbox::draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(box); }

 Checkbox::Checkbox(sf::Vector2f dimensione) {
  box.setSize(dimensione);
  box.setFillColor(sf::Color(128, 128, 128));
  box.setOutlineColor(sf::Color::White);
  box.setOutlineThickness(2);
  box.setOrigin(sf::Vector2f(dimensione.x / 2, dimensione.y / 2));  // metto l'origine al centro
}
void Checkbox::Set_posizione(sf::Vector2f posizione) { box.setPosition(posizione); }
void Checkbox::set_color(sf::Color Colore) { box.setFillColor(Colore); }

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sfondo);
  target.draw(limite);
  target.draw(casella_animazione);
  target.draw(casella_automa);
}

GUI::GUI(sf::Vector2f dimensione)
    : limite(sf::Vector2f(dimensione)), casella_animazione(sf::Vector2f(40, 40)), casella_automa(sf::Vector2f(40, 40)) {
  sfondo.setFillColor(sf::Color(128, 128, 128));  // Grey
  sfondo.setSize(dimensione);

}
void GUI::aggiorna_posizione(sf::Vector2f punto_in_altodx, sf::Vector2f dimensioni_finestra) {  // da usare nel resize
  sf::Vector2f posizione(punto_in_altodx.x, punto_in_altodx.y+ dimensioni_finestra.y / 2 - limite.getlimiti().height / 2); //punta al centro del lato sinistro
  sfondo.setPosition(posizione);
  limite.set_posizione(posizione);
  casella_animazione.Set_posizione(sf::Vector2f(punto_in_altodx.x + limite.getlimiti().width / 2, posizione.y+ limite.getlimiti().height / 6));
  casella_automa.Set_posizione(sf::Vector2f(punto_in_altodx.x +limite.getlimiti().width / 2, posizione.y + limite.getlimiti().height / 3));
}
