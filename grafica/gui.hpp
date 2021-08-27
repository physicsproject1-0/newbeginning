#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#ifndef GUI_HPP
#define GUI_HPP

enum class Vista { Automa, Animazione };

enum class MousePos { Checkbox1, Checkbox2, none };

class Bordi : public sf::Drawable {
 protected:
  sf::RectangleShape rettangolo;
  sf::FloatRect bordo_collisioni;

 public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  Bordi(sf::Vector2f dimensione, sf::Vector2f posizione);
  Bordi(sf::Vector2f dimensione);
  virtual void set_posizione(sf::Vector2f posizione);
  void set_color(sf::Color Colore);
  sf::FloatRect getlimiti() const;
};

/* class Checkbox : public sf::Drawable{
    sf::RectangleShape rettangolo;
  sf::FloatRect bordo_collisioni;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 public:
  Checkbox(sf::Vector2f dimensione, sf::Vector2f posizione);
  Checkbox(sf::Vector2f dimensione);
  void set_posizione(sf::Vector2f posizione);
  void set_color(sf::Color Colore);
  sf::FloatRect getlimiti() const;
  void setinterncolor(sf::Color colore);
};

 */

class Checkbox : public Bordi {  //origine al centro
  bool clicked = false;

 public:
  void set_posizione(sf::Vector2f posizione) {
    rettangolo.setPosition(posizione);

    bordo_collisioni.left = posizione.x - bordo_collisioni.width / 2;
    bordo_collisioni.top = posizione.y - bordo_collisioni.height / 2;  // aggiustato cambio coordinate
  }
  bool return_status() { return clicked; }
  void change_status(bool var) { clicked = var; }
  Checkbox(sf::Vector2f dimensione);
  void setinterncolor(sf::Color colore);
};

class Textbox : public Bordi {  //origine al centro //chiamare pprima scrivi e poi set posizione
  sf::Font* font;
  sf::Text testo;

 public:
  Textbox(int dimensione_carattere, sf::Font* t_font) : Bordi(sf::Vector2f(dimensione_carattere,dimensione_carattere)) , font{t_font} {     //chiamare il costruttore di bordi
    testo.setFillColor(sf::Color::White);
    testo.setFont(*font);
    testo.setCharacterSize(dimensione_carattere);

    rettangolo.setFillColor(sf::Color(211, 211, 211));
  }
void set_posizione(sf::Vector2f posizione){
    rettangolo.setPosition(posizione);
    testo.setPosition(posizione);
}

  void scrivi(std::string stringa) {
    testo.setString(stringa);
    sf::Vector2f dimensioni_testo(testo.getCharacterSize() * stringa.size(),testo.getCharacterSize()); //ci sono anche le funzioni getlocal e... ma sono imprecise
    rettangolo.setSize(dimensioni_testo);
    testo.setOrigin(dimensioni_testo.x/2, dimensioni_testo.y/2);
    rettangolo.setOrigin(dimensioni_testo.x/2, dimensioni_testo.y/2);
  }
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(rettangolo);
    target.draw(testo);
  }
};

class GUI : public sf::Drawable {
  sf::Font font;

  sf::RectangleShape sfondo;
  Bordi limite;

  Textbox testo_animazione;
  Textbox testo_automa;

  Checkbox casella_animazione;
  Checkbox casella_automa;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  MousePos posizione;

 public:
  GUI(sf::Vector2f dimensione);
  void aggiorna_posizione(sf::Vector2f punto_in_altodx, sf::Vector2f dimensioni_finestra);

  void check_mouse_position(sf::Vector2f posizione_mouse) {
    if (casella_animazione.getlimiti().contains(posizione_mouse)) {
      posizione = MousePos::Checkbox1;
      std::cout << "sei sopra la casella animazione" << '\n';
    } else if (casella_automa.getlimiti().contains(posizione_mouse)) {
      posizione = MousePos::Checkbox2;
      std::cout << "sei sopra la casella automa" << '\n';

    } else {
      posizione = MousePos::none;
      std::cout << "false" << '\n';  // attenzione agli else!!!!!!!!!!!!!!!!!
    }
  }

  void check_color(sf::Color colore) {
    switch (posizione) {
      case MousePos::Checkbox1:
        if (!casella_animazione.return_status()) {
          casella_animazione.setinterncolor(colore);
          std::cout << "colore animazione cambiato" << '\n';
        }
        break;

      case MousePos::Checkbox2:
        if (!casella_automa.return_status()) {
          casella_automa.setinterncolor(colore);
          std::cout << "colore automa cambiato" << '\n';
        }
        break;
      case MousePos::none:
        if (!casella_automa.return_status()) {
          casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!casella_animazione.return_status()) {
          casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        break;
    }
  }

  MousePos mouse_clicked() {
    if (posizione == MousePos::Checkbox1) {
      casella_animazione.setinterncolor(sf::Color::Red);
      casella_animazione.change_status(true);
      casella_automa.setinterncolor(sf::Color(128, 128, 128));
      casella_automa.change_status(false);
      return posizione;
    }
    if (posizione == MousePos::Checkbox2) {
      casella_animazione.setinterncolor(sf::Color(128, 128, 128));
      casella_animazione.change_status(false);
      casella_automa.setinterncolor(sf::Color::Red);
      casella_automa.change_status(true);
      return posizione;
    } else {
      return posizione;
    }
  }

  void is_out() { check_color(sf::Color(128, 128, 128)); }
};

#endif