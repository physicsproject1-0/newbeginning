#include <vector>

#include "gnuplot-iostream.h"
#include "newSIR.hpp"

int main() {
  Gnuplot g1;

  std::vector<double> prova{1, 2, 4};
  g1 << "set terminal jpeg\n";
  g1 << "set output 'prova.jpeg'\n";
  // g1 << "set xrange [0:10] \n set yrange [0:100]\n";   // 10 numero dei giorni di simulazione
  // g1 << "plot x+2";
  //g1 << "set key fixed left top vertical Right noreverse enhanced autotitle box lt black linewidth 1.000 dashtype solid\n";
  g1 << "set title 'Simple Plots'\n";
  g1 << "set xrange [0:10] \n";
  g1 << "set yrange [0:10] \n";
  g1 << "plot" << g1.file1d(prova) << "with points\n";

}
