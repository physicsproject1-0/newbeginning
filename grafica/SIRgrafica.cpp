#include "SIRgrafica.hpp"

#include <algorithm>

epidemia::Persona::Persona( State persona = State::t_suscettibile, float dim = 20.f) {
    stato=persona;
    g_persona.setRadius(dim);
    particularpos = sf::Vector2f(abs(Random()),abs(Random()));
    velocità = sf::Vector2f (Random(),Random());
    Updategraphics();
}  // Costruttore a qualsiasi stato, di default suscettibile

epidemia::State epidemia::Persona::getstate() { return stato; };

void epidemia::Persona::Updategraphics() {  // dimensione std
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
    Updategraphics();
}

void epidemia::Persona::diventa_rimosso() {
    assert(stato == State::t_infetto);
    stato = State::t_rimosso;
    Updategraphics();
}

epidemia::Popolazione::Popolazione(int sani, int malati = 0, int rimossi = 0){
    Riempimento(sani, State::t_suscettibile);
    Riempimento(malati, State::t_infetto);
    Riempimento(rimossi, State::t_rimosso);
}

void epidemia::Popolazione::Riempimento(int N, State condizione) {
    Persona edo(condizione);  //c'è modo per inizializzare solo dimensione?
    for (int i = 0; i < N; ++i) {
        popolazione.insert(edo);
    }
}

int epidemia::Popolazione::size() { return popolazione.size(); }  // metodo sizev

void epidemia::Popolazione::test(Persona& singola) {  // tocca mettecela altrimenti non va bene a for each
    singola.Updategraphics();
}

void epidemia::Popolazione::Updategrafica() { std::for_each(popolazione.cbegin(), popolazione.cend(), test); }
