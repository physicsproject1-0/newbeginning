#include <SFML/Graphics.hpp>

#include "struct_enum.hpp"

#ifndef RIQUADRO_INFORMAZIONI_HPP
#define RIQUADRO_INFORMAZIONI_HPP

// Piccolo grafico in alto a destra. Segna l'andamento delle varie categorie
class Plot : public sf::Drawable {  // rispetto a punto in alto a destra
  sf::RectangleShape m_sfondo;
  sf::VertexArray m_assi;
  std::vector<std::pair<sf::VertexArray, std::vector<float>>> m_lista_linee;
  std::vector<sf::Color> m_colori_linee;

  float m_max_value_y;

 public:
  Plot(sf::Vector2f t_dimensione_grafico, sf::Vector2f t_posizione_alto_destra);
  Plot(sf::Vector2f t_dimensione_grafico);

  void draw(sf::RenderTarget& target, sf::RenderStates) const;

  void AggiornaPosizioni(sf::Vector2f t_punto_in_alto_destra);

  void NumeroLineee(int t_darmi_il_numero);

  void DefinisciColoreLinea(int t_linea, sf::Color t_colore);

  void AggiungiPunto(int t_numero_linea, float t_valore);

  void RiscalaPunti();
};

// Riporta i valori numerici per ogni categoria, contiene i grafici
class Informazioni : public sf::Drawable {  //rispetto a punto in alto a destra
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

  void draw(sf::RenderTarget& target, sf::RenderStates) const;

  void AggiornaPosizione(sf::Vector2f t_posizione_in_alto_dx);

  void AggiungiStatoAutoma(Censimento t_stato_automa);
  void AggiungiStatoAnimazione(Censimento t_stato_animazione);

  void MostraDatiAutoma();
  void MostraDatiAnimazione();

  void AggiornaScritte();
 

};

#endif