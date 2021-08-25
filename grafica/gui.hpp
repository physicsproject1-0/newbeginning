#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#ifndef GUI_HPP
#define GUI_HPP

class Bordi : public sf::Drawable {
  sf::RectangleShape rettangolo;
  sf::FloatRect bordo_collisioni;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 public:
  Bordi(sf::Vector2f dimensione, sf::Vector2f posizione);
  Bordi(sf::Vector2f dimensione) ;
  void set_posizione(sf::Vector2f posizione) ;
  void set_color(sf::Color Colore) ; 
  sf::FloatRect getlimiti();
};

class Checkbox : public sf::Drawable {
  sf::RectangleShape box;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 public:
  Checkbox(sf::Vector2f dimensione) ;
    
  void Set_posizione(sf::Vector2f posizione) ;
  void set_color(sf::Color Colore) ;
};


class GUI : public sf::Drawable {
  sf::RectangleShape sfondo;
  Bordi limite;

  Checkbox casella_animazione;
  Checkbox casella_automa;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 public:
  GUI(sf::Vector2f dimensione);
  void aggiorna_posizione(sf::Vector2f punto_in_altodx, sf::Vector2f dimensioni_finestra);
  
};


#endif