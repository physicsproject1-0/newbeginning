#include <SFML/Graphics.hpp>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unordered_set>

#ifndef SIRGRAFICA_HPP
#define SIRGRAFICA_HPP

namespace epidemia {

enum class State { t_suscettibile, t_infetto, t_rimosso };

// ci va l'inline qua?
inline float Random() {
  std::srand(time(NULL));
  int segno = rand() % 2;  // se è 0 moltiplico per +1, altrimenti se è 1 per -1;  //c'è una specie di algoritmo che lo fa?
  if (segno == 0) {
    segno--;
  }
  return segno * (rand() / RAND_MAX);

};  // funzione per random

class Persona {
 private:
  // gestire le dimensioni dei pallini in base allo schermo e al numero di pallini
  State stato;
  sf::CircleShape g_persona;   // chiedere se conviene metterlo qui comunque, anche se
                               // nella prima simulazione non viene utilizzato, ma è
                               // inutile creare un'altra classe, mezza ripetuta
  sf::Vector2f particularpos;  // diamo valori da zero a uno
  sf::Vector2f velocità;       // come sopra

 public:
  Persona( State persona = State::t_suscettibile, float dim = 20.f);  // Costruttore a qualsiasi stato, scegliere dimensione di default

  void diventa_infetto();
  void diventa_rimosso();

  State getstate();
  sf::Vector2f getposition();  // ti da la posizione

  void Updategraphics();
};

class Popolazione {
  std::vector<Persona> popolazione; //misa che non si può utilizzare unordered_set, chieedere a giaco

  float dim_pers;  // far applicare questo al costruttore di popolazione persona
 public:
  Popolazione(int sani, int malati = 0, int rimossi = 0);
  void Riempimento(int N, State condizione);
  int size();                  // metodo size
  void test(Persona& person);  // altrimenti non so come far funzionare il for_each
  void Updategrafica();
  void posizionatutti();  // posiziona randomicamente le persone nella mappa
  void check();           // controlla se c'è vicinanza tra più palline e bisogna fare la cura
};

}  // namespace epidemia

#endif
