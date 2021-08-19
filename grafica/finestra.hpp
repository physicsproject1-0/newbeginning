#ifndef FINESTRA_HPP
#define FINESTRA_HPP
#include <SFML/Graphics.hpp>

class Finestra {
    std::string v_titolo;
    sf::Vector2u v_dimensioni;
    sf::RenderWindow v_mainfinestra;
    bool v_Isclosed;
    bool v_Isfullscreen;

    void Setup(const std::string& titolo, const sf::Vector2u& dimensione);
    void Create();
    void Destroy();

   public:
    Finestra();  // di default facciamo che la dimensione è 1280*720
    Finestra(const std::string& titolo,
             const sf::Vector2u& dimensione);  // passo tipi strani, quindi passo by ref
    ~Finestra();

    // void Setsfondo(); //aggiunta da me per attaccare lo sfondo
    void Update();
    void Fullscreen();

    void Pulisci();
    void Disegna(sf::Drawable& cosadadisegnare);
    void Mostra();

    sf::Vector2u Getdimensions();
    
    sf::Vector2u Getoriginaldimensions();

    bool Isclosed();
    bool Isfullscreen();

    void SetVista(sf::View vista);
};

#endif 