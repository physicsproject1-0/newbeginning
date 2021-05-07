#include <SFML/Graphics.hpp>
#include <cassert>
#include <vector>
#include <cstdlib>
#include <ctime>

#ifndef SIR_HPP
#define SIR_HPP

namespace epidemia {

enum class State { t_suscettibile, t_infetto, t_rimosso };

enum class Direzione {t_fermo, t_alto, t_basso, t_destra, t_sinistra};

//ci va l'inline qua?
inline unsigned Random(){
    std::srand(time(NULL));
    
}; //funzione per random

class Persona {
   private:
    State stato;
    sf::CircleShape g_persona;  // chiedere se conviene metterlo qui comunque, anche se
                                // nella prima simulazione non viene utilizzato, ma è
                                // inutile creare un'altra classe, mezza ripetuta
    sf::Vector2u posizmille;
   public:
    Persona();               // default
    Persona(State persona);  // Costruttore a qualsiasi stato

    void diventa_infetto();
    void diventa_rimosso();

    State getstate();

    //funzioni relative alla grafica
    void Updategraphics();           // dimensione std
    void Updategraphics(float dim);  // dimensione variabile
    void posiziona();   //registrarlo con il random, 

};

class Popolazione {
    std::vector<Persona> popolazione;

   public:
    Popolazione(int n);
    void Riempimento(int N);
    int size();                  // metodo size
    void test(Persona& person);  // altrimenti non so come far funzionare il for_each
    void Updategrafica();
    void posizionatutti();       //posiziona randomicamente le persone nella mappa
};

}  // namespace epidemia

#endif
