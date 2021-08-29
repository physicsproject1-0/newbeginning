#include <SFML/Graphics.hpp>

#include "bordi.hpp"
#include "struct_enum.hpp"

#ifndef PENNELLO_HPP
#define PENNELLO_HPP

//Class per disegnare tavolozza per riempimento automa
class Pennello : public sf::Drawable {  //disegna rispetto rispetto al centro
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

  bool m_is_active;           // relativa a opacizzazione paletta
  bool m_inserimento_attivo;  // relativa a inserimento rettangolo
  
  bool m_verde_visibile;
  bool m_rosso_visibile;
  bool m_blu_visibile;
  bool m_bianco_visibile;

 public:
  Pennello(sf::Vector2f t_dimensione_riquadro, sf::Font* t_font);

  void ImpostaPosizione(sf::Vector2f t_posizione_centro);
  void Scrivi();
  void draw(sf::RenderTarget& target, sf::RenderStates) const;

  void AggiornaPosizioneRettangoliDaInserire(sf::Vector2f t_posizione_mouse);

  bool IsActive();  //relative a opacizzazione paletta
  void Attiva();
  void Disattiva();

  Checkbox RitornaVerde();
  Checkbox RitornaRosso();
  Checkbox RitornaBlu();
  Checkbox RitornaBianco();

  bool IsInserimentoAttivo();   //relative a inserimento rettangolo
  void AttivaInserimento();
  void DisattivaInserimento();

  Stato RitornaStatoRettangoloInserendo();
  void AttivaVerde();
  void AttivaRosso();
  void AttivaBlu();
  void AttivaBianco();
};

#endif