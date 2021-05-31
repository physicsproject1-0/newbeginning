#include <vector>

#include "gnuplot-iostream.h"
#include "newSIR.hpp"

namespace epidemia {

void grafico(std::vector<double> x, std::vector<double> y, std::vector<double> z) {
  Gnuplot g1;

  // g1 << "set terminal jpeg\n";
  // g1 << "set output 'prova.jpeg'\n";      
  g1 << "set title 'Modello SIR'\n";
  g1 << "set xrange [0:*] \n";                    //Da risolvere warning del range
  g1 << "plot" << g1.file1d(x) << "with linespoints title 'Suscettibili' linecolor rgb 'green'," << g1.file1d(y) 
     << "with linespoints title 'Infetti' linecolor rgb 'yellow'," << g1.file1d(z)
     << "with linespoints title 'Rimossi' linecolor rgb 'red'" << std::endl;
} 

}  // namespace epidemia 