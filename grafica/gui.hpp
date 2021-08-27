#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#ifndef GUI_HPP
#define GUI_HPP

enum class Vista { Automa, Animazione };

enum class MousePos { None, Checkbox1, Checkbox2  };

class Bordi : public sf::Drawable {
 protected:
  sf::RectangleShape m_rettangolo;
  sf::FloatRect m_rettangolo_esterno;

 public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  Bordi(sf::Vector2f dimensione, sf::Vector2f posizione);
  Bordi(sf::Vector2f dimensione);
  virtual void set_posizione(sf::Vector2f posizione);
  void ridimensiona(sf::Vector2f dimensione) {
    m_rettangolo.setSize(dimensione);
    m_rettangolo_esterno.width = dimensione.x;
    m_rettangolo_esterno.height = dimensione.y;
  }
  void set_color(sf::Color Colore);
  sf::FloatRect getlimiti() const;
};


class Checkbox : public Bordi {  // origine al centro
  bool m_clicked = false;

 public:
  void set_posizione(sf::Vector2f posizione) {
    m_rettangolo.setPosition(posizione);

    m_rettangolo_esterno.left = posizione.x - m_rettangolo_esterno.width / 2;
    m_rettangolo_esterno.top = posizione.y - m_rettangolo_esterno.height / 2;  // aggiustato cambio coordinate
  }
  bool return_status() { return m_clicked; }
  void change_status(bool var) { m_clicked = var; }
  Checkbox(sf::Vector2f dimensione);
  void setinterncolor(sf::Color colore);
};

class Textbox : public Bordi {  // origine al centro //chiamare pprima scrivi e poi set posizione
  sf::Font* m_font;
  sf::Text m_testo;
  float m_fattore_conversione; //dimensioni lettera testo lungo asse x rispetto ad asse y

 public:
  Textbox(int dimensione_carattere, sf::Font* t_font, sf::Color colore_testo)
      : Bordi(sf::Vector2f(dimensione_carattere, dimensione_carattere)), m_font{t_font}, m_fattore_conversione{0.6} {  // chiamare il costruttore di bordi
    m_testo.setFillColor(colore_testo);
    m_testo.setFont(*m_font);
    m_testo.setCharacterSize(dimensione_carattere);

    m_rettangolo.setFillColor(sf::Color(211, 211, 211));

    m_rettangolo.setOutlineThickness(1);  // lo spessore più piccolo altrimenti non si vede
  }
  void set_posizione(sf::Vector2f posizione) {
    m_rettangolo.setPosition(posizione);
    m_testo.setPosition(posizione);
  }

  void scrivi(std::string stringa) {
    m_testo.setString(stringa);
    sf::Vector2f dimensioni_testo(m_testo.getCharacterSize() * stringa.size() * m_fattore_conversione ,  //fattore di conversione per dimensione x
                                  m_testo.getCharacterSize());  // ci sono anche le funzioni getlocal e... ma sono imprecise
    ridimensiona(dimensioni_testo);
    m_testo.setOrigin(dimensioni_testo.x / 2, dimensioni_testo.y / 2);
    m_rettangolo.setOrigin(dimensioni_testo.x / 2, dimensioni_testo.y / 2);
  }
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_rettangolo);
    target.draw(m_testo);
  }
};

class Pulsante {};
class GUI : public sf::Drawable {
  sf::Font m_font;

  sf::RectangleShape m_sfondo_grigio;  //non uso nè bordi nè checkbox perchè non adatti
  Bordi m_limiti_sfondo_grigio;

  Textbox m_testo_animazione;
  Textbox m_testo_automa;

  Checkbox m_casella_animazione;
  Checkbox m_casella_automa;

  MousePos m_posizione_mouse;

 public:
  GUI(sf::Vector2f dimensione);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void aggiorna_posizione(sf::Vector2f punto_in_altodx, sf::Vector2f dimensioni_finestra);

  void check_mouse_position(sf::Vector2f t_coordinate_mouse) {
    if (m_casella_animazione.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::Checkbox1;
      std::cout << "sei sopra la casella animazione" << '\n';
    } else if (m_casella_automa.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::Checkbox2;
      std::cout << "sei sopra la casella automa" << '\n';

    } else {
      m_posizione_mouse = MousePos::None;
     /*  std::cout << "false" << '\n'; */  // attenzione agli else!!!!!!!!!!!!!!!!!
    }
  }

  void check_color(sf::Color colore) {
    switch (m_posizione_mouse) {
      case MousePos::Checkbox1:
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(colore);
        }
        break;

      case MousePos::Checkbox2:
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(colore);
        }
        break;
      case MousePos::None:
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        break;
    }
  }

  MousePos mouse_clicked() {
    if (m_posizione_mouse == MousePos::Checkbox1) {
      m_casella_animazione.setinterncolor(sf::Color::Red);
      m_casella_animazione.change_status(true);
      m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
      m_casella_automa.change_status(false);
      return m_posizione_mouse;
    }
    if (m_posizione_mouse == MousePos::Checkbox2) {
      m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
      m_casella_animazione.change_status(false);
      m_casella_automa.setinterncolor(sf::Color::Red);
      m_casella_automa.change_status(true);
      return m_posizione_mouse;
    } else {
      return m_posizione_mouse;
    }
  }

  void is_out() { check_color(sf::Color(128, 128, 128)); }
};

#endif