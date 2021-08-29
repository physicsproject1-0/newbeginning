#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>

#include "gui.hpp"

#ifndef AUTOMA_HPP
#define AUTOMA_HPP

class Cellula : public Bordi {
 public:
  Cellula(sf::Vector2f m_posizione, sf::Vector2f dimensione);

  void AggiornaColore();

  int m_counter;
  Stato m_stato = Stato::VULNERABILE;
};

class Automa : public sf::Drawable {
  std::vector<std::vector<Cellula>> m_grid;

  sf::Vector2f m_posizione;
  sf::Vector2f m_dimensioni;

  int m_numero_lato;

  Censimento m_censimento;

  float m_probabilita_contagio;
  float m_probabilita_guarigione;
  float m_probabilita_morte;

  Bordi m_limite;
  bool m_is_stopped;

  sf::Clock m_orologio;

 public:
  Automa(sf::Vector2f t_posizione, sf::Vector2f t_dimensione, int t_numero, float t_probabilita_contagio, float t_probabilita_guarigione,
         float t_probabilita_morte, int t_infetti, int t_rimossi);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;

  Bordi GetBordi();

  void Genera(int t_infetti, int t_rimossi);
  bool Esiste(int t_i, int t_j);

  void Aggiorna_counter(int t_i, int t_j);
  void Aggiorna();
  void Avanza();

  std::pair<int, int> CheckMousePosition(sf::Vector2f t_coordinate_mouse);
  void ChangeStatus(std::pair<int, int> t_coordinate, Stato t_stato);

  void AzzeraOrologio();

  void StopAutoma();
  void StartAutoma();
  bool IsStopped();

  Censimento GetCensimento();

  void AggiornaSenzaAvanzare();
};

#endif
