#include "classe.hpp"
#include <iostream>

int main(){
    sf::RenderWindow window(sf::VideoMode(600, 600), "Tilemap");
    window.setFramerateLimit(60);
    int n;
    std::cin>>n;
    Mondo map(n);
    map.creagriglia();
    
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // draw the map
        window.clear();

        map.azzera();
        map.evolvi();
        
        window.draw(map);
        window.display();
    }

}