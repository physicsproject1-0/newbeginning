#include "classe.hpp"

int main(){
    sf::RenderWindow window(sf::VideoMode(600, 600), "Tilemap");

    Mondo map(10);
    
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
        map.evolvi();
        window.draw(map);
        window.display();
    }

}