
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
#include <random>
#include <stdexcept>
#include <string>

#ifndef GUI_HPP
#define GUI_HPP

enum class Vista { Automa, Animazione };

enum class MousePos {
  None,
  CheckboxAnimazione,
  CheckboxAutoma,
  PulsantePlay,
  PulsantePausa,
  PalettaVulnerabile,
  PalettaInfetto,
  PalettaGuariti,
  PalettaMorti
};

enum class Stato { VULNERABILE, INFETTO, MORTO, GUARITO };

struct Censimento {
  int m_suscettibili;
  int m_infetti;
  int m_guariti;
  int m_morti;
};

template <typename C>
void censimento(C const& cell, Censimento& popolazione) {
  switch (cell.m_S) {
    case (Stato::VULNERABILE):
      popolazione.m_suscettibili++;
      break;

    case (Stato::INFETTO):
      popolazione.m_infetti++;
      break;

    case (Stato::GUARITO):
      popolazione.m_guariti++;
      break;
    case (Stato::MORTO):
      popolazione.m_morti++;
      break;
  }
}
class Bordi : public sf::Drawable {
 protected:
  sf::RectangleShape m_rettangolo;
  sf::FloatRect m_rettangolo_esterno;

 public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  Bordi(sf::Vector2f dimensione, sf::Vector2f posizione);
  Bordi(sf::Vector2f dimensione);
  virtual void set_posizione(sf::Vector2f posizione);
  void ridimensiona(sf::Vector2f dimensione) {
    m_rettangolo.setSize(dimensione);
    m_rettangolo_esterno.width = dimensione.x;
    m_rettangolo_esterno.height = dimensione.y;
  }
  void set_color(sf::Color Colore);
  sf::FloatRect getlimiti() const;
};

class Checkbox : public Bordi {  // origine al centro
 protected:
  bool m_clicked = false;

 public:
  virtual void set_posizione(sf::Vector2f posizione) {
    m_rettangolo.setPosition(posizione);

    m_rettangolo_esterno.left = posizione.x - m_rettangolo_esterno.width / 2;
    m_rettangolo_esterno.top = posizione.y - m_rettangolo_esterno.height / 2;  // aggiustato cambio coordinate
  }
  bool return_status() { return m_clicked; }
  void change_status(bool var) { m_clicked = var; }
  Checkbox(sf::Vector2f dimensione);
  void setinterncolor(sf::Color colore);
  void ImpostaOrigine(sf::Vector2f t_nuova_origine) { m_rettangolo.setOrigin(t_nuova_origine); }
};

class Textbox : public Bordi {  // origine al centro //chiamare pprima scrivi e poi set posizione
  sf::Font* m_font;
  sf::Text m_testo;
  float m_fattore_conversione;  // dimensioni lettera testo lungo asse x rispetto ad asse y

 public:
  Textbox(int dimensione_carattere, sf::Font* t_font, sf::Color colore_testo)
      : Bordi(sf::Vector2f(dimensione_carattere, dimensione_carattere)), m_font{t_font}, m_fattore_conversione{0.6} {  // chiamare il costruttore di
                                                                                                                       // bordi
    m_testo.setFillColor(colore_testo);
    m_testo.setFont(*m_font);
    m_testo.setCharacterSize(dimensione_carattere /* 4 */);

    m_rettangolo.setFillColor(sf::Color(211, 211, 211));

    m_rettangolo.setOutlineThickness(1);  // lo spessore più piccolo altrimenti non si vede
  }

  void set_posizione(sf::Vector2f posizione) {
    m_rettangolo.setPosition(posizione);
    m_testo.setPosition(posizione);
  }

  void scrivi(std::string stringa) {
    m_testo.setString(stringa);
    sf::Vector2f dimensioni_testo(
        m_testo.getGlobalBounds().width +
            1 /* m_testo.getCharacterSize() * stringa.size() * m_fattore_conversione */,  // fattore di conversione per dimensione x
        m_testo.getGlobalBounds().height + 3);                                            // ci sono anche le funzioni getlocal e... ma sono imprecise
    m_testo.setOrigin(dimensioni_testo.x / 2, dimensioni_testo.y / 2);
    /* m_testo.scale(0.25, 0.25); */

    std::cout << "width" << m_testo.getGlobalBounds().width << "height" << m_testo.getCharacterSize() << '\n';
    /* sf::Vector2f dimensioni_testo_ridotte(
        m_testo.getGlobalBounds().width +
            4 /* m_testo.getCharacterSize() * stringa.size() * m_fattore_conversione ,  // fattore di conversione per dimensione x
        m_testo.getGlobalBounds().height + 4);
 */
    ridimensiona(dimensioni_testo);

    m_rettangolo.setOrigin(dimensioni_testo.x / 2, dimensioni_testo.y / 2);
  }
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_rettangolo);
    target.draw(m_testo);
  }
};

enum class TipoPulsante { Play, Pausa };

class Pulsante : public Checkbox {  // origine al centro
  TipoPulsante m_tipo_pulsante;
  sf::CircleShape m_simbolo;

 public:
  Pulsante(float lato, TipoPulsante t_tipopulsante) : Checkbox(sf::Vector2f(lato, lato)), m_tipo_pulsante{t_tipopulsante} {
    m_simbolo.setRadius(lato / 4);
    if (m_tipo_pulsante == TipoPulsante::Play) {
      m_simbolo.setPointCount(3);
      m_simbolo.setRotation(90);  // senso orario
    } else {
      m_simbolo.setPointCount(4);
      m_simbolo.setRotation(-45);
    }
    m_simbolo.setFillColor(sf::Color::Black);
    m_simbolo.setOrigin(sf::Vector2f(lato / 4, lato / 4));
  }

  void set_posizione(sf::Vector2f posizione) {
    m_rettangolo.setPosition(posizione);
    m_simbolo.setPosition(posizione);

    m_rettangolo_esterno.left = posizione.x - m_rettangolo_esterno.width / 2;
    m_rettangolo_esterno.top = posizione.y - m_rettangolo_esterno.height / 2;
  }

  void AggiornaStato() {  // se cliccato
  }
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_rettangolo);
    target.draw(m_simbolo);
  }
};

// fare classe da ereditare di cose che si possono attivare o no

class Paletta : public sf::Drawable {
  Checkbox m_box_verde;
  Checkbox m_box_rosso;
  Checkbox m_box_blu;
  Checkbox m_box_bianco;

  Bordi m_riquadro_esterno;

  sf::RectangleShape m_rettangolo_opaco;

  sf::RectangleShape m_dainserire_verde;
  sf::RectangleShape m_dainserire_rosso;
  sf::RectangleShape m_dainserire_blu;
  sf::RectangleShape m_dainserire_bianco;

  Textbox m_testo_verde;
  Textbox m_testo_rosso;
  Textbox m_testo_blu;
  Textbox m_testo_bianco;

  bool m_is_active;           // si intende se si può cliccare
  bool m_inserimento_attivo;  // se hai preso il rettangolo
  bool m_verde_visibile;
  bool m_rosso_visibile;
  bool m_blu_visibile;
  bool m_bianco_visibile;

 public:
  Paletta(sf::Vector2f t_dimensione_riquadro, sf::Font* t_font)
      : m_box_verde(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_box_rosso(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_box_blu(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_box_bianco(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.x / 2)),
        m_riquadro_esterno(t_dimensione_riquadro),

        m_testo_verde(9.5, t_font, sf::Color::Black),
        m_testo_rosso(9.5, t_font, sf::Color::Black),
        m_testo_blu(9.5, t_font, sf::Color::Black),
        m_testo_bianco(9.5, t_font, sf::Color::Black),
        m_is_active{true},
        m_inserimento_attivo{false},
        m_verde_visibile{false},
        m_rosso_visibile{false},
        m_blu_visibile{false},
        m_bianco_visibile{false} {
    m_rettangolo_opaco.setFillColor(sf::Color(128, 128, 128, 120));
    m_rettangolo_opaco.setSize(t_dimensione_riquadro);
    m_rettangolo_opaco.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 2, t_dimensione_riquadro.y / 2));

    m_box_verde.setinterncolor(sf::Color::Green);
    m_box_verde.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_box_rosso.setinterncolor(sf::Color::Red);
    m_box_rosso.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_box_blu.setinterncolor(sf::Color::Cyan);
    m_box_blu.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_box_bianco.setinterncolor(sf::Color::White);
    m_box_bianco.ImpostaOrigine(sf::Vector2f(t_dimensione_riquadro.x / 4, t_dimensione_riquadro.x / 4));

    m_dainserire_verde.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_verde.setFillColor(sf::Color::Green);
    m_dainserire_verde.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));

    m_dainserire_rosso.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_rosso.setFillColor(sf::Color::Red);
    m_dainserire_rosso.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));

    m_dainserire_blu.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_blu.setFillColor(sf::Color::Cyan);
    m_dainserire_blu.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));

    m_dainserire_bianco.setSize(sf::Vector2f(t_dimensione_riquadro.x / 6, t_dimensione_riquadro.x / 6));
    m_dainserire_bianco.setFillColor(sf::Color::White);
    m_dainserire_bianco.setOrigin(sf::Vector2f(t_dimensione_riquadro.x / 12, t_dimensione_riquadro.x / 12));
  }

  void ImpostaPosizione(sf::Vector2f t_posizione_centro) {  // la consideriamo rispetto al centro
    m_riquadro_esterno.set_posizione(t_posizione_centro -
                                     sf::Vector2f(m_riquadro_esterno.getlimiti().width / 2, m_riquadro_esterno.getlimiti().height / 2));

    m_rettangolo_opaco.setPosition(t_posizione_centro);

    m_box_verde.set_posizione(
        sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.getlimiti().top + (m_riquadro_esterno.getlimiti().height / 5) * 1));
    m_box_rosso.set_posizione(
        sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.getlimiti().top + (m_riquadro_esterno.getlimiti().height / 5) * 2));
    m_box_blu.set_posizione(sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.getlimiti().top + (m_riquadro_esterno.getlimiti().height / 5) * 3));
    m_box_bianco.set_posizione(
        sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.getlimiti().top + (m_riquadro_esterno.getlimiti().height / 5) * 4));

    m_testo_verde.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_verde.getlimiti().top - 10));
    m_testo_rosso.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_rosso.getlimiti().top - 10));
    m_testo_blu.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_blu.getlimiti().top - 10));
    m_testo_bianco.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_bianco.getlimiti().top - 10));
  }
  void Scrivi() {  // funzione presente altrimenti scrive prima di caricare il font e mi restituisce 0 di larghezza , il loading del font avviene dopo
                   // in GUI
    m_testo_verde.scrivi("Vulnerabile");
    m_testo_rosso.scrivi("Infetto");
    m_testo_blu.scrivi("Guarito");
    m_testo_bianco.scrivi("Morto");

    if (m_testo_verde.getlimiti().width > m_riquadro_esterno.getlimiti().width ||
        m_testo_rosso.getlimiti().width > m_riquadro_esterno.getlimiti().width ||
        m_testo_blu.getlimiti().width > m_riquadro_esterno.getlimiti().width ||
        m_testo_bianco.getlimiti().width > m_riquadro_esterno.getlimiti().width) {
      throw std::runtime_error("dimensione paletta non sufficiente per ospitare testo");
    }
  }
  void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_riquadro_esterno);

    target.draw(m_box_verde);
    target.draw(m_box_rosso);
    target.draw(m_box_blu);
    target.draw(m_box_bianco);

    target.draw(m_testo_verde);
    target.draw(m_testo_rosso);
    target.draw(m_testo_blu);
    target.draw(m_testo_bianco);

    if (!m_is_active) {
      target.draw(m_rettangolo_opaco);
    }

    if (m_inserimento_attivo) {
      if (m_verde_visibile) {
        target.draw(m_dainserire_verde);
      }
      if (m_rosso_visibile) {
        target.draw(m_dainserire_rosso);
      }
      if (m_blu_visibile) {
        target.draw(m_dainserire_blu);
      }
      if (m_bianco_visibile) {
        target.draw(m_dainserire_bianco);
      }
    }
  }

  void AggiornaPosizioneRettangoliDaInserire(sf::Vector2f t_posizione_mouse) {
    m_dainserire_verde.setPosition(t_posizione_mouse);
    m_dainserire_rosso.setPosition(t_posizione_mouse);
    m_dainserire_blu.setPosition(t_posizione_mouse);
    m_dainserire_bianco.setPosition(t_posizione_mouse);
  }

  bool IsActive() { return m_is_active; }

  void Attiva() { m_is_active = true; }

  void Disattiva() { m_is_active = false; }

  Checkbox RitornaVerde() { return m_box_verde; }
  Checkbox RitornaRosso() { return m_box_rosso; }
  Checkbox RitornaBlu() { return m_box_blu; }
  Checkbox RitornaBianco() { return m_box_bianco; }

  bool IsInserimentoAttivo() { return m_inserimento_attivo; }
  void AttivaInserimento() { m_inserimento_attivo = true; }
  void DisattivaInserimento() { m_inserimento_attivo = false; }

  Stato RitornaStatoRettangoloInserendo() {
    if (m_verde_visibile) {
      return Stato::VULNERABILE;
    } else if (m_rosso_visibile) {
      return Stato::INFETTO;
    } else if (m_blu_visibile) {
      return Stato::GUARITO;
    } else if (m_bianco_visibile) {
      return Stato::MORTO;
    } else {
      throw std::runtime_error("non stai inserendo nessun rettangolo");
    }
  }

  void AttivaVerde() {
    m_verde_visibile = true;
    m_rosso_visibile = false;
    m_blu_visibile = false;
    m_bianco_visibile = false;
  }
  void AttivaRosso() {
    m_verde_visibile = false;
    m_rosso_visibile = true;
    m_blu_visibile = false;
    m_bianco_visibile = false;
  }
  void AttivaBlu() {
    m_verde_visibile = false;
    m_rosso_visibile = false;
    m_blu_visibile = true;
    m_bianco_visibile = false;
  }
  void AttivaBianco() {
    m_verde_visibile = false;
    m_rosso_visibile = false;
    m_blu_visibile = false;
    m_bianco_visibile = true;
  }
  /*  bool Contiene(sf::Vector2f t_coordinate_mouse, MousePos& t_posizione_mouse) {
     if (m_box_verde.getlimiti().contains(t_coordinate_mouse)) {
       m_posizione_mouse = MousePos::CheckboxAnimazione;
       std::cout << "sei sopra la casella animazione" << '\n';

     } else if (m_box_rosso.contains(t_coordinate_mouse)) {
       m_posizione_mouse = MousePos::CheckboxAutoma;
       std::cout << "sei sopra la casella automa" << '\n';

     } else if (m_box_blu.contains(t_coordinate_mouse)) {
       m_posizione_mouse = MousePos::PulsantePlay;
       std::cout << "sei sopra la casella play" << '\n';

     } else if (m_riquadro_esterno.contains(t_coordinate_mouse)) {
       m_posizione_mouse = MousePos::PulsantePausa;
       std::cout << "sei sopra la casella pausa" << '\n';

     } else {
       m_posizione_mouse = MousePos::None;
       std::cout << "nope" << '\n';
     }
   } */
};
/*
sf::Vector2f operator/(const int& t_numero) {}   come si overloada???
 */

class Plot : public sf::Drawable {
  sf::RectangleShape m_sfondo;
  std::vector<std::pair<sf::VertexArray, std::vector<float>>> m_lista_linee;
  std::vector<sf::Color> m_colori_linee;
  float m_max_value_y;

  bool da_svuotare;

  sf::VertexArray m_assi;

 public:
  Plot(sf::Vector2f t_dimensione_grafico, sf::Vector2f t_posizione_alto_destra) : m_max_value_y{0}, da_svuotare{false} {
    m_sfondo.setFillColor(sf::Color(128, 128, 128));
    m_sfondo.setOutlineColor(sf::Color::White);
    m_sfondo.setOutlineThickness(3.f);
    m_sfondo.setSize(t_dimensione_grafico);
    m_sfondo.setOrigin(sf::Vector2f(t_dimensione_grafico.x, 0));
    m_sfondo.setPosition(t_posizione_alto_destra);

    m_assi.resize(3);
    m_assi.setPrimitiveType(sf::LineStrip);
    AggiornaPosizioni(t_posizione_alto_destra);
    m_assi[0].color = sf::Color::White;
    m_assi[1].color = sf::Color::White;
    m_assi[2].color = sf::Color::White;
  }
  Plot(sf::Vector2f t_dimensione_grafico) : m_max_value_y{0} {
    m_sfondo.setFillColor(sf::Color(128, 128, 128));
    m_sfondo.setOutlineColor(sf::Color::White);
    m_sfondo.setOutlineThickness(3.f);
    m_sfondo.setSize(t_dimensione_grafico);
    m_sfondo.setOrigin(sf::Vector2f(t_dimensione_grafico.x, 0));

    m_assi.resize(3);
    m_assi.setPrimitiveType(sf::LineStrip);
    m_assi[0].color = sf::Color::White;
    m_assi[1].color = sf::Color::White;
    m_assi[2].color = sf::Color::White;
  }

  void AggiornaPosizioni(sf::Vector2f t_punto_in_alto_destra) {
    m_sfondo.setPosition(t_punto_in_alto_destra);

    m_assi[0].position =
        sf::Vector2f(t_punto_in_alto_destra.x - ((m_sfondo.getSize().x / 10) * 9), t_punto_in_alto_destra.y + (m_sfondo.getSize().y / 10));
    m_assi[1].position =
        sf::Vector2f(t_punto_in_alto_destra.x - ((m_sfondo.getSize().x / 10) * 9), t_punto_in_alto_destra.y + ((m_sfondo.getSize().y / 10) * 9));
    m_assi[2].position =
        sf::Vector2f(t_punto_in_alto_destra.x - (m_sfondo.getSize().x / 10), t_punto_in_alto_destra.y + (m_sfondo.getSize().y / 10) * 9);

    RiscalaPunti();
  }

  void NumeroLineee(int darmi_il_numero) {
    m_lista_linee.resize(darmi_il_numero);
    m_colori_linee.resize(darmi_il_numero);
    for (long unsigned int i = 0; i < m_lista_linee.size(); i++) {
      m_lista_linee[i].first.setPrimitiveType(sf::LineStrip);
    }
  }

  void DefinisciColoreLinea(int t_linea, sf::Color t_colore) { m_colori_linee[t_linea] = t_colore; }

  void AggiungiPunto(int numero_linea, float t_valore) {
    /* if (da_svuotare) {
      m_lista_linee[numero_linea].first.resize(0);
      m_lista_linee[numero_linea].first.append(
          sf::Vertex(sf::Vector2f(m_lista_linee[numero_linea].second.size(), t_valore), m_colori_linee[numero_linea]));
      m_lista_linee[numero_linea].second.clear();
      m_lista_linee[numero_linea].second.push_back(t_valore);
      m_max_value_y = std::max(m_max_value_y, t_valore);
      RiscalaPunti();
      da_svuotare=false; */
    /*  } else { */
    m_lista_linee[numero_linea].first.append(
        sf::Vertex(sf::Vector2f(m_lista_linee[numero_linea].second.size(), t_valore), m_colori_linee[numero_linea]));
    m_lista_linee[numero_linea].second.push_back(t_valore);
    m_max_value_y = std::max(m_max_value_y, t_valore);
    RiscalaPunti();
  }

  void RiscalaPunti() {
    sf::Vector2f t_posizione_origine = m_assi[1].position;
    for (long unsigned int j = 0; j < m_lista_linee.size(); j++) {
      for (long unsigned int i = 0; i < m_lista_linee[j].second.size(); i++) {
        sf::Vector2f t_posizione_scalata(((m_assi[2].position.x - m_assi[1].position.x) / m_lista_linee[j].second.size()) * i,
                                         ((m_assi[1].position.y - m_assi[0].position.y) / m_max_value_y) * m_lista_linee[j].second[i]);
        m_lista_linee[j].first[i].position =
            sf::Vector2f(t_posizione_origine.x + t_posizione_scalata.x, t_posizione_origine.y - t_posizione_scalata.y);
      }
    }
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sfondo);
    for (long unsigned int i = 0; i < m_lista_linee.size(); i++) {
      target.draw(m_lista_linee[i].first);
    }
    target.draw(m_assi);
  }

  void Svuota() {
    /* for (int i = 0; i < m_lista_linee.size(); i++) {
      m_lista_linee[i].first.clear();
      m_lista_linee[i].second.clear();
    } */
    /*  m_lista_linee.clear(); */
    da_svuotare = true;
    m_max_value_y = 0;
  }
};

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
  Informazioni(sf::Vector2f t_dimensioni, /* sf::Vector2f t_posizione_alto_dx, */ sf::Font* t_font)
      : m_grafico_automa{t_dimensioni},
        m_grafico_animazione{t_dimensioni},
        m_animazione_attiva{true},
        m_automa_attivo{false},
        m_appena_azzerato{false} {
    m_riquadro_esterno.setFillColor(sf::Color(128, 128, 128));
    m_riquadro_esterno.setOutlineColor(sf::Color::White);
    m_riquadro_esterno.setOutlineThickness(3.f);
    m_riquadro_esterno.setSize(t_dimensioni);
    m_riquadro_esterno.setOrigin(sf::Vector2f(t_dimensioni.x, 0));
    /* m_riquadro_esterno.setPosition(t_posizione_alto_dx); */

    m_testo_vulnerabili.setFont(*t_font);
    m_testo_infetti.setFont(*t_font);
    m_testo_guariti.setFont(*t_font);
    m_testo_morti.setFont(*t_font);

    m_testo_vulnerabili.setCharacterSize(10);
    m_testo_infetti.setCharacterSize(10);
    m_testo_guariti.setCharacterSize(10);
    m_testo_morti.setCharacterSize(10);

    m_testo_vulnerabili.setFillColor(sf::Color::Black);
    m_testo_infetti.setFillColor(sf::Color::Black);
    m_testo_guariti.setFillColor(sf::Color::Black);
    m_testo_morti.setFillColor(sf::Color::Black);

    /* m_testo_vulnerabili.setPosition(sf::Vector2f(t_posizione_alto_dx.x - t_dimensioni.x + 10, t_posizione_alto_dx.y - t_dimensioni.y / 5));
    m_testo_infetti.setPosition(sf::Vector2f(t_posizione_alto_dx.x - t_dimensioni.x + 10, t_posizione_alto_dx.y - (t_dimensioni.y / 5) * 2));
    m_testo_guariti.setPosition(sf::Vector2f(t_posizione_alto_dx.x - t_dimensioni.x + 10, t_posizione_alto_dx.y - (t_dimensioni.y / 5) * 3));
    m_testo_morti.setPosition(sf::Vector2f(t_posizione_alto_dx.x - t_dimensioni.x + 10, t_posizione_alto_dx.y - (t_dimensioni.y / 5) * 4)); */

    m_testo_vulnerabili.setString("Vulnerabili: ");
    m_testo_infetti.setString("Infetti: ");
    m_testo_guariti.setString("Guariti: ");
    m_testo_morti.setString("Morti: ");

    m_grafico_animazione.NumeroLineee(4);
    m_grafico_animazione.DefinisciColoreLinea(0, sf::Color::Green);
    m_grafico_animazione.DefinisciColoreLinea(1, sf::Color::Red);
    m_grafico_animazione.DefinisciColoreLinea(2, sf::Color::Cyan);
    m_grafico_animazione.DefinisciColoreLinea(3, sf::Color::White);

    m_grafico_automa.NumeroLineee(4);
    m_grafico_automa.DefinisciColoreLinea(0, sf::Color::Green);
    m_grafico_automa.DefinisciColoreLinea(1, sf::Color::Red);
    m_grafico_automa.DefinisciColoreLinea(2, sf::Color::Cyan);
    m_grafico_automa.DefinisciColoreLinea(3, sf::Color::White);
  }

  void AggiornaPosizione(sf::Vector2f t_posizione_in_alto_dx) {
    m_riquadro_esterno.setPosition(t_posizione_in_alto_dx);
    m_testo_vulnerabili.setPosition(
        sf::Vector2f(t_posizione_in_alto_dx.x - m_riquadro_esterno.getSize().x + 10, t_posizione_in_alto_dx.y + m_riquadro_esterno.getSize().y / 5));
    m_testo_infetti.setPosition(sf::Vector2f(t_posizione_in_alto_dx.x - m_riquadro_esterno.getSize().x + 10,
                                             t_posizione_in_alto_dx.y + (m_riquadro_esterno.getSize().y / 5) * 2));
    m_testo_guariti.setPosition(sf::Vector2f(t_posizione_in_alto_dx.x - m_riquadro_esterno.getSize().x + 10,
                                             t_posizione_in_alto_dx.y + (m_riquadro_esterno.getSize().y / 5) * 3));
    m_testo_morti.setPosition(sf::Vector2f(t_posizione_in_alto_dx.x - m_riquadro_esterno.getSize().x + 10,
                                           t_posizione_in_alto_dx.y + (m_riquadro_esterno.getSize().y / 5) * 4));
    m_grafico_animazione.AggiornaPosizioni(sf::Vector2f(t_posizione_in_alto_dx.x, t_posizione_in_alto_dx.y + m_riquadro_esterno.getSize().y));
    m_grafico_automa.AggiornaPosizioni(sf::Vector2f(t_posizione_in_alto_dx.x, t_posizione_in_alto_dx.y + m_riquadro_esterno.getSize().y));
  }

  void AggiungiStatoAutoma(Censimento t_stato_automa) {
    if (m_appena_azzerato) {
      m_successione_stati_automa[0] = t_stato_automa;
      m_grafico_automa.AggiungiPunto(0, t_stato_automa.m_suscettibili);
      m_grafico_automa.AggiungiPunto(1, t_stato_automa.m_infetti);
      m_grafico_automa.AggiungiPunto(2, t_stato_automa.m_guariti);
      m_grafico_automa.AggiungiPunto(3, t_stato_automa.m_morti);
      m_appena_azzerato = false;
    } else {
      m_successione_stati_automa.push_back(t_stato_automa);
      m_grafico_automa.AggiungiPunto(0, t_stato_automa.m_suscettibili);
      m_grafico_automa.AggiungiPunto(1, t_stato_automa.m_infetti);
      m_grafico_automa.AggiungiPunto(2, t_stato_automa.m_guariti);
      m_grafico_automa.AggiungiPunto(3, t_stato_automa.m_morti);
    }
  }

  void AggiungiStatoAnimazione(Censimento t_stato_animazione) {
    m_successione_stati_animazione.push_back(t_stato_animazione);
    m_grafico_animazione.AggiungiPunto(0, t_stato_animazione.m_suscettibili);
    m_grafico_animazione.AggiungiPunto(1, t_stato_animazione.m_infetti);
    m_grafico_animazione.AggiungiPunto(2, t_stato_animazione.m_guariti);
    m_grafico_animazione.AggiungiPunto(3, t_stato_animazione.m_morti);
  }

  void MostraDatiAutoma() {
    m_automa_attivo = true;
    m_animazione_attiva = false;
  }
  void MostraDatiAnimazione() {
    m_automa_attivo = false;
    m_animazione_attiva = true;
  }
  void AggiornaScritte() {
    if (m_automa_attivo) {
      if (!m_successione_stati_automa.empty()) {
        m_testo_vulnerabili.setString("Vulnerabili: " + std::to_string(m_successione_stati_automa.back().m_suscettibili));
        m_testo_infetti.setString("Infetti: " + std::to_string(m_successione_stati_automa.back().m_infetti));
        m_testo_guariti.setString("Guariti: " + std::to_string(m_successione_stati_automa.back().m_guariti));
        m_testo_morti.setString("Morti: " + std::to_string(m_successione_stati_automa.back().m_morti));
      }
    } else {
      if (!m_successione_stati_animazione.empty()) {
        m_testo_vulnerabili.setString("Vulnerabili: " + std::to_string(m_successione_stati_animazione.back().m_suscettibili));
        m_testo_infetti.setString("Infetti: " + std::to_string(m_successione_stati_animazione.back().m_infetti));
        m_testo_guariti.setString("Guariti: " + std::to_string(m_successione_stati_animazione.back().m_guariti));
        m_testo_morti.setString("Morti: " + std::to_string(m_successione_stati_animazione.back().m_morti));
      }
    }
  }
  void AzzeraAnimazione() {
    m_grafico_animazione.Svuota();
    m_successione_stati_animazione.clear();
    m_appena_azzerato = true;
  }
  void AzzeraAutoma() {
    m_grafico_automa.Svuota();
    m_successione_stati_automa.clear();
    m_appena_azzerato = true;
  }
  void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_riquadro_esterno);
    if (m_automa_attivo) {
      target.draw(m_grafico_automa);

    } else {
      target.draw(m_grafico_animazione);
    }

    target.draw(m_testo_vulnerabili);

    target.draw(m_testo_infetti);

    target.draw(m_testo_guariti);
    target.draw(m_testo_morti);
  }
};

class GUI : public sf::Drawable {
  sf::Font m_font;

  sf::RectangleShape m_sfondo_grigio;  // non uso nè bordi nè checkbox perchè non adatti
  Bordi m_limiti_sfondo_grigio;

  Textbox m_testo_animazione;
  Textbox m_testo_automa;

  Checkbox m_casella_animazione;
  Checkbox m_casella_automa;

  Pulsante m_pulsante_play;
  Pulsante m_pulsante_pausa;

  Paletta m_paletta_colori;

  Informazioni m_riquadro_informazioni;

  MousePos m_posizione_mouse;

 public:
  GUI(sf::Vector2f dimensione);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void aggiorna_posizione(sf::Vector2f punto_in_altodx, sf::Vector2f dimensioni_finestra);

  void AggiornaPosizioneRettangoliPaletta(sf::Vector2f t_coordinate_mouse) {
    if (m_paletta_colori.IsInserimentoAttivo()) {
      m_paletta_colori.AggiornaPosizioneRettangoliDaInserire(t_coordinate_mouse);
    }
  }

  void AttivaInserimento() { m_paletta_colori.Attiva(); }

  void DisattivaInserimento() { m_paletta_colori.Disattiva(); }

  void check_mouse_position(sf::Vector2f t_coordinate_mouse) {
    if (m_casella_animazione.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::CheckboxAnimazione;

    } else if (m_casella_automa.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::CheckboxAutoma;

    } else if (m_pulsante_play.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::PulsantePlay;

    } else if (m_pulsante_pausa.getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::PulsantePausa;
    } else if (m_paletta_colori.RitornaVerde().getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::PalettaVulnerabile;
    } else if (m_paletta_colori.RitornaRosso().getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::PalettaInfetto;
    } else if (m_paletta_colori.RitornaBlu().getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::PalettaGuariti;
    } else if (m_paletta_colori.RitornaBianco().getlimiti().contains(t_coordinate_mouse)) {
      m_posizione_mouse = MousePos::PalettaMorti;
    } else {
      m_posizione_mouse = MousePos::None;
    }
  }

  void check_color(sf::Color colore) {
    switch (m_posizione_mouse) {
      case MousePos::CheckboxAnimazione:
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(colore);
        }
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }

        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
        }

        break;

      case MousePos::CheckboxAutoma:
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(colore);
        }
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
        }

        break;
      case MousePos::PulsantePlay:
        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(colore);
        }
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
        }

        break;
      case MousePos::PulsantePausa:
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(colore);
        }
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
        }

        break;

      case MousePos::None:
        if (!m_casella_automa.return_status()) {
          m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_casella_animazione.return_status()) {
          m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_play.return_status()) {
          m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
        }
        if (!m_pulsante_pausa.return_status()) {
          m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
        }
        break;
    }
  }

  /*  void ControlloGiaCliccata(Checkbox t_checkbox) {
     if (!m_casella_automa.return_status()) {
           m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
         }
   } */

  MousePos mouse_clicked() {
    if (m_posizione_mouse == MousePos::CheckboxAnimazione) {
      m_casella_animazione.setinterncolor(sf::Color::Red);
      m_casella_animazione.change_status(true);
      m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
      m_casella_automa.change_status(false);

      m_paletta_colori.DisattivaInserimento();

      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::CheckboxAutoma) {
      m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
      m_casella_animazione.change_status(false);
      m_casella_automa.setinterncolor(sf::Color::Red);
      m_casella_automa.change_status(true);

      m_paletta_colori.DisattivaInserimento();

      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::PulsantePlay) {
      m_pulsante_play.setinterncolor(sf::Color::Red);
      m_pulsante_play.change_status(true);
      m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
      m_pulsante_pausa.change_status(false);

      m_paletta_colori.DisattivaInserimento();

      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::PulsantePausa) {
      m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
      m_pulsante_play.change_status(false);
      m_pulsante_pausa.setinterncolor(sf::Color::Red);
      m_pulsante_pausa.change_status(true);

      m_paletta_colori.DisattivaInserimento();

      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::PalettaVulnerabile && m_paletta_colori.IsActive()) {
      m_paletta_colori.AttivaInserimento();
      m_paletta_colori.AttivaVerde();

      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::PalettaInfetto && m_paletta_colori.IsActive()) {
      m_paletta_colori.AttivaInserimento();
      m_paletta_colori.AttivaRosso();
      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::PalettaGuariti && m_paletta_colori.IsActive()) {
      m_paletta_colori.AttivaInserimento();
      m_paletta_colori.AttivaBlu();
      return m_posizione_mouse;
    } else if (m_posizione_mouse == MousePos::PalettaMorti && m_paletta_colori.IsActive()) {
      m_paletta_colori.AttivaInserimento();
      m_paletta_colori.AttivaBianco();
      return m_posizione_mouse;

    } else {
      m_paletta_colori.DisattivaInserimento();
      return m_posizione_mouse;
    }
  }

  void is_out() { check_color(sf::Color(128, 128, 128)); }

  bool cliccabile() {
    if (m_posizione_mouse != MousePos::None) {
      return true;
    } else {
      return false;
    }
  }

  Paletta* GetPointerPaletta() { return &m_paletta_colori; }
  Informazioni* GetPointerRiquadro() { return &m_riquadro_informazioni; }

  bool IsInserimentoAttivo() { return m_paletta_colori.IsInserimentoAttivo(); }
};

int Casuale();

#endif