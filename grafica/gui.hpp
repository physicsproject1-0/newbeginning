
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <SFML/Graphics.hpp>

#include <cassert>
#include <cstdlib>
#include <string>

#include "struct_enum.hpp"
#include "bordi.hpp"
#include "riquadro_informazioni.hpp"
#include "paletta.hpp"

#ifndef GUI_HPP
#define GUI_HPP

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

  Informazioni m_riquadro_informazioni;

  MousePos m_posizione_mouse;

 public:
  GUI(sf::Vector2f dimensione);
  
  virtual void draw(sf::RenderTarget& target, sf::RenderStates ) const;
  
  void AggiornaPosizione(sf::Vector2f punto_in_altodx, sf::Vector2f dimensioni_finestra);

  void AggiornaPosizioneRettangoliPaletta(sf::Vector2f t_coordinate_mouse) {
    if (m_paletta_colori.IsInserimentoAttivo()) {
      m_paletta_colori.AggiornaPosizioneRettangoliDaInserire(t_coordinate_mouse);
    }
  }

  void AttivaInserimento() { m_paletta_colori.Attiva(); }

  void DisattivaInserimento() { m_paletta_colori.Disattiva(); }

  void CheckMousePosition(sf::Vector2f t_coordinate_mouse);

  void CheckColor(sf::Color colore);

  MousePos ClickOfMouse();

  void IsOut() { CheckColor(sf::Color(128, 128, 128)); }

  Paletta* GetPointerPaletta() { return &m_paletta_colori; }
  Informazioni* GetPointerRiquadro() { return &m_riquadro_informazioni; }

  bool IsInserimentoAttivo() { return m_paletta_colori.IsInserimentoAttivo(); }
};

#endif