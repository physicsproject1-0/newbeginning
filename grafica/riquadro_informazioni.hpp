#include <SFML/Graphics.hpp>

#include "struct_enum.hpp"

#ifndef RIQUADRO_INFORMAZIONI_HPP
#define RIQUADRO_INFORMAZIONI_HPP

// Piccolo grafico in alto a destra. Segna l'andamento delle varie categorie
class Plot : public sf::Drawable {
  sf::RectangleShape m_sfondo;
  std::vector<std::pair<sf::VertexArray, std::vector<float>>> m_lista_linee;
  std::vector<sf::Color> m_colori_linee;
  float m_max_value_y;

  bool da_svuotare;

  sf::VertexArray m_assi;

 public:
  Plot(sf::Vector2f t_dimensione_grafico, sf::Vector2f t_posizione_alto_destra);
  Plot(sf::Vector2f t_dimensione_grafico);

  void AggiornaPosizioni(sf::Vector2f t_punto_in_alto_destra);

  void NumeroLineee(int darmi_il_numero);

  void DefinisciColoreLinea(int t_linea, sf::Color t_colore);

  void AggiungiPunto(int numero_linea, float t_valore);

  void RiscalaPunti();

  void draw(sf::RenderTarget& target, sf::RenderStates) const;

  void Svuota();
};

// Riporta i valori numerici per ogni categoria
class Informazioni : public sf::Drawable {
  Plot m_grafico_automa;
  Plot m_grafico_animazione;
  bool m_animazione_attiva;
  bool m_automa_attivo;
  bool m_appena_azzerato;

  sf::RectangleShape m_riquadro_esterno;

  sf::Text m_testo_vulnerabili;
  sf::Text m_testo_infetti;
  sf::Text m_testo_guariti;
  sf::Text m_testo_morti;

  std::vector<Censimento> m_successione_stati_automa;
  std::vector<Censimento> m_successione_stati_animazione;

 public:
  Informazioni(sf::Vector2f t_dimensioni, sf::Font* t_font);

  void AggiornaPosizione(sf::Vector2f t_posizione_in_alto_dx);

  void AggiungiStatoAutoma(Censimento t_stato_automa);

  void AggiungiStatoAnimazione(Censimento t_stato_animazione);

  void MostraDatiAutoma();

  void MostraDatiAnimazione();

  void AggiornaScritte();
  void AzzeraAnimazione();
  void AzzeraAutoma();
  void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif