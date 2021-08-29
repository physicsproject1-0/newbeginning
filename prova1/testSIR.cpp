#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "newSIR.hpp"

TEST_CASE("Testing stable situation") {
  epidemia::State start{100, 20, 0};  // 100 suscettibili, 10 infetti, 0 rimossi, giorno zero di default
  int giorni = 30;
  double beta = 0;
  double gamma = 0;
  int const people = 120;
  epidemia::SIR test{start, giorni, beta, gamma, people};

  auto risultato = test.convertitore(test.riempimento());
  epidemia::State finale = risultato.back();
  CHECK(finale.suscettibili == start.suscettibili);
  CHECK(finale.infetti == start.infetti);
}

TEST_CASE("Testing great infection") {
  epidemia::State start{100, 20, 0};
  int giorni = 30;
  double beta = 1;
  double gamma = 0;
  int const people = 120;
  epidemia::SIR test{start, giorni, beta, gamma, people};

  auto risultato = test.convertitore(test.riempimento());
  epidemia::State finale = risultato.back();
  CHECK(finale.suscettibili == 0);
  CHECK(finale.infetti == 120);
  CHECK(finale.rimossi == 0);
}
TEST_CASE("Testing great mortality") {
  epidemia::State start{100, 20, 0};
  int giorni = 30;
  double beta = 0;
  double gamma = 1;
  int const people = 120;
  epidemia::SIR test{start, giorni, beta, gamma, people};

  auto risultato = test.convertitore(test.riempimento());
  epidemia::State finale = risultato.back();
  CHECK(finale.suscettibili == 100);
  CHECK(finale.infetti == 0);
  CHECK(finale.rimossi == 20);
}

TEST_CASE("Testing end of the world") {
  epidemia::State start{100, 20, 0};
  int giorni = 1000;
  double beta = 1;
  double gamma = 0.01;
  int const people = 120;
  epidemia::SIR test{start, giorni, beta, gamma, people};

  auto risultato = test.convertitore(test.riempimento());
  epidemia::State finale = risultato.back();
  CHECK(finale.suscettibili == 0);
  CHECK(finale.infetti == 0);
  CHECK(finale.rimossi == 120);
}
