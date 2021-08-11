#include "classe.hpp"

#include "finestra.hpp"

Mondo::Mondo(int persone) : a_window("test", sf::Vector2u(500, 500)) {
  if (!ominoprova.loadFromFile("uomoverde.png")) {
    throw std::runtime_error{"texture loading failed"};
  }

  Persona prova;

  for (int i = 0; i < persone; i++) {
    prova.centro = sf::Vector2f(rand() % 450 + 50, rand() % 450 + 50);
    prova.raggio = 10.f;
    // prova.metalato = 5.f;
    prova.vel = sf::Vector2f(rand() % 50 - 25.f, rand() % 50 - 25.f);
    Lista[i] = prova;
  }
  Griglia.resize(Lista.size() * 3);
  // Griglia.resize(Lista.size() * 4);
  // Griglia.setPrimitiveType(sf::Quads);
  Griglia.setPrimitiveType(sf::Triangles);
  // facciamo partire la finestra

  settexture();
}

void Mondo::settexture() {
  for (int i = 0; i < Lista.size(); i++) {
    sf::Vertex* iter = &Griglia[i * 3];
    // iter[0].color = sf::Color::Transparent;
    // iter[1].color = sf::Color::Transparent;
    // iter[2].color = sf::Color::Transparent;
    iter[0].texCoords = sf::Vector2f(430.f, 0.f);  // strane coord
    iter[1].texCoords = sf::Vector2f(0.f, 1681.f);
    iter[2].texCoords = sf::Vector2f(860.f, 1681.f);
  }
}

void Mondo::aggiornagriglia() {
  for (int i = 0; i < Lista.size(); i++) {
    // sf::Vertex* iter = &Griglia[i * 3];

    // iter[0].color = sf::Color::Yellow;
    // iter[1].color = sf::Color::Yellow;
    // iter[2].color = sf::Color::Yellow;
    sf::Vertex* iter = &Griglia[i * 3];
    iter[0].position = sf::Vector2f(Lista[i].centro.x, Lista[i].centro.y - Lista[i].raggio);  // strane coord
    iter[1].position = sf::Vector2f(Lista[i].centro.x - Lista[i].raggio * (1.7f / 2), Lista[i].centro.y + (Lista[i].raggio / 2));
    iter[2].position = sf::Vector2f(Lista[i].centro.x + Lista[i].raggio * (1.7f / 2), Lista[i].centro.y + (Lista[i].raggio / 2));

    // sf::Vertex* iter = &Griglia[i * 4];
    // iter[0].position = sf::Vector2f(Lista[i].centro.x - Lista[i].metalato, Lista[i].centro.y - Lista[i].metalato);  // strane coord
    // iter[1].position = sf::Vector2f(Lista[i].centro.x + Lista[i].metalato, Lista[i].centro.y - Lista[i].metalato);
    // iter[2].position = sf::Vector2f(Lista[i].centro.x + Lista[i].metalato, Lista[i].centro.y + Lista[i].metalato);
    // iter[3].position = sf::Vector2f(Lista[i].centro.x - Lista[i].metalato, Lista[i].centro.y + Lista[i].metalato);
  }
}

// sf::Vector2f estimate(Persona& persona) {
//  return persona.centro + persona.vel * trascorso.asSeconds();
//  // (estimated.x <50 || estimated.y<50 || estimated.y>500 || estimated.x>500){
//}

// pensare a modo con cambiare la posizione delle pallline che sbattono
void Mondo::create_sweptvolume(Persona& persona) {
  sf::Vector2f spostamento = persona.vel * trascorso.asSeconds();
  sf::Vector2f posiz_finale = persona.centro + spostamento;

  persona.scia.top = fmin(persona.centro.y, posiz_finale.y) - persona.raggio;
  persona.scia.left = fmin(persona.centro.x, posiz_finale.x) - persona.raggio;
  persona.scia.height = abs(persona.centro.y - posiz_finale.y) + 2 * persona.raggio;
  persona.scia.width = abs(persona.centro.x - posiz_finale.x) + 2 * persona.raggio;
}

void Mondo::evolvi_singolo(int indice) {
  // while (estimate(Lista[indice]) e altro) {
  //}
  float deltat = trascorso.asSeconds();  // metto qua perchè se lo chiamo in punti diversi magari sono leggemente diversi

  if (Lista[indice].centro.x < 50 || Lista[indice].centro.x > 550) {
    Lista[indice].vel.x = -Lista[indice].vel.x;
  }
  if (Lista[indice].centro.y < 50 || Lista[indice].centro.y > 550) {
    Lista[indice].vel.y = -Lista[indice].vel.y;
  }

  // check_collisions(indice);

  /*
    for (int indice = 0; indice < Lista.size(); indice++) {                                  // Forse il problema sta in map che ha due argomenti
      if (Lista[indice].getGlobalBounds().intersects(Lista[indice].getGlobalBounds()) == 1)  // non so come applicare la funzione alle singole persone
      {                                                                                      // intersects() va bene solo se usiamo rettangoli
        Lista[indice].vel.x = -Lista[indice].vel.x;
        Lista[indice].vel.y = -Lista[indice].vel.y;

        if (!ominoprova.loadFromFile("uomoverde.png"))      // capire come cavolo associare la texture alla singola persona
      }
    }
  */

  // if (Lista[indice].cambiovelocita.getElapsedTime().asSeconds() > 3) {  // per modificare il moto browniano
  //  sf::Vector2f nuovavel(rand() % 50 - 25.f, rand() % 50 - 25.f);
  //  Lista[indice].vel = nuovavel;
  //  Lista[indice].cambiovelocita.restart();
  //}
  Lista[indice].centro += Lista[indice].vel * deltat;
  // sf::Vertex* iter = &Griglia[i* 4];
  // iter[0].position += Lista[i].vel * deltat;  // strane coord
  // iter[1].position += Lista[i].vel * deltat;
  // iter[2].position += Lista[i].vel * deltat;
}

void Mondo::evolvi() {
  for (int i = 0; i < Lista.size(); i++) {
    // if (Griglia[i].position.x > 550 || Griglia[i].position.x < -50 || Griglia[i].position.y < -50 || Griglia[i].position.y > 550) {
    //  invvel(i);
    //}
    evolvi_singolo(i);
  }
}

int Mondo::check_occur(Persona const& persona, int raggio) {  // decidere un raggio accettabile
  int occur = 0;
  for (int i = 0; i < Lista.size(); i++) {
    if (modulo(persona.centro - Lista[i].centro) <= raggio) {
      occur++;
    }
  }
  return occur;
}  // in caso creare delle corone circolari con varie numerazioni
// introdurre dipendenza dal tempo

// void check_collisions(int j) {
//  for (int i = 0; i < Lista.size(); i++) {
//    if (((abs(Lista[i].centro.x - Lista[j].centro.x) <= 2 * Lista[j].metalato)) &&
//        (abs(Lista[i].centro.y - Lista[j].centro.y) <= 2 * Lista[j].metalato) && (i != j)) {
//      std::swap(Lista[j].vel , Lista[i].vel);
//
//
//    }
//  }
//}

double Mondo::modulo(sf::Vector2f const& vettore) { return sqrt(pow(vettore.x, 2) + pow(vettore.y, 2)); }

void Mondo::azzera() { trascorso = timer.restart(); }
