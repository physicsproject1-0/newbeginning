#include "SIR.hpp"
#include "finestra.hpp"

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

class Animazione {
    // funzioni relative alla gestione interna delle palline che rappresentano
    // le persone
    Finestra v_finestra;
    epidemia::Popolazione v_popolo;
    // sfondo che deve caricare, possibilmente cina, così si inizia da paziente
    // zero
    int durata;  // tempo che passa tra un refresh e un altro

   public:
    Animazione();
    Animazione(epidemia::Popolazione& const
                   popolo);  // deve inizializzare la parte grafica del vettore e spawnare la gente a random con il metodo di popolazione
    ~Animazione();
    void Updinput();  // prende gli input (vaccini o cambio di qualcosa)
    void Update();    // in base agli input
    void Mostra();    // refresha
    // metodo per controllare se la finestra è aperta?
};

#endif