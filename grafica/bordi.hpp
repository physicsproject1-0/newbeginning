#include <SFML/Graphics.hpp>

#include "struct_enum.hpp"

#ifndef BORDI_HPP
#define BORDI_HPP

class Bordi : public sf::Drawable {
 protected:
  sf::RectangleShape m_rettangolo;
  sf::FloatRect m_rettangolo_esterno;

 public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;
  Bordi(sf::Vector2f dimensione, sf::Vector2f posizione);
  Bordi(sf::Vector2f dimensione);
  virtual void set_posizione(sf::Vector2f posizione);
  void ridimensiona(sf::Vector2f dimensione);
  void set_color(sf::Color Colore);
  sf::FloatRect getlimiti() const;
};

class Checkbox : public Bordi {
 protected:
  bool m_clicked = false;

 public:
  Checkbox(sf::Vector2f dimensione);
  virtual void set_posizione(sf::Vector2f posizione);
  bool return_status();
  void change_status(bool var);
  void setinterncolor(sf::Color colore);
  void ImpostaOrigine(sf::Vector2f t_nuova_origine);
};

class Textbox : public Bordi {
  sf::Font* m_font;
  sf::Text m_testo;
  float m_fattore_conversione;  // dimensioni lettera testo lungo asse x rispetto ad asse y

 public:
  Textbox(int dimensione_carattere, sf::Font* t_font, sf::Color colore_testo);

  void set_posizione(sf::Vector2f posizione);

  void scrivi(std::string stringa);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

class Pulsante : public Checkbox {
  TipoPulsante m_tipo_pulsante;
  sf::CircleShape m_simbolo;

 public:
  Pulsante(float lato, TipoPulsante t_tipopulsante);

  void set_posizione(sf::Vector2f posizione);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif