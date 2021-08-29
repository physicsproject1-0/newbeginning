#include <SFML/Graphics.hpp>

#include "struct_enum.hpp"
#include "bordi.hpp"

#ifndef PALETTA_HPP
#define PALETTA_HPP

class Paletta : public sf::Drawable {
  Checkbox m_box_verde;
  Checkbox m_box_rosso;
  Checkbox m_box_blu;
  Checkbox m_box_bianco;

  Bordi m_riquadro_esterno;

  sf::RectangleShape m_rettangolo_opaco;

  sf::RectangleShape m_dainserire_verde;
  sf::RectangleShape m_dainserire_rosso;
  sf::RectangleShape m_dainserire_blu;
  sf::RectangleShape m_dainserire_bianco;

  Textbox m_testo_verde;
  Textbox m_testo_rosso;
  Textbox m_testo_blu;
  Textbox m_testo_bianco;

  bool m_is_active;           // si intende se si può cliccare
  bool m_inserimento_attivo;  // se hai preso il rettangolo
  bool m_verde_visibile;
  bool m_rosso_visibile;
  bool m_blu_visibile;
  bool m_bianco_visibile;

 public:
  Paletta(sf::Vector2f t_dimensione_riquadro, sf::Font* t_font)
      : m_box_verde(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_box_rosso(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_box_blu(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_box_bianco(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_riquadro_esterno(t_dimensione_riquadro),

        m_testo_verde(9.5, t_font, sf::Color::Black),
        m_testo_rosso(9.5, t_font, sf::Color::Black),
        m_testo_blu(9.5, t_font, sf::Color::Black),
        m_testo_bianco(9.5, t_font, sf::Color::Black),
        m_is_active{true},
        m_inserimento_attivo{false},
        m_verde_visibile{false},
        m_rosso_visibile{false},
        m_blu_visibile{false},
        m_bianco_visibile{false} {
    m_rettangolo_opaco.setFillColor(sf::Color(128, 128, 128, 120));
    m_rettangolo_opaco.setSize(t_dimensione_riquadro);
    m_rettangolo_opaco.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.y / 2));

    m_box_verde.setinterncolor(sf::Color::Green);
    m_box_verde.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_box_rosso.setinterncolor(sf::Color::Red);
    m_box_rosso.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_box_blu.setinterncolor(sf::Color::Cyan);
    m_box_blu.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_box_bianco.setinterncolor(sf::Color::White);
    m_box_bianco.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_dainserire_verde.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_verde.setFillColor(sf::Color::Green);
    m_dainserire_verde.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));

    m_dainserire_rosso.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_rosso.setFillColor(sf::Color::Red);
    m_dainserire_rosso.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));

    m_dainserire_blu.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_blu.setFillColor(sf::Color::Cyan);
    m_dainserire_blu.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));

    m_dainserire_bianco.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_bianco.setFillColor(sf::Color::White);
    m_dainserire_bianco.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));
  }

  void ImpostaPosizione(sf::Vector2f t_posizione_centro) {  // la consideriamo rispetto al centro
    m_riquadro_esterno.set_posizione(t_posizione_centro -
                                     sf::Vector2f(m_riquadro_esterno.getlimiti().width / 2, m_riquadro_esterno.getlimiti().height / 2));

    m_rettangolo_opaco.setPosition(t_posizione_centro);

    m_box_verde.set_posizione(
        sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.getlimiti().top + (m_riquadro_esterno.getlimiti().height / 5) * 1));
    m_box_rosso.set_posizione(
        sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.getlimiti().top + (m_riquadro_esterno.getlimiti().height / 5) * 2));
    m_box_blu.set_posizione(sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.getlimiti().top + (m_riquadro_esterno.getlimiti().height / 5) * 3));
    m_box_bianco.set_posizione(
        sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.getlimiti().top + (m_riquadro_esterno.getlimiti().height / 5) * 4));

    m_testo_verde.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_verde.getlimiti().top - 10));
    m_testo_rosso.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_rosso.getlimiti().top - 10));
    m_testo_blu.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_blu.getlimiti().top - 10));
    m_testo_bianco.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_bianco.getlimiti().top - 10));
  }
  void Scrivi() {  // funzione presente altrimenti scrive prima di caricare il font e mi restituisce 0 di larghezza , il loading del font avviene dopo
                   // in GUI
    m_testo_verde.scrivi("Vulnerabile");
    m_testo_rosso.scrivi("Infetto");
    m_testo_blu.scrivi("Guarito");
    m_testo_bianco.scrivi("Morto");

    if (m_testo_verde.getlimiti().width > m_riquadro_esterno.getlimiti().width ||
        m_testo_rosso.getlimiti().width > m_riquadro_esterno.getlimiti().width ||
        m_testo_blu.getlimiti().width > m_riquadro_esterno.getlimiti().width ||
        m_testo_bianco.getlimiti().width > m_riquadro_esterno.getlimiti().width) {
      throw std::runtime_error("dimensione paletta non sufficiente per ospitare testo");
    }
  }
  void draw(sf::RenderTarget& target, sf::RenderStates ) const {
    target.draw(m_riquadro_esterno);

    target.draw(m_box_verde);
    target.draw(m_box_rosso);
    target.draw(m_box_blu);
    target.draw(m_box_bianco);

    target.draw(m_testo_verde);
    target.draw(m_testo_rosso);
    target.draw(m_testo_blu);
    target.draw(m_testo_bianco);

    if (!m_is_active) {
      target.draw(m_rettangolo_opaco);
    }

    if (m_inserimento_attivo) {
      if (m_verde_visibile) {
        target.draw(m_dainserire_verde);
      }
      if (m_rosso_visibile) {
        target.draw(m_dainserire_rosso);
      }
      if (m_blu_visibile) {
        target.draw(m_dainserire_blu);
      }
      if (m_bianco_visibile) {
        target.draw(m_dainserire_bianco);
      }
    }
  }

  void AggiornaPosizioneRettangoliDaInserire(sf::Vector2f t_posizione_mouse) {
    m_dainserire_verde.setPosition(t_posizione_mouse);
    m_dainserire_rosso.setPosition(t_posizione_mouse);
    m_dainserire_blu.setPosition(t_posizione_mouse);
    m_dainserire_bianco.setPosition(t_posizione_mouse);
  }

  bool IsActive() { return m_is_active; }

  void Attiva() { m_is_active = true; }

  void Disattiva() { m_is_active = false; }

  Checkbox RitornaVerde() { return m_box_verde; }
  Checkbox RitornaRosso() { return m_box_rosso; }
  Checkbox RitornaBlu() { return m_box_blu; }
  Checkbox RitornaBianco() { return m_box_bianco; }

  bool IsInserimentoAttivo() { return m_inserimento_attivo; }
  void AttivaInserimento() { m_inserimento_attivo = true; }
  void DisattivaInserimento() { m_inserimento_attivo = false; }

  Stato RitornaStatoRettangoloInserendo() {
    if (m_verde_visibile) {
      return Stato::VULNERABILE;
    } else if (m_rosso_visibile) {
      return Stato::INFETTO;
    } else if (m_blu_visibile) {
      return Stato::GUARITO;
    } else if (m_bianco_visibile) {
      return Stato::MORTO;
    } else {
      throw std::runtime_error("non stai inserendo nessun rettangolo");
    }
  }

  void AttivaVerde() {
    m_verde_visibile = true;
    m_rosso_visibile = false;
    m_blu_visibile = false;
    m_bianco_visibile = false;
  }
  void AttivaRosso() {
    m_verde_visibile = false;
    m_rosso_visibile = true;
    m_blu_visibile = false;
    m_bianco_visibile = false;
  }
  void AttivaBlu() {
    m_verde_visibile = false;
    m_rosso_visibile = false;
    m_blu_visibile = true;
    m_bianco_visibile = false;
  }
  void AttivaBianco() {
    m_verde_visibile = false;
    m_rosso_visibile = false;
    m_blu_visibile = false;
    m_bianco_visibile = true;
  }
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

#endif