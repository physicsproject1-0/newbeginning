#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "newSIR.hpp"

TEST_CASE("Testing State") {
  epidemia::State start {100, 10, 0, 0};
  int giorni = 30;
  double beta = 0;
  double gamma = 0;
  int const people = 110;
  epidemia::SIR test { start, giorni, beta, gamma, people};
  
  auto risultato = test.convertitore(test.riempimento());
  epidemia::State finale = risultato.back();
  CHECK(finale.infetti == start.infetti);
  //CHECK();
  //CHECK();
}