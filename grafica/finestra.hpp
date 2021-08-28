#include <SFML/Graphics.hpp>

#include "animazione.hpp"
#include "automa.hpp"
#include "gui.hpp"

#ifndef FINESTRA_HPP
#define FINESTRA_HPP

sf::Vector2f converti(sf::Vector2u vettore);
sf::Vector2f converti(sf::Vector2i vettore);

class Finestra {
  std::string v_titolo;
  sf::Vector2u v_dimensioni;

  sf::Vector2u v_dimensioni_minime;  // quelle con cui viene creata

  sf::RenderWindow v_mainfinestra;
  bool v_Isclosed;
  bool v_Isfullscreen;
  bool mouse_in;
  Vista v_vista;
  sf::View vista_animazione;
  sf::View vista_automa;

  sf::Mouse v_mouse;
  sf::Vector2f posizione_mouse;
  sf::Vector2f punto_alto_sx;

  GUI* v_overlay;
  Animazione* m_dinamico;
  Automa* m_statico;

  void Setup(const std::string& titolo, const sf::Vector2u& dimensione);
  void Create();
  void Destroy();

 public:
  Finestra();  // di default facciamo che la dimensione è 1280*720
  Finestra(const std::string& titolo, const sf::Vector2u& dimensione, GUI* overlay, Animazione* t_dinamico, Automa* t_statico,
           const Bordi& t_bordo_animazione,
           const Bordi& t_bordo_automa);  // passo tipi strani, quindi passo by ref
  ~Finestra();

  // void Setsfondo(); //aggiunta da me per attaccare lo sfondo
  void Update();

  void set_origin();
  void upd_vista();
  void specifiche_viste(Vista t_vista, sf::FloatRect t_limite);

  void Pulisci();
  void Disegna(sf::Drawable& cosadadisegnare);
  void Mostra();

  sf::Vector2u Getdimensions();

  sf::Vector2u Getoriginaldimensions();

  bool Isclosed();
  bool Isfullscreen();

  void SetVista(sf::View vista);
  Vista GetVista() { return v_vista; };
};

#endif