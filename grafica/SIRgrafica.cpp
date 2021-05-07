#include "SIR.hpp"

#include <algorithm>

epidemia::Persona::Persona(State persona) : stato{persona} {}  // Costruttore a qualsiasi stato

epidemia::Persona::Persona() {
    stato = State::t_suscettibile;  // costruttore di default, inizializza a
                                    // suscettibile
}

epidemia::State epidemia::Persona::getstate() { return stato; };

void epidemia::Persona::Updategraphics() {  // dimensione std
    g_persona.setRadius(20.f);
    switch (stato) {
        case State::t_suscettibile:
            g_persona.setFillColor(sf::Color::White);

        case State::t_infetto:
            g_persona.setFillColor(sf::Color::Red);

        case State::t_rimosso:
            g_persona.setFillColor(sf::Color::Green);
    }
}

void epidemia::Persona::Updategraphics(float dim) {  // dimensione variabile
    g_persona.setRadius(dim);
    switch (stato) {
        case State::t_suscettibile:
            g_persona.setFillColor(sf::Color::White);

        case State::t_infetto:
            g_persona.setFillColor(sf::Color::Red);

        case State::t_rimosso:
            g_persona.setFillColor(sf::Color::Green);
    }
}

void epidemia::Persona::diventa_infetto() {
    // assert controlla se infetto oppure con exception
    assert(stato == State::t_suscettibile);
    stato = State::t_infetto;
}

void epidemia::Persona::diventa_rimosso() {
    assert(stato == State::t_infetto);
    stato = State::t_rimosso;
}

epidemia::Popolazione::Popolazione(int n) { Riempimento(n); }

void epidemia::Popolazione::Riempimento(int N) {
    Persona edo;  // Come funziona??
    //algoritmo
    for (int i = 0; i < N; ++i) {
        popolazione.push_back(edo);
    }
}

int epidemia::Popolazione::size() { return popolazione.size(); }  // metodo sizev

void epidemia::Popolazione::test(Persona& singola) {  // tocca mettecela altrimenti non va bene a for each
    singola.Updategraphics();
}

void epidemia::Popolazione::Updategrafica() { std::for_each(popolazione.cbegin(), popolazione.cend(), test); }
