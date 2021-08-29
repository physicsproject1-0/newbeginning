#include <SFML/Graphics.hpp>

#include "animazione.hpp"
#include "automa.hpp"
#include "gui.hpp"

#ifndef FINESTRA_HPP
#define FINESTRA_HPP

sf::Vector2f converti(sf::Vector2u vettore);
sf::Vector2f converti(sf::Vector2i vettore);

class Finestra {
  std::string m_titolo;
  sf::Vector2u m_dimensioni;

  sf::Vector2u m_dimensioni_minime;  // quelle con cui viene creata
  sf::Vector2i m_posizione;

  sf::RenderWindow m_mainfinestra;

  bool m_is_closed;
  bool m_mouse_in;

  Vista m_vista;
  sf::View m_vista_animazione;
  sf::View m_vista_automa;

  sf::Mouse m_mouse;
  sf::Vector2f m_posizione_mouse;
  sf::Vector2f m_punto_alto_sx;

  GUI* m_overlay;
  Animazione* m_dinamico;
  Automa* m_statico;

  void Setup(const std::string& t_titolo, const sf::Vector2u& t_dimensione);
  void Create();
  void Destroy();

 public:
  Finestra();  // di default facciamo che la dimensione è 1280*720
  Finestra(const std::string& t_titolo, const sf::Vector2u& t_dimensione, GUI* t_overlay, Animazione* t_dinamico, Automa* t_statico,
           const Bordi& t_bordo_animazione, const Bordi& t_bordo_automa);
  ~Finestra();

  void Update();

  void UpdVista();
  void SpecificheViste(Vista t_vista, sf::FloatRect t_limite);

  void Pulisci();
  void Disegna(sf::Drawable& t_cosadadisegnare);
  void Mostra();

  bool IsClosed();

  void SetVista(sf::View t_vista);
  Vista GetVista() { return m_vista; };
};

#endif