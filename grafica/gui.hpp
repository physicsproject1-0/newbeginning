#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <SFML/Graphics.hpp>
#include <cassert>
#include <cstdlib>
#include <string>

#include "bordi.hpp"
#include "pennello.hpp"
#include "riquadro_informazioni.hpp"
#include "struct_enum.hpp"

#ifndef GUI_HPP
#define GUI_HPP

//Gestisce l'intera GUI
class GUI : public sf::Drawable {
  sf::Font m_font;

  sf::RectangleShape m_sfondo_grigio;
  Bordi m_limiti_sfondo_grigio;

  Textbox m_testo_animazione;
  Textbox m_testo_automa;

  Checkbox m_casella_animazione;
  Checkbox m_casella_automa;

  Pulsante m_pulsante_play;
  Pulsante m_pulsante_pausa;

  Pennello m_pennello_colori;

  Informazioni m_riquadro_informazioni;

  MousePos m_posizione_mouse;

 public:
  GUI(sf::Vector2f t_dimensione);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;

  void AggiornaPosizione(sf::Vector2f t_punto_in_altodx, sf::Vector2f t_dimensioni_finestra);
  void AggiornaPosizioneRettangoliPennello(sf::Vector2f t_coordinate_mouse);

  void AttivaInserimento();   //riguardano opacità paletta
  void DisattivaInserimento();

  void CheckMousePosition(sf::Vector2f t_coordinate_mouse);
  void CheckColor(sf::Color t_colore);
  MousePos ClickOfMouse();  
  
  void IsOut();

  Pennello* GetPointerPennello();
  Informazioni* GetPointerRiquadro();

  bool IsInserimentoAttivo();  //riguardano possibilità di inserire
};

#endif