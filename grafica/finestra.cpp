#include "finestra.hpp"

#include <SFML/Window.hpp>
#include <iostream>

#include "gui.hpp"

sf::Vector2f converti(sf::Vector2u vettore) { return sf::Vector2f(vettore.x, vettore.y); }

void Finestra::Setup(const std::string& temptitolo, const sf::Vector2u& tempdimensione) {
  v_titolo = temptitolo;
  v_dimensioni = tempdimensione;
  v_Isfullscreen = false;
  v_Isclosed = false;
  Create();
}

void Finestra::Create() {
  auto stile = (v_Isfullscreen ? sf::Style::Fullscreen : sf::Style::Default);
  v_mainfinestra.create(sf::VideoMode(v_dimensioni.x, v_dimensioni.y), v_titolo, stile);
}

void Finestra::Destroy() { v_mainfinestra.close(); }

Finestra::Finestra() { Setup("Defaultwindow", sf::Vector2u(1280, 720)); }

Finestra::Finestra(const std::string& titolo, const sf::Vector2u& dimensione, GUI* overlay) : v_overlay{overlay} { Setup(titolo, dimensione); }

Finestra::~Finestra() { Destroy(); }

void Finestra::Update() {
  sf::Event evento;
  while (v_mainfinestra.pollEvent(evento)) {
    if (evento.type == sf::Event::Closed || (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape)) {
      v_Isclosed = true;
    } else if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F5) {
      Fullscreen();  // perchè ci va l'else if?
    }

    if (evento.type == sf::Event::Resized) {
      sf::Vector2u dimensioni_nuove = v_mainfinestra.getSize();

      v_dimensioni = dimensioni_nuove;

      sf::Vector2f NuovoCentro(dimensioni_nuove.x, dimensioni_nuove.y);

      sf::View view2(sf::Vector2f(400, 300), NuovoCentro);

      sf::Vector2f punto_alto_dx(view2.getCenter().x-dimensioni_nuove.x/2,view2.getCenter().y-dimensioni_nuove.y/2);
      
      v_overlay->aggiorna_posizione(punto_alto_dx, converti(dimensioni_nuove));

      SetVista(view2);
    }
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::P) {    //lasciamo le lettere P e O ???
      sf::Vector2u dimensioni_nuove = v_mainfinestra.getSize();
      sf::Vector2f NuovoCentro(dimensioni_nuove.x, dimensioni_nuove.y);
      sf::View view3(sf::Vector2f(2000, 2000), NuovoCentro);
      SetVista(view3);
    }
     if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::O) {
      sf::Vector2u dimensioni_nuove = v_mainfinestra.getSize();
      sf::Vector2f NuovoCentro(dimensioni_nuove.x, dimensioni_nuove.y);
      sf::View view4(sf::Vector2f(400, 300), NuovoCentro);
      SetVista(view4);
    }
  }
}

void Finestra::SetVista(sf::View vista) { v_mainfinestra.setView(vista); }

/* void Finestra::aggiungi_pointer(GUI* puntatore){ v_overlay = puntatore;} */

void Finestra::Fullscreen() {
  v_Isfullscreen = true;
  Destroy();
  Create();
}

void Finestra::Pulisci() { v_mainfinestra.clear(sf::Color::Black); }
void Finestra::Disegna(sf::Drawable& cosadadisegnare) { v_mainfinestra.draw(cosadadisegnare); }  // capire come funziona sta roba
void Finestra::Mostra() { v_mainfinestra.display(); }

sf::Vector2u Finestra::Getdimensions() { return v_dimensioni; }

bool Finestra::Isclosed() { return v_Isclosed; }
bool Finestra::Isfullscreen() { return v_Isfullscreen; }