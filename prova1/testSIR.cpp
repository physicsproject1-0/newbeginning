#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "newSIR.hpp"

TEST_CASE("Testing stable situation") {
  epidemia::State start{100, 20, 0};  // 100 m_suscettibili, 20 m_infetti, 0 m_rimossi, giorno zero di default
  int giorni = 30;
  double beta = 0;
  double gamma = 0;
  int const people = 120;
  epidemia::SIR test{start, giorni, beta, gamma, people};

  auto risultato = test.Convertitore(test.Riempimento());
  epidemia::State finale = risultato.back();
  CHECK(finale.m_suscettibili == start.m_suscettibili);
  CHECK(finale.m_infetti == start.m_infetti);
}

TEST_CASE("Testing great infection") {
  epidemia::State start{100, 20, 0};
  int giorni = 30;
  double beta = 1;
  double gamma = 0;
  int const people = 120;
  epidemia::SIR test{start, giorni, beta, gamma, people};

  auto risultato = test.Convertitore(test.Riempimento());
  epidemia::State finale = risultato.back();
  CHECK(finale.m_suscettibili == 0);
  CHECK(finale.m_infetti == 120);
  CHECK(finale.m_rimossi == 0);
}
TEST_CASE("Testing great mortality") {
  epidemia::State start{100, 20, 0};
  int giorni = 30;
  double beta = 0;
  double gamma = 1;
  int const people = 120;
  epidemia::SIR test{start, giorni, beta, gamma, people};

  auto risultato = test.Convertitore(test.Riempimento());
  epidemia::State finale = risultato.back();
  CHECK(finale.m_suscettibili == 100);
  CHECK(finale.m_infetti == 0);
  CHECK(finale.m_rimossi == 20);
}

TEST_CASE("Testing end of the world") {
  epidemia::State start{100, 20, 0};
  int giorni = 1000;
  double beta = 1;
  double gamma = 0.01;
  int const people = 120;
  epidemia::SIR test{start, giorni, beta, gamma, people};

  auto risultato = test.Convertitore(test.Riempimento());
  epidemia::State finale = risultato.back();
  CHECK(finale.m_suscettibili == 0);
  CHECK(finale.m_infetti == 0);
  CHECK(finale.m_rimossi == 120);
}
