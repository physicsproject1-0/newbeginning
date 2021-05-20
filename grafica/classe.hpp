#include <SFML/Graphics.hpp>
#include <cstdlib>

#ifndef DISA
#define DISA

struct Persona {
  sf::Vector2f centro;
  float raggio;
  sf::Vector2f vel;
};

class Mondo : public sf::Drawable {
 private:  // la draw non va nel protected??
  sf::VertexArray Griglia;
  std::map<int, Persona> Lista;

  sf::Clock timer;
  sf::Time trascorso;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(Griglia); }

  // perchè la devo mettere qua
  // virtual qua non ce lo devo mettere, giusto? tanto non creerò classi derivate da questa
  // poi come fa window.draw(entity) a chiamare internamente entity.draw se è nella parte privata?!

 public:
  Mondo(int persone) {
    Persona prova;

    for (int i = 0; i < persone; i++) {
      prova.centro = sf::Vector2f(rand() % 450 + 50, rand() % 450 + 50);
      prova.raggio = 10.f;
      prova.vel = sf::Vector2f(rand() % 50 - 25.f, rand() % 50 - 25.f);
      Lista[i] = prova;
    }
    Griglia.resize(Lista.size() * 3);
    Griglia.setPrimitiveType(sf::Triangles);
  }

  void creagriglia() {
    for (int i = 0; i < Lista.size(); i++) {
      // sf::Vertex* iter = &Griglia[i * 3];
      // iter[0].position = Lista[i].get(0);
      // iter[1].position = Lista[i].get(1);
      // iter[2].position = Lista[i].get(2);
      // iter[0].color = sf::Color::Yellow;
      // iter[1].color = sf::Color::Yellow;
      // iter[2].color = sf::Color::Yellow;
      sf::Vertex* iter = &Griglia[i * 3];
      iter[0].position = sf::Vector2f(Lista[i].centro.x, Lista[i].centro.y - Lista[i].raggio);  // strane coord
      iter[1].position = sf::Vector2f(Lista[i].centro.x - Lista[i].raggio * (1.7f / 2), Lista[i].centro.y + (Lista[i].raggio / 2));
      iter[2].position = sf::Vector2f(Lista[i].centro.x + Lista[i].raggio * (1.7f / 2), Lista[i].centro.y + (Lista[i].raggio / 2));
    }
  }

  //sf::Vector2f estimate(Persona& persona) {
  //  return persona.centro + persona.vel * trascorso.asSeconds();
  //  // (estimated.x <50 || estimated.y<50 || estimated.y>500 || estimated.x>500){
  //}

//pensare a modo con cambiare la posizione delle pallli ne che sbattono
  void evolvi_singolo(int indice) {
    //while (estimate(Lista[indice]) e altro) {
    //}

    Lista[indice].centro += Lista[indice].vel * trascorso.asSeconds();

    sf::Vertex* iter = &Griglia[indice * 3];
    iter[0].position += sf::Vector2f(Lista[indice].vel * trascorso.asSeconds());  // strane coord
    iter[1].position += sf::Vector2f(Lista[indice].vel * trascorso.asSeconds());
    iter[2].position += sf::Vector2f(Lista[indice].vel * trascorso.asSeconds());
  }


  void evolvi() {
  for (int i = 0; i < Lista.size(); i++) {
    // if (Griglia[i].position.x > 550 || Griglia[i].position.x < -50 || Griglia[i].position.y < -50 || Griglia[i].position.y > 550) {
    //  invvel(i);
    //}
    evolvi_singolo(i);
  }
}

void azzera() { trascorso = timer.restart(); }
}
;

#endif