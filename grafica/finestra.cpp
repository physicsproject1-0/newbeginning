#include "finestra.hpp"
#include <iostream>
#include <SFML/Window.hpp>

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

Finestra::Finestra(const std::string& titolo, const sf::Vector2u& dimensione) { Setup(titolo, dimensione); }

Finestra::~Finestra() { Destroy(); }

void Finestra::Update() {
    sf::Event evento;
    while (v_mainfinestra.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed || (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape)) {
            v_Isclosed = true;
        } else if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F5) {
            Fullscreen();  // perchè ci va l'else if?
        }
        
        if (evento.type == sf::Event::Resized){
            
           

           sf::Vector2u dimensioni_nuove = v_mainfinestra.getSize();

            sf::Vector2f NuovoCentro(dimensioni_nuove.x, dimensioni_nuove.y);

            sf::View view2(sf::Vector2f(400,300),NuovoCentro);
        
        
            SetVista(view2);
        }
        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::P){
            sf::Vector2u dimensioni_nuove = v_mainfinestra.getSize();
            sf::Vector2f NuovoCentro(dimensioni_nuove.x, dimensioni_nuove.y);
            sf::View view3(sf::Vector2f(2000,2000),NuovoCentro);
            SetVista(view3);
        }

    }
}

void Finestra::SetVista(sf::View vista){
    v_mainfinestra.setView(vista);
}

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