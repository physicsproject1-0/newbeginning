#include "classe.hpp"

#include <algorithm>


Mondo::Mondo()
    :m_overlay(sf::Vector2f(70, 500)) , m_dinamica(20), m_statica(sf::Vector2f(1800, 1800), sf::Vector2f(500, 500), 3, 0.3, 0.5, 0, 0), m_window("test", sf::Vector2u(800, 600), &m_overlay, m_dinamica.get_bordi(), m_statica.get_bordi()){
  /*   if (!ominoprova.loadFromFile("uomoverde.png")) {
      throw std::runtime_error{"texture loading failed"};
    }
   */
}

// sf::Vector2f estimate(Persona& persona) {
//  return persona.centro + persona.vel * trascorso.asSeconds();
//  // (estimated.x <50 || estimated.y<50 || estimated.y>500 || estimated.x>500){
//}

// pensare a modo con cambiare la posizione delle pallline che sbattono

/* void Mondo::evolvi_singolo(int indice) {
  // while (estimate(Lista[indice]) e altro) {
  //}
  float deltat = trascorso.asSeconds();  // metto qua perchè se lo chiamo in punti diversi magari sono leggemente diversi

  if (Lista[indice].centro.x < 50 || Lista[indice].centro.x > 550) {
    Lista[indice].vel.x = -Lista[indice].vel.x;
  }
  if (Lista[indice].centro.y < 50 || Lista[indice].centro.y > 550) {
    Lista[indice].vel.y = -Lista[indice].vel.y;
  }


    for (int indice = 0; indice < Lista.size(); indice++) {                                  // Forse il problema sta in map che ha due argomenti
      if (Lista[indice].getGlobalBounds().intersects(Lista[indice].getGlobalBounds()) == 1)  // non so come applicare la funzione alle singole persone
      {                                                                                      // intersects() va bene solo se usiamo rettangoli
        Lista[indice].vel.x = -Lista[indice].vel.x;
        Lista[indice].vel.y = -Lista[indice].vel.y;


      }
    }


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

 */
  // in caso creare delle corone circolari con varie numerazioni
// introdurre dipendenza dal tempo

/* void Animazione::check_collisions() {  // Non so cosa passare a questa funzione e se cosi' va bene, l 'idea c e'
  for (int i = 0; i < popolazione.size(); i++) {
    Persona& PallinaA = popolazione[i];
    for (int j = 0; j < popolazione.size(); j++) {

      Persona& PallinaB = popolazione[j];


      if ((!(PallinaA.checked || PallinaB.checked)) && (i != j)) {
        if (modulo(PallinaA.centro - PallinaB.centro) <= 1.5 * PallinaB.raggio) {
          PallinaA.checked = true;
          PallinaB.checked = true;
          std::swap(PallinaA.vel.x, PallinaB.vel.x);
          std::swap(PallinaA.vel.y, PallinaB.vel.y);
          // collisione elementare, controllare solo le palline che si buggano su se stesse
        }
      }
    }



  }
  for (int i = 0; i < popolazione.size(); i++) {
    popolazione[i].checked = false;
  }
} */

/*
void Mondo::change_status() {
  for (int i = 0; i < Lista.size(); i++) {          //col ciclo controlla ogni singola persona della Lista, penso
    if ((Mondo::check_collisions(qualcosa che non so) == true) && (status == VULNERABLE)) {       // Non tiene conto del fatto che almeno una delle
      status = INFECTED;                                                                          // due che si scontrano deve essere gia' rossa
    } else if ((Mondo::check_collisions(qualcosa che non so) == true) && (status == INFECTED)) {  // da fixare questa cosa
      status = REMOVED;
    }
  }
}
*/

/*
void Mondo::change_vel() {
  for (int i = 0; i < Lista.size(); i++) {
    if ((Mondo::check_collisions(qualcosa che non so) == true) {
      std::swap(Lista[j].vel, Lista[i].vel);
    }
  }
}
*/


void Mondo::Azzera() { m_trascorso = m_timer.restart(); }
