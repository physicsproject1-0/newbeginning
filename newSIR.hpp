#ifndef NEWSIR_HPP
#define NEWSIR_HPP

#include <vector>
#include <cassert>

//simone 

namespace epidemia {

struct State {
    
    double suscettibili;
    double infetti;
    double rimossi;

};

class SIR {

    private:
    State m_stato_iniziale;
    int m_durata_simulazione;
    double m_beta;
    double m_gamma;
   //boh forse, problema n int? 
    const int N = m_stato_iniziale.suscettibili + m_stato_iniziale.infetti + m_stato_iniziale.rimossi;

    public:
    SIR (State const& stato_iniziale, int durata_simulazione, double beta, double gamma)                           //costruttore
    : m_stato_iniziale{stato_iniziale}, m_durata_simulazione{durata_simulazione}, m_beta{beta}, m_gamma{gamma} {}

    std::vector<State> riempimento () {

        std::vector<State> simulazione{m_stato_iniziale};
        
        //State stato = simulazione.back()
        
        
        for (int i = 0; i <= m_durata_simulazione; i++) {
            
            State stato = simulazione.back(); 
            State newState;

            //stato.suscettibili -= (m_beta * stato.suscettibili * stato.infetti)/N;
            //
            //stato.infetti += m_beta * stato.suscettibili/N *stato.infetti - m_gamma * stato.infetti;
            //
            //stato.rimossi += m_gamma * stato.infetti;

            newState.suscettibili = stato.suscettibili - (m_beta * stato.suscettibili * stato.infetti)/N;
            newState.infetti = stato.infetti + (m_beta * stato.suscettibili *stato.infetti)/N - m_gamma * stato.infetti;
            newState.rimossi = stato.rimossi+ m_gamma * stato.infetti;
        
            simulazione.push_back(newState);
        }
        
        return simulazione;
    }

};

}






#endif 