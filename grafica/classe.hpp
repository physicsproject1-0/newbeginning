#include <SFML/Graphics.hpp>
#include <cstdlib>

#ifndef DISA
#define DISA

class Persona {
  sf::VertexArray triangolo;  //  perchè cazzo non funziona
  sf::Vector2f vel;

 public:
  Persona(sf::Vector2f const& centro, float radius) {
    triangolo.setPrimitiveType(sf::Triangles);
    triangolo.resize(3);
    triangolo[0].position = sf::Vector2f(centro.x, centro.y - radius);  // strane coord
    triangolo[1].position = sf::Vector2f(centro.x - radius * (1.7f / 2), centro.y + (radius / 2));
    triangolo[2].position = sf::Vector2f(centro.x + radius * (1.7f / 2), centro.y + (radius / 2));
    triangolo[0].color = sf::Color::Yellow;
    triangolo[1].color = sf::Color::Yellow;
    triangolo[2].color = sf::Color::Yellow;

    vel = sf::Vector2f(rand() % 5 - 2.5f, rand() % 5 - 2.5f);
  }
  sf::Vector2f get(int n) { return triangolo[n].position; }
  sf::Vector2f getv() { return vel; }
};

class Mondo : public sf::Drawable {
 private:  // la draw non va nel protected??
  sf::VertexArray Griglia;
  int personetotali;
  std::vector<sf::Vector2f> velocita;
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(Griglia); }
  // perchè la devo mettere qua
  // virtual qua non ce lo devo mettere, giusto? tanto non creerò classi derivate da questa
  // poi come fa window.draw(entity) a chiamare internamente entity.draw se è nella parte privata?!
 
 void invvel(int i){
   velocita[i] = -velocita[i];
 }
 public:
  Mondo(int persone) {
    personetotali = persone;
    Griglia.resize(persone * 3);
    Griglia.setPrimitiveType(sf::Triangles);
    for (int i = 0; i < persone; i++) {
      Persona test(sf::Vector2f(rand() % 500, rand() % 500), 10);
      sf::Vertex* iter = &Griglia[i * 3];
      iter[0].position = test.get(0);
      iter[1].position = test.get(1);
      iter[2].position = test.get(2);
      velocita.push_back(test.getv());
    }
  }
  void evolvi() {

    for (int i = 0; i < personetotali * 3; i++) {
      if (Griglia[i].position.x >550||Griglia[i].position.x<-50 || Griglia[i].position.y<-50||Griglia[i].position.y>550){
        invvel(i);
      }
      Griglia[i].position = sf::Vector2f(Griglia[i].position.x + velocita[(int)(i/3)].x, Griglia[i].position.y + velocita[(int)(i/3)].x);
    }

  }
};

#endif