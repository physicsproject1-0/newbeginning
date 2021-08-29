#include <SFML/Graphics.hpp>

#include "bordi.hpp"
#include "struct_enum.hpp"

#ifndef PENNELLO_HPP
#define PENNELLO_HPP

class Pennello : public sf::Drawable {
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

  bool m_is_active;           // significa se e' possibile cliccare
  bool m_inserimento_attivo;  // se hai preso il rettangolo
  bool m_verde_visibile;
  bool m_rosso_visibile;
  bool m_blu_visibile;
  bool m_bianco_visibile;

 public:
  Pennello(sf::Vector2f t_dimensione_riquadro, sf::Font* t_font)
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

  void ImpostaPosizione(sf::Vector2f t_posizione_centro);
  void Scrivi();
  void draw(sf::RenderTarget& target, sf::RenderStates) const;

  void AggiornaPosizioneRettangoliDaInserire(sf::Vector2f t_posizione_mouse);

  bool IsActive();

  void Attiva();

  void Disattiva();

  Checkbox RitornaVerde();
  Checkbox RitornaRosso();
  Checkbox RitornaBlu();
  Checkbox RitornaBianco();

  bool IsInserimentoAttivo();
  void AttivaInserimento();
  void DisattivaInserimento();

  Stato RitornaStatoRettangoloInserendo();
  void AttivaVerde();
  void AttivaRosso();
  void AttivaBlu();
  void AttivaBianco();
};

#endif