// int main(){
// 
    // epidemia::Persona simone(epidemia::State::t_suscettibile);
    // 
    // assert (simone.getstate()==epidemia::State::t_suscettibile);
    // simone.diventa_infetto();
    // assert (simone.getstate()==epidemia::State::t_infetto);
    // simone.diventa_rimosso();
    // assert (simone.getstate()==epidemia::State::t_rimosso);
// 
    // 
// 
//  epidemia::Popolazione bologna(100000);
//  std::cout << bologna.size() <<'\n';   
// }

#include "NewSIR.hpp"
#include <cassert>
#include <iostream>

void print ();    //o qui o dentro la classe

// chiedere se il main deve essere il più stringato possibile
// chiedere come dividere il codice?

int main () {

double beta;            //chiedere se va bene la posizione delle dichiarazioni
double gamma;
int S_I;
int I_I;
int R_I;
int giorni;

//assert per controllare che il giaco non metta stronzate

std::cout << "Buongiorno, inserisca i parametri beta e gamma del modello SIR \n"; 
std::cout << "beta >> ";
std::cin >> beta;
assert (!(beta < 0 || beta > 1));

std::cout << "gamma >> ";
std::cin >> gamma;
assert (!(gamma < 0 || gamma > 1));

std:: cout << "Ora inserisca il numero iniziale di persone suscettibili nel nostro campione \n";
std:: cin >> S_I;
assert(!(S_I < 0));

std:: cout << "Ora inserisca il numero iniziale di persone infetti nel nostro campione \n";
std:: cin >> I_I;
assert(!(I_I < 0));

std:: cout << "Ora inserisca il numero iniziale di persone rimossi nel nostro campione \n";
std:: cin >> R_I;
assert(!(R_I < 0));

std:: cout << "Ora inserisca la durata del nostro esperimento \n";
std:: cin >> giorni;
assert (!(giorni < 0));

//creare uno stato iniziale, darlo al costrttore di sir, chiami evoluzione, fai stampare

epidemia::State s0;
s0.suscettibili = S_I; 
s0.infetti = I_I;
s0.rimossi = R_I;

epidemia::SIR mondo(s0, giorni, beta, gamma);

//al massimo mettiamo riempimento dentro la funzione che ti stampa le cose, definita dentro la classe sir
std::vector<epidemia::State> simulazione = mondo.riempimento();

for (int i=0; i<simulazione.size(); ++i) { 
    
    std::cout << simulazione[i].suscettibili << "\n";
    std::cout << simulazione[i].infetti << "\n";
    std::cout << simulazione[i].rimossi << "\n" << "\n";

}


}