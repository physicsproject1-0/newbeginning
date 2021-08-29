//#include <vector>   /// perchè avevamo inserito anche vector? senza funziona ugualmente
#include "gnuplot-iostream.h"
#include "newSIR.hpp"

namespace epidemia {

void grafico(std::vector<double> x, std::vector<double> y, std::vector<double> z) {
  Gnuplot g1;

  g1 << "set title 'Modello SIR'\n";
  g1 << "set xlabel 'Tempo (giorni)'\n set ylabel 'Parametri (unità)'\n";
  g1 << "set xrange [0:*] \n";
  g1 << "plot" << g1.file1d(x) << "with lines title 'Suscettibili' linecolor rgb 'dark-green' linewidth 2," << g1.file1d(y)
     << "with lines title 'Infetti' linecolor rgb 'dark-yellow' linewidth 2," << g1.file1d(z)
     << "with lines title 'Rimossi' linecolor rgb 'dark-red' linewidth 2" << std::endl;
}

void SIR::print_grafico(std::vector<State> vettore) {
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;

  for (auto const& i : vettore) {
    x.push_back(i.suscettibili);
    y.push_back(i.infetti);
    z.push_back(i.rimossi);
  }
  grafico(x, y, z);
}

}  // namespace epidemia