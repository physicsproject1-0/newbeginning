
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <random>

#ifndef GUI_HPP
#define GUI_HPP

enum class Vista { Automa, Animazione };

enum class MousePos { None, CheckboxAnimazione, CheckboxAutoma, PulsantePlay, PulsantePausa };

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
 protected:
  bool m_clicked = false;

 public:
  virtual void set_posizione(sf::Vector2f posizione) {
    m_rettangolo.setPosition(posizione);

    m_rettangolo_esterno.left = posizione.x - m_rettangolo_esterno.width / 2;
    m_rettangolo_esterno.top = posizione.y - m_rettangolo_esterno.height / 2;  // aggiustato cambio coordinate
  }
  bool return_status() { return m_clicked; }
  void change_status(bool var) { m_clicked = var; }
  Checkbox(sf::Vector2f dimensione);
  void setinterncolor(sf::Color colore);
  void ImpostaOrigine(sf::Vector2f t_nuova_origine) { m_rettangolo.setOrigin(t_nuova_origine); }
};

class Textbox : public Bordi {  // origine al centro //chiamare pprima scrivi e poi set posizione
  sf::Font* m_font;
  sf::Text m_testo;
  float m_fattore_conversione;  // dimensioni lettera testo lungo asse x rispetto ad asse y

 public:
  Textbox(int dimensione_carattere, sf::Font* t_font, sf::Color colore_testo)
      : Bordi(sf::Vector2f(dimensione_carattere, dimensione_carattere)), m_font{t_font}, m_fattore_conversione{0.6} {  // chiamare il costruttore di
                                                                                                                       // bordi
    m_testo.setFillColor(colore_testo);
    m_testo.setFont(*m_font);
    m_testo.setCharacterSize(dimensione_carattere /* * 4 */);

    m_rettangolo.setFillColor(sf::Color(211, 211, 211));

    m_rettangolo.setOutlineThickness(1);  // lo spessore più piccolo altrimenti non si vede
  }
  void set_posizione(sf::Vector2f posizione) {
    m_rettangolo.setPosition(posizione);
    m_testo.setPosition(posizione);
  }

  void scrivi(std::string stringa) {
    m_testo.setString(stringa);
    sf::Vector2f dimensioni_testo(
        m_testo.getGlobalBounds().width +
            1 /* m_testo.getCharacterSize() * stringa.size() * m_fattore_conversione */,  // fattore di conversione per dimensione x
        m_testo.getGlobalBounds().height + 3);                                            // ci sono anche le funzioni getlocal e... ma sono imprecise
    m_testo.setOrigin(dimensioni_testo.x / 2, dimensioni_testo.y / 2);
    /* m_testo.scale(0.25, 0.25); */

    std::cout << "width" << m_testo.getGlobalBounds().width << "height" << m_testo.getCharacterSize() << '\n';
    /* sf::Vector2f dimensioni_testo_ridotte(
        m_testo.getGlobalBounds().width +
            4 /* m_testo.getCharacterSize() * stringa.size() * m_fattore_conversione ,  // fattore di conversione per dimensione x
        m_testo.getGlobalBounds().height + 4);
 */
    ridimensiona(dimensioni_testo);

    m_rettangolo.setOrigin(dimensioni_testo.x / 2, dimensioni_testo.y / 2);
  }
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_rettangolo);
    target.draw(m_testo);
  }
};

enum class TipoPulsante { Play, Pausa };

class Pulsante : public Checkbox {  // origine al centro
  TipoPulsante m_tipo_pulsante;
  sf::CircleShape m_simbolo;

 public:
  Pulsante(float lato, TipoPulsante t_tipopulsante) : Checkbox(sf::Vector2f(lato, lato)), m_tipo_pulsante{t_tipopulsante} {
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

  void set_posizione(sf::Vector2f posizione) {
    m_rettangolo.setPosition(posizione);
    m_simbolo.setPosition(posizione);

    m_rettangolo_esterno.left = posizione.x - m_rettangolo_esterno.width / 2;
    m_rettangolo_esterno.top = posizione.y - m_rettangolo_esterno.height / 2;
  }

  void AggiornaStato() {  // se cliccato
  }
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_rettangolo);
    target.draw(m_simbolo);
  }
};

// fare classe da ereditare di cose che si possono attivare o no

class Paletta : public sf::Drawable {
  Checkbox m_box_verde;
  Checkbox m_box_rosso;
  Checkbox m_box_blu;

  Bordi m_riquadro_esterno;

  sf::RectangleShape m_rettangolo_opaco;

  sf::RectangleShape m_dainserire_verde;
  sf::RectangleShape m_dainserire_rosso;
  sf::RectangleShape m_dainserire_blu;

  Textbox m_testo_verde;
  Textbox m_testo_rosso;
  Textbox m_testo_blu;

  bool m_is_active;

 public:
  Paletta(sf::Vector2f t_dimensione_riquadro, sf::Font* t_font)
      : m_box_verde(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_box_rosso(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_box_blu(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_riquadro_esterno(t_dimensione_riquadro),

        m_testo_verde(9.5, t_font, sf::Color::Black),
        m_testo_rosso(9.5, t_font, sf::Color::Black),
        m_testo_blu(9.5, t_font, sf::Color::Black),
        m_is_active{true} {
    m_rettangolo_opaco.setFillColor(sf::Color(128, 128, 128, 120));
    m_rettangolo_opaco.setSize(t_dimensione_riquadro);
    m_rettangolo_opaco.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.y / 2));

    m_box_verde.setinterncolor(sf::Color::Green);
    m_box_verde.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_box_rosso.setinterncolor(sf::Color::Red);
    m_box_rosso.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_box_blu.setinterncolor(sf::Color::Cyan);
    m_box_blu.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_dainserire_verde.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_verde.setFillColor(sf::Color::Green);
    m_dainserire_verde.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));

    m_dainserire_rosso.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_rosso.setFillColor(sf::Color::Red);
    m_dainserire_rosso.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));

    m_dainserire_blu.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_blu.setFillColor(sf::Color::Cyan);
    m_dainserire_blu.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));
  }

  void ImpostaPosizione(sf::Vector2f t_posizione_centro) {  // la consideriamo rispetto al centro
    m_riquadro_esterno.set_posizione(t_posizione_centro -
                                     sf::Vector2f(m_riquadro_esterno.getlimiti().width / 2, m_riquadro_esterno.getlimiti().height / 2));

    m_rettangolo_opaco.setPosition(t_posizione_centro);

    m_box_verde.set_posizione(sf::Vector2f(t_posizione_centro.x, t_posizione_centro.y - m_riquadro_esterno.getlimiti().height / 3));
    m_box_rosso.set_posizione(sf::Vector2f(t_posizione_centro.x, t_posizione_centro.y));
    m_box_blu.set_posizione(sf::Vector2f(t_posizione_centro.x, t_posizione_centro.y + m_riquadro_esterno.getlimiti().height / 3));

    m_testo_verde.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_verde.getlimiti().top - 10));
    m_testo_rosso.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_rosso.getlimiti().top - 10));
    m_testo_blu.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_blu.getlimiti().top - 10));
  }
  void Scrivi() {  // funzione presente altrimenti scrive prima di caricare il font e mi restituisce 0 di larghezza , il loading del font avviene dopo
                   // in GUI
    m_testo_verde.scrivi("Vulnerabile");
    m_testo_rosso.scrivi("Infetto");
    m_testo_blu.scrivi("Rimosso");
    if (m_testo_verde.getlimiti().width > m_riquadro_esterno.getlimiti().width ||
        m_testo_rosso.getlimiti().width > m_riquadro_esterno.getlimiti().width ||
        m_testo_blu.getlimiti().width > m_riquadro_esterno.getlimiti().width) {
      throw std::runtime_error("dimensione paletta non sufficiente per ospitare testo");
    }
  }
  void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_box_verde);
    target.draw(m_box_rosso);
    target.draw(m_box_blu);
    target.draw(m_riquadro_esterno);
    target.draw(m_testo_verde);
    target.draw(m_testo_rosso);
    target.draw(m_testo_blu);
    if (!m_is_active) {
      target.draw(m_rettangolo_opaco);
    }
  }

  bool IsActive() { return m_is_active; }

  void Attiva() { m_is_active = true; }

  void Disattiva() { m_is_active = false; }

  void PrendiVerde() {}

  void PrendiRosso() {}
  void PrendiGiallo() {}
  /*  bool Contiene(sf::Vector2f t_coordinate_mouse, MousePos& t_posizione_mouse) {
     if (m_box_verde.getlimiti().contains(t_coordinate_mouse)) {
       m_posizione_mouse = MousePos::CheckboxAnimazione;
       std::cout << "sei sopra la casella animazione" << '\n';

     } else if (m_box_rosso.contains(t_coordinate_mouse)) {
       m_posizione_mouse = MousePos::CheckboxAutoma;
       std::cout << "sei sopra la casella automa" << '\n';

     } else if (m_box_blu.contains(t_coordinate_mouse)) {
       m_posizione_mouse = MousePos::PulsantePlay;
       std::cout << "sei sopra la casella play" << '\n';

     } else if (m_riquadro_esterno.contains(t_coordinate_mouse)) {
       m_posizione_mouse = MousePos::PulsantePausa;
       std::cout << "sei sopra la casella pausa" << '\n';

     } else {
       m_posizione_mouse = MousePos::None;
       std::cout << "nope" << '\n';
     }
   } */
};
/*
sf::Vector2f operator/(const int& t_numero) {}   come si overloada???
 */

class Plot : public sf::Drawable {
  sf::RectangleShape m_sfondo;
  std::vector<std::pair<sf::VertexArray, std::vector<sf::Vector2f>>> m_lista_linee;
  std::vector<sf::Color> m_colori_linee;
  float m_max_value_y;

  sf::VertexArray m_assi;

 public:
  Plot(sf::Vector2f t_dimensione_grafico, sf::Vector2f t_posizione_alto_destra) : m_max_value_y{0} {
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
  void AggiornaPosizioni(sf::Vector2f t_punto_in_alto_destra) {
    m_sfondo.setPosition(t_punto_in_alto_destra);

    m_assi[0].position =
        sf::Vector2f(t_punto_in_alto_destra.x - (m_sfondo.getPosition().x / 10) * 9, t_punto_in_alto_destra.y + (m_sfondo.getPosition().x / 10));
    m_assi[1].position =
        sf::Vector2f(t_punto_in_alto_destra.x - (m_sfondo.getPosition().x / 10) * 9, t_punto_in_alto_destra.y + (m_sfondo.getPosition().x / 10) * 9);
    m_assi[2].position =
        sf::Vector2f(t_punto_in_alto_destra.x - (m_sfondo.getPosition().x / 10), t_punto_in_alto_destra.y + (m_sfondo.getPosition().x / 10) * 9);
    RiscalaPunti();
  }

  void NumeroLineee(int darmi_il_numero) {
    m_lista_linee.resize(darmi_il_numero);
    m_colori_linee.resize(darmi_il_numero);
  }

  void DefinisciColoreLinea(int t_linea, sf::Color t_colore) { m_colori_linee[t_linea] = t_colore; }

  void AggiungiPunto(int numero_linea, sf::Vector2f t_valore) {
    m_lista_linee[numero_linea].first.append(sf::Vertex(t_valore, m_colori_linee[numero_linea]));
    m_lista_linee[numero_linea].second.push_back(t_valore);
    m_max_value_y = std::max(m_max_value_y, t_valore.y);
    RiscalaPunti();
  }

  void RiscalaPunti() {
    sf::Vector2f t_posizione_origine = m_assi[1].position;
    for (int j = 0; j < m_lista_linee.size(); j++) {
      for (int i = 0; i < m_lista_linee[j].second.size(); i++) {
        sf::Vector2f t_posizione_scalata(((m_assi[1].position.x - m_assi[2].position.x) / m_lista_linee[j].second.size()) * i,
                                         ((m_assi[0].position.y - m_assi[1].position.y) / m_max_value_y) * m_lista_linee[j].second[i].y);
        m_lista_linee[j].first[i].position =
            sf::Vector2f(t_posizione_origine.x + t_posizione_scalata.x, t_posizione_origine.y - t_posizione_scalata.y);
      }
    }
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sfondo);
    for (int i = 0; i < m_lista_linee.size(); i++) {
      target.draw(m_lista_linee[i].first);
    }
    target.draw(m_assi);
  }
};

class Informazioni : public sf::Drawable {};

class GUI : public sf::Drawable {
  sf::Font m_font;

  sf::RectangleShape m_sfondo_grigio;  // non uso nè bordi nè checkbox perchè non adatti
  Bordi m_limiti_sfondo_grigio;

  Textbox m_testo_animazione;
  Textbox m_testo_automa;

  Checkbox m_casella_animazione;
  Checkbox m_casella_automa;

  Pulsante m_pulsante_play;
  Pulsante m_pulsante_pausa;

  Paletta m_paletta_colori;

  MousePos m_posizione_mouse;

 public:
  GUI(sf::Vector2f dimensione);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void aggiorna_posizione(sf::Vector2f punto_in_altodx, sf::Vector2f dimensioni_finestra);

  void AttivaInserimento() { m_paletta_colori.Attiva(); }

  void DisattivaInserimento() { m_paletta_colori.Disattiva(); }

  void check_mouse_position(sf::Vector2f t_coordinate_mouse) {
    if (m_casella_animazione.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::CheckboxAnimazione;
      std::cout << "sei sopra la casella animazione" << '\n';

    } else if (m_casella_automa.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::CheckboxAutoma;
      std::cout << "sei sopra la casella automa" << '\n';

    } else if (m_pulsante_play.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::PulsantePlay;
      std::cout << "sei sopra la casella play" << '\n';

    } else if (m_pulsante_pausa.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::PulsantePausa;
      std::cout << "sei sopra la casella pausa" << '\n';

    } else {
      m_posizione_mouse = MousePos::None;
      std::cout << "nope" << '\n';
    }
  }

  void check_color(sf::Color colore) {
    switch (m_posizione_mouse) {
      case MousePos::CheckboxAnimazione:
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(colore);
        }
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }

        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
        }

        break;

      case MousePos::CheckboxAutoma:
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(colore);
        }
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
        }

        break;
      case MousePos::PulsantePlay:
        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(colore);
        }
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
        }

        break;
      case MousePos::PulsantePausa:
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(colore);
        }
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
        }

        break;

      case MousePos::None:
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
        }
        break;
    }
  }

  /*  void ControlloGiaCliccata(Checkbox t_checkbox) {
     if (!m_casella_automa.return_status()) {
           m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
         }
   } */

  MousePos mouse_clicked() {
    if (m_posizione_mouse == MousePos::CheckboxAnimazione) {
      m_casella_animazione.setinterncolor(sf::Color::Red);
      m_casella_animazione.change_status(true);
      m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
      m_casella_automa.change_status(false);
      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::CheckboxAutoma) {
      m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
      m_casella_animazione.change_status(false);
      m_casella_automa.setinterncolor(sf::Color::Red);
      m_casella_automa.change_status(true);
      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::PulsantePlay) {
      m_pulsante_play.setinterncolor(sf::Color::Red);
      m_pulsante_play.change_status(true);
      m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
      m_pulsante_pausa.change_status(false);
      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::PulsantePausa) {
      m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
      m_pulsante_play.change_status(false);
      m_pulsante_pausa.setinterncolor(sf::Color::Red);
      m_pulsante_pausa.change_status(true);
      return m_posizione_mouse;

    } else {
      return m_posizione_mouse;
    }
  }

  void is_out() { check_color(sf::Color(128, 128, 128)); }

  bool cliccabile() {
    if (m_posizione_mouse != MousePos::None) {
      return true;
    } else {
      return false;
    }
  }
};

// QUI INIZIA LA PARTE SIMULAZIONE   ################################################

// QUI INIZIA LA PARTE SIMULAZIONE   ################################################

// QUI INIZIA LA PARTE SIMULAZIONE   ################################################

// QUI INIZIA LA PARTE SIMULAZIONE   ################################################

// QUI INIZIA LA PARTE SIMULAZIONE   ################################################

// QUI INIZIA LA PARTE SIMULAZIONE   ################################################

// QUI INIZIA LA PARTE SIMULAZIONE   ################################################

enum class Stato { VULNERABILE, INFETTO, RIMOSSO, MORTO, GUARITO };

struct Censimento {
  int m_suscettibili;
  int m_infetti;
  int m_guariti;
  int m_morti;
};

int Casuale();


/* struct Persona {
  sf::Vector2f m_centro;
  float m_raggio;
  sf::Vector2f m_vel;
             //sf::Clock m_cambiom_velocita;   senza collisioni non serve più giusto???
  Stato m_S;
  bool checked = false;
  int m_numero_contatti = 0;
};

class Animazione : public sf::Drawable {
  sf::Clock m_orologio2;
  sf::Texture m_ominoprova;
  sf::VertexArray m_struttura;
  std::map<int, Persona> m_popolazione;  // vedere se meglio vector

  bool m_is_stopped;

  Bordi m_limiti;

  Censimento m_burocrazia;

  // Nel momento in cui collidono due persone, se una era infetta, cambia lo stato anche dell' altra
  void Collisione() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      Persona& PallinaA = m_popolazione[i];
      if (PallinaA.m_S == Stato::VULNERABILE) {
        for (int j = 0; j < m_popolazione.size(); j++) {
          Persona& PallinaB = m_popolazione[j];

          // Contagiosita' del 30%
          if ((i != j) && (PallinaB.m_S == Stato::INFETTO)) {
            if (Modulo(PallinaA.m_centro - PallinaB.m_centro) <= 1.5 * PallinaB.m_raggio) {
              srand(time(NULL));
              int a = rand() % 100 + 1;
              if (a < 30) {
                PallinaA.m_S = Stato::INFETTO;
                SetRedTextures();
              } else {
                continue;
              }
            }
          } else {
            continue;
          }
        }
      } else {
        continue;
      }
    }
  }

  void Conteggio_contatti() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      Persona& PallinaA = m_popolazione[i];
      for (int j = 0; j < m_popolazione.size(); j++) {
        Persona& PallinaB = m_popolazione[j];
        if ((i != j) && (PallinaA.m_S == Stato::INFETTO)) {
          if ((Modulo(PallinaA.m_centro - PallinaB.m_centro) >= 1.49f * PallinaB.m_raggio) &&
              (Modulo(PallinaA.m_centro - PallinaB.m_centro) <= 1.51f * PallinaB.m_raggio)) {
            PallinaA.m_numero_contatti++;
          }
        } else {
          continue;
        }
      }
    }
  }

  void Morte_persona() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      Persona& PallinaA = m_popolazione[i];
      if ((PallinaA.m_S == Stato::INFETTO) && (PallinaA.m_numero_contatti == 35)) {
        PallinaA.m_S = Stato::RIMOSSO;
        SetWhiteTextures();
      } else {
        continue;
      }
    }
  }

  // Funzione con cui carico la Texture verde
  void SetGreenTextures() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      sf::Vertex* iter = &m_struttura[i * 3];

      iter[0].texCoords = sf::Vector2f(110.f, 20.f);  // coordinate pupini verdi
      iter[1].texCoords = sf::Vector2f(20.f, 210.f);
      iter[2].texCoords = sf::Vector2f(205.f, 210.f);
    }
  }

  // Funzione in cui carico la Texture rossa sullo stato INFETTO
  void SetRedTextures() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      if (m_popolazione[i].m_S == Stato::INFETTO) {
        sf::Vertex* iter = &m_struttura[i * 3];

        iter[0].texCoords = sf::Vector2f(315.f, 20.f);  // coordinate pupini rossi
        iter[1].texCoords = sf::Vector2f(230.f, 210.f);
        iter[2].texCoords = sf::Vector2f(410.f, 210.f);

      } else {
        continue;
      }
    }
  }

  // Ho immaginato che al 30% muoiano e al 70% guariscono, si possono cambiare le probabilita' of course
  // Funzione in cui carico sullo stato RIMOSSO al 30% la texture grigia e al 70% quella azzurra
  void SetWhiteTextures() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      Persona& PallinaA = m_popolazione[i];
      if (PallinaA.m_S == Stato::RIMOSSO) {  // Aggiungere stato guarito/morto
        srand(time(NULL));
        int a = rand() % 100 + 1;
        if (a < 30) {
          PallinaA.m_S = Stato::MORTO;
          sf::Vertex* iter = &m_struttura[i * 3];
          iter[0].texCoords = sf::Vector2f(520.f, 20.f);  // coordinate pupini grigi
          iter[1].texCoords = sf::Vector2f(430.f, 210.f);
          iter[2].texCoords = sf::Vector2f(615.f, 210.f);
        } else {
          PallinaA.m_S = Stato::GUARITO;
          sf::Vertex* iter = &m_struttura[i * 3];
          iter[0].texCoords = sf::Vector2f(730.f, 20.f);  // coordinate pupini azzurri
          iter[1].texCoords = sf::Vector2f(635.f, 210.f);
          iter[2].texCoords = sf::Vector2f(825.f, 210.f);
        }
      } else {
        continue;
      }
    }
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_ominoprova;
    // capire che madonna succede qui dentro!!!
    target.draw(m_struttura, states);
    target.draw(m_limiti);
  }

 public:
  Animazione(int n) : m_limiti(sf::Vector2f(600, 400), sf::Vector2f(100, 100)), m_is_stopped{true} {
    if (!m_ominoprova.loadFromFile("uomini.png")) {
      throw std::runtime_error{"texture loading failed"};  // catcharlo
    }

    Persona m_prova;

    srand(time(NULL));

    // Riempio la mappa (m_popolazione) di persone
    for (int i = 0; i < n; i++) {
      m_prova.m_raggio = 13.f;
      m_prova.m_centro =
          sf::Vector2f(rand() % static_cast<int>(m_limiti.getlimiti().width - 2 * m_prova.m_raggio) + m_limiti.getlimiti().left + m_prova.m_raggio,
                       rand() % static_cast<int>(m_limiti.getlimiti().height - 2 * m_prova.m_raggio) + m_limiti.getlimiti().top + m_prova.m_raggio);
      m_prova.m_vel = sf::Vector2f(rand() % 50 - 25.f, rand() % 50 - 25.f);
      m_popolazione[i] = m_prova;
      m_popolazione[i].m_S = Stato::VULNERABILE;
    }

    m_popolazione[rand() % m_popolazione.size() + 1].m_S = Stato::INFETTO;

    m_struttura.resize(m_popolazione.size() * 3);

    m_struttura.setPrimitiveType(sf::Triangles);

    SetGreenTextures();
    SetRedTextures();
    Aggiorna_griglia();  // chiamarlo almeno una volta sennò no good;
  }

  Bordi get_bordi() { return m_limiti; }

  void Aggiorna_griglia() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      sf::Vertex* iter = &m_struttura[i * 3];
      iter[0].position = sf::Vector2f(m_popolazione[i].m_centro.x, m_popolazione[i].m_centro.y - m_popolazione[i].m_raggio);  // strane coord
      iter[1].position = sf::Vector2f(m_popolazione[i].m_centro.x - m_popolazione[i].m_raggio * (1.7f / 2),
                                      m_popolazione[i].m_centro.y + (m_popolazione[i].m_raggio / 2));
      iter[2].position = sf::Vector2f(m_popolazione[i].m_centro.x + m_popolazione[i].m_raggio * (1.7f / 2),
                                      m_popolazione[i].m_centro.y + (m_popolazione[i].m_raggio / 2));
      // bisognerà cancellare il puntatore?
    }
  }

  // void check_collisions();  // devo passare un puntatore all'm_orologio per averne solo uno  //pensarci

  void Aggiorna_lista() {
    // metto qua perchè se lo chiamo in punti diversi magari sono leggemente diversi
    float deltat2 = m_orologio2.restart().asSeconds();

    // std::cout << "tempo1 " << deltat << '\n';
    // std::cout << "tempo2 " << deltat2 << '\n';
    for (int i = 0; i < m_popolazione.size(); i++) {
      m_popolazione[i].m_centro += m_popolazione[i].m_vel * deltat2;
    }
  }

  void Check_borders() {
    for (int i = 0; i < m_popolazione.size(); i++) {
      if (m_popolazione[i].m_centro.x < m_limiti.getlimiti().left + m_popolazione[i].m_raggio ||
          m_popolazione[i].m_centro.x > m_limiti.getlimiti().left + m_limiti.getlimiti().width - m_popolazione[i].m_raggio) {
        m_popolazione[i].m_vel.x = -m_popolazione[i].m_vel.x;
      }
      if (m_popolazione[i].m_centro.y < m_limiti.getlimiti().top + m_popolazione[i].m_raggio ||
          m_popolazione[i].m_centro.y > m_limiti.getlimiti().top + m_limiti.getlimiti().height - m_popolazione[i].m_raggio) {
        m_popolazione[i].m_vel.y = -m_popolazione[i].m_vel.y;
      }
    }
  }

  void AzzeraOrologio() { m_orologio2.restart(); }

  //void Check_external_bounds(Persona& test);

  //void change_Stato();  // Fa cambiare la texture nel momento in cui le due particelle si scontrano

  //void change_m_vel();

  int Check_occur(Persona const& persona, int m_raggio);

  double Modulo(sf::Vector2f const& vettore);

  void StopAnimazione() { m_is_stopped = true; }

  void StartAnimazione() {
    m_is_stopped = false;
    m_orologio2.restart();
  }

  bool IsStopped() { return m_is_stopped; }

  void Aggiorna_Generale() {
    Check_borders();

    Conteggio_contatti();

    Collisione();

    Morte_persona();

    Aggiorna_lista();

    Aggiorna_griglia();
  }
};
 */
// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

// QUI INIZIA LA PARTE AUTOMA   ################################################

/* class Cellula : public Bordi {  // se è una struct non funziona l'inheritance?
                                // metterci anche states altrimenti rompe il casso
                                // non posso loadare un font qui dentro direttamente

 public:
  // wtf

  void Aggiorna_colore() {
    switch (m_S) {
      case (Stato::INFETTO):  // Carichiamo la red texture...
        m_rettangolo.setFillColor(sf::Color::Red);
        std::cout << "sto Aggiornando il rosso" << '\n';

        break;

      case (Stato::RIMOSSO):  // carichiamo la white texture
        m_rettangolo.setFillColor(sf::Color::Cyan);
        break;

      case (Stato::VULNERABILE):  // carichiamo la green texture

        m_rettangolo.setFillColor(sf::Color::Green);
        break;
    }
  }

  int m_counter;

  int m_infection_days;

  Stato m_S = Stato::VULNERABILE;

  Cellula(sf::Vector2f m_posizione, sf::Vector2f dimensione) : Bordi(dimensione, m_posizione), m_counter(0), m_infection_days(0) {  // funzionerà

    m_rettangolo.setOutlineColor(sf::Color::White);
    m_rettangolo.setOutlineThickness(2.f);
    Aggiorna_colore();
  }
};

class Automa : public sf::Drawable {  // ESTRARRE LE CLASSI NESTATE E DISTINGUERE I MORTI DAI GUARITI
  // sf::Font font;

  // mettere errori per m_dimensioni minori di 0?
  std::vector<std::vector<Cellula>> m_grid;

  sf::Vector2f m_posizione;

  sf::Vector2f m_dimensioni;

  sf::Clock m_orologio;

  int m_numero_lato;

  float m_probabilita_contagio;

  float m_probabilita_guarigione;

  Bordi limiti;

  int m_giorni = 0;

  Censimento popolazione;

  bool m_is_stopped;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int i = 0; i < m_numero_lato; i++) {
      for (int j = 0; j < m_numero_lato; j++) {
        target.draw(m_grid[i][j]);
      }
    }
    target.draw(limiti);
  }

 public:
  Automa(sf::Vector2f t_posizione, sf::Vector2f t_dimensione, int t_numero, float t_probabilita_contagio, float t_probabilita_guarigione, int infetti,
         int rimossi)
      : m_posizione{t_posizione},
        m_dimensioni{t_dimensione},
        m_numero_lato{t_numero},
        m_probabilita_contagio{t_probabilita_contagio},
        m_probabilita_guarigione{t_probabilita_guarigione},
        limiti{t_dimensione, t_posizione},
        m_is_stopped{true} {
    assert(m_probabilita_contagio <= 1 && m_probabilita_contagio >= 0);  // mettere except
    /* if (!font.loadFromFile("Arial.ttf")) {
      throw std::runtime_error{"texture loading failed"};
    } */
/*  try {
   if (!font.loadFromFile("Arial.ttf")) {
     throw std::runtime_error{"denominator is zero"};
   }
 } catch (std::runtime_error const& e) {
   std::cerr << e.what() << '\n';  //
 } */

/*

float t_lunghezza_x = m_dimensioni.x / m_numero_lato;

float t_lunghezza_y = m_dimensioni.y / m_numero_lato;

for (int i = 0; i < m_numero_lato; i++) {
 std::vector<Cellula> riga;
 for (int j = 0; j < m_numero_lato; j++) {
   sf::Vector2f m_posizionemovente(m_posizione.x + j * t_lunghezza_x, m_posizione.y + i * t_lunghezza_y);
   Cellula riempi(m_posizionemovente, sf::Vector2f(t_lunghezza_x, t_lunghezza_y) /* , &font );

  /* riga.push_back(riempi);

   // i per le righe, j per le colonne
 }
 m_grid.push_back(riga);
}
Genera(infetti, rimossi);
}

Bordi get_bordi() { return limiti; }

void Genera(int infette, int rimosse) {
assert(infette + rimosse < m_numero_lato * m_numero_lato);
srand(time(NULL));

for (int a = 0; a < infette; a++) {
 int riga = rand() % m_numero_lato;
 int colonna = rand() % m_numero_lato;
 if (m_grid[riga][colonna].m_S != Stato::VULNERABILE) {
   a--;
   continue;
 }
 m_grid[riga][colonna].m_S = Stato::INFETTO;
 m_grid[riga][colonna].Aggiorna_colore();
}

for (int a = 0; a < rimosse; a++) {
 int riga = rand() % m_numero_lato;
 int colonna = rand() % m_numero_lato;
 if (m_grid[riga][colonna].m_S != Stato::VULNERABILE) {
   a--;
   continue;
 }
 m_grid[riga][colonna].m_S = Stato::RIMOSSO;
 m_grid[riga][colonna].Aggiorna_colore();
}
}

bool Esiste(int i, int j) {
if (i < 0 || i >= m_numero_lato || j < 0 || j >= m_numero_lato) {
 return false;
}
return true;
}
/* template <typename C>
void censimento (C const& cell) {
switch (cell.m_S) {

Censimento censimento(Cellula const& cell) {
popolazione = {0, 0, 0, 0};
switch (cell.S) {
 case (Stato::VULNERABILE):
   popolazione.m_suscettibili++;
   break;

 case (Stato::INFETTO):
   popolazione.m_infetti++;
   break;

 case (Stato::GUARITO):
   popolazione.m_guariti++;

default:
popolazione.m_morti++;
}
}

void Aggiorna_counter(int i, int j) {
Cellula& cell = m_grid[i][j];

for (int a = 0; a <= 2; a++) {
 if (Esiste(i - 1, j - 1 + a)) {
   if (m_grid[i - 1][j - 1 + a].m_S == Stato::INFETTO) {
     cell.m_counter++;
   }
 }
 if (Esiste(i + 1, j - 1 + a)) {
   if (m_grid[i + 1][j - 1 + a].m_S == Stato::INFETTO) {
     cell.m_counter++;
   }
 }
}
if (Esiste(i, j - 1)) {
 if (m_grid[i][j - 1].m_S == Stato::INFETTO) {
   cell.m_counter++;
 }
}
if (Esiste(i, j + 1)) {
 if (m_grid[i][j + 1].m_S == Stato::INFETTO) {
   cell.m_counter++;
 }
}
}

void Aggiorna() {
srand(time(NULL));
m_giorni++;
popolazione = {0,0,0,0};
for (int i = 0; i < m_numero_lato; i++) {
 for (int j = 0; j < m_numero_lato; j++) {
   Cellula& cell = m_grid[i][j];
  /*  censimento(cell);

   if (cell.m_S == Stato::VULNERABILE) {
     int esponente = cell.m_counter;
     // cell.m_numero.setString(std::to_string(esponente));
     if (esponente == 0) {
       continue;
     } else {
       float prob_sano = pow(1 - m_probabilita_contagio, esponente);  // beta o gamma? beta sicuro

       float estrazione = (rand() % 101) / 100.f;  // IL .F è FONDAMENTALE

       if (estrazione > prob_sano) {
         cell.m_S = Stato::INFETTO;

         cell.Aggiorna_colore();
       }
     }
   }

   else if (cell.m_S == Stato::INFETTO) {
     cell.m_infection_days++;
     if ((rand() % 100) / 100.f < m_probabilita_guarigione) {
       cell.m_S = Stato::RIMOSSO;
       cell.Aggiorna_colore();
       // qua forse ci sta fare così
     }
   }

   cell.m_counter = 0;
 }
}
}

/*void contatore (){
stampa il censimento, non so come si fa. Edo aiutami tu

}

void Avanza() {
if (m_orologio.getElapsedTime().asSeconds() > 3) {
 for (int i = 0; i < m_numero_lato; i++) {
   for (int j = 0; j < m_numero_lato; j++) {
     Aggiorna_counter(i, j);
   }
 }
 Aggiorna();
 //contatore();
 m_orologio.restart();
}
}

std::pair<int, int> CheckMousePosition(sf::Vector2f t_coordinate_mouse) {
for (int i = 0; i < m_numero_lato; i++) {
 for (int j = 0; j < m_numero_lato; j++) {
   if (m_grid[i][j].getlimiti().contains(t_coordinate_mouse)) {
     return std::pair<int, int>{i, j};
   }
 }
}
return std::pair<int, int>{-1, -1};
}

void ChangeStatus(std::pair<int, int> t_coordinate, Stato t_stato) { m_grid[t_coordinate.first][t_coordinate.second].m_S = t_stato; }

void AzzeraOrologio() { m_orologio.restart(); }

void StopAutoma() { m_is_stopped = true; }

void StartAutoma() {
m_is_stopped = false;
m_orologio.restart();
}

bool IsStopped() { return m_is_stopped; }
};
*/

#endif