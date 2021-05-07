#include "animation.hpp"

int main() {
    // entry point
    Popolazione gruppo1();  // costruisce la popolazione
    // input del giaco con parametri b,y e popolazione
    // prima parte del progetto

    Animazione animazione(Popolazione gruppo1);  // costruisce l'animazione(griglia?); prende giusto il numero della gente che deve spawnare

    while (/*finestra è aperta*/) {
        // loop
        // input durante l'animazione, potenzialmente un comando restart e un comando per gestire i vaccini
        animazione.Updinput();
        animazione.Update();
        animazione.Mostra();
    }
}