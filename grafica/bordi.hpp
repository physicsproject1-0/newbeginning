#include <SFML/Graphics.hpp>

#include "struct_enum.hpp"

#ifndef BORDI_HPP
#define BORDI_HPP

// classe per avere rettangolo e floatrect preciso sopra di lui(non accade con GetGlobalBounds ad esempio)
class Bordi : public sf::Drawable {  //disegna rispetto all'angolo in alto a sinistra
 protected:
  sf::RectangleShape m_rettangolo;
  sf::FloatRect m_rettangolo_esterno;

 public:
  Bordi(sf::Vector2f t_dimensione, sf::Vector2f t_posizione);
  Bordi(sf::Vector2f t_dimensione);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;  
  virtual void SetPosizione(sf::Vector2f t_posizione);

  void Ridimensiona(sf::Vector2f t_dimensione);
  /* void set_color(sf::Color Colore);
   */
  sf::FloatRect GetLimiti() const;
};

// Classe per disegnare caselle di testo
class Textbox : public Bordi {  //disegna rispetto al centro
  sf::Font* m_font;
  sf::Text m_testo;

 public:  
  Textbox(int t_dimensione_carattere, sf::Font* t_font, sf::Color t_colore_testo);  

  void SetPosizione(sf::Vector2f t_posizione);  

  void Scrivi(std::string t_stringa);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

// Classe per disegnare caselle cliccabili
class Checkbox : public Bordi {  //disegna rispetto al centro
 protected:
  bool m_clicked = false;

 public:
  Checkbox(sf::Vector2f t_dimensione);

  virtual void SetPosizione(sf::Vector2f t_posizione);

  bool ReturnStatus();
  void ChangeStatus(bool t_var);
  void SetColoreInterno(sf::Color t_colore);
  void ImpostaOrigine(sf::Vector2f t_nuova_origine);
};

// Classe per disegnare pulsanti con simboli all'interno
class Pulsante : public Checkbox {
  TipoPulsante m_tipo_pulsante;
  sf::CircleShape m_simbolo;

 public:
  Pulsante(float t_lato, TipoPulsante t_tipopulsante);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;

  void SetPosizione(sf::Vector2f t_posizione);
};

#endif