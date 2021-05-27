#include <vector>

#include "gnuplot-iostream.h"

int main() {
  Gnuplot g1;
  g1 << "set terminal jpeg\n";
  g1 << "set output 'prova.jpeg'\n";
  g1 << "set xrange [0:10] \n set yrange [0:100]\n";   // 10 numero dei giorni di simulazione
  g1 << "plot x+2";
}
