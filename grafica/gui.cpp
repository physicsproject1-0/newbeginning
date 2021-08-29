#include "gui.hpp"

#include <SFML/Graphics.hpp>
#include <stdexcept>

GUI::GUI(sf::Vector2f dimensione)
    : m_limiti_sfondo_grigio(sf::Vector2f(dimensione)),
      m_testo_animazione(10, &m_font, sf::Color::Black),
      m_testo_automa(10, &m_font, sf::Color::Black),
      m_casella_animazione(sf::Vector2f(40, 40)),
      m_casella_automa(sf::Vector2f(40, 40)),
      m_pulsante_play(30, TipoPulsante::Play),
      m_pulsante_pausa(30, TipoPulsante::Pausa),
      m_pennello_colori(sf::Vector2f(dimensione.x / 1.3, dimensione.y * 0.4), &m_font),
      m_riquadro_informazioni(sf::Vector2f(dimensione.x * 2, dimensione.y / 4), &m_font),
      m_posizione_mouse(MousePos::None) {
  if (!m_font.loadFromFile("Arial.ttf")) {
    throw std::runtime_error("font non caricato");
  }

  m_sfondo_grigio.setFillColor(sf::Color(128, 128, 128));  // Grey
  m_sfondo_grigio.setSize(dimensione);

  m_casella_animazione.change_status(true);
  m_casella_animazione.setinterncolor(sf::Color::Red);
  m_pulsante_pausa.change_status(true);
  m_pulsante_pausa.setinterncolor(sf::Color::Red);

  m_testo_animazione.scrivi("Animazione");
  m_testo_automa.scrivi("Automa");

  m_pennello_colori.Scrivi();

  if (m_testo_animazione.getlimiti().width > m_limiti_sfondo_grigio.getlimiti().width ||
      m_testo_automa.getlimiti().width > m_limiti_sfondo_grigio.getlimiti().width) {
    throw std::runtime_error("la larghezza della GUI non è sufficiente ad ospitare le caselle di testo");
  }
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates) const {
  target.draw(m_sfondo_grigio);
  target.draw(m_limiti_sfondo_grigio);
  target.draw(m_casella_animazione);
  target.draw(m_casella_automa);
  target.draw(m_testo_animazione);
  target.draw(m_testo_automa);
  target.draw(m_pulsante_play);
  target.draw(m_pulsante_pausa);
  target.draw(m_pennello_colori);
  target.draw(m_riquadro_informazioni);
}

void GUI::AggiornaPosizione(sf::Vector2f punto_in_altosx, sf::Vector2f dimensioni_finestra) {  // da usare nel resize
  sf::Vector2f t_posizione_aggancio_alto_sx(
      punto_in_altosx.x,
      punto_in_altosx.y + dimensioni_finestra.y / 2 - m_limiti_sfondo_grigio.getlimiti().height / 2);  // punta al vertice alto sx dello sfondo grigio

  float t_altezza_sfondo_grigio = m_limiti_sfondo_grigio.getlimiti().height;
  float t_larghezza_sfondo_grigio = m_limiti_sfondo_grigio.getlimiti().width;

  m_sfondo_grigio.setPosition(t_posizione_aggancio_alto_sx);
  m_limiti_sfondo_grigio.set_posizione(t_posizione_aggancio_alto_sx);

  m_casella_animazione.set_posizione(
      sf::Vector2f(punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 6));
  m_casella_automa.set_posizione(
      sf::Vector2f(punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 3));

  m_testo_animazione.set_posizione(
      sf::Vector2f(punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, m_casella_animazione.getlimiti().top - 10));  // riferiti alle checkbox
  m_testo_automa.set_posizione(sf::Vector2f(punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, m_casella_automa.getlimiti().top - 10));

  m_pulsante_play.set_posizione(
      sf::Vector2f(t_posizione_aggancio_alto_sx.x + t_larghezza_sfondo_grigio / 4, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 2));
  m_pulsante_pausa.set_posizione(sf::Vector2f(t_posizione_aggancio_alto_sx.x + (t_larghezza_sfondo_grigio / 4) * 3,
                                              t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 2));

  m_pennello_colori.ImpostaPosizione(sf::Vector2f(t_posizione_aggancio_alto_sx.x + t_larghezza_sfondo_grigio / 2,
                                                 t_posizione_aggancio_alto_sx.y + (t_altezza_sfondo_grigio / 4) * 3));

  m_riquadro_informazioni.AggiornaPosizione(sf::Vector2f(punto_in_altosx.x + dimensioni_finestra.x, punto_in_altosx.y));
}

void GUI::CheckMousePosition(sf::Vector2f t_coordinate_mouse) {
  if (m_casella_animazione.getlimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::CheckboxAnimazione;

  } else if (m_casella_automa.getlimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::CheckboxAutoma;

  } else if (m_pulsante_play.getlimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PulsantePlay;

  } else if (m_pulsante_pausa.getlimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PulsantePausa;
  } else if (m_pennello_colori.RitornaVerde().getlimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PennelloVulnerabile;
  } else if (m_pennello_colori.RitornaRosso().getlimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PennelloInfetto;
  } else if (m_pennello_colori.RitornaBlu().getlimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PennelloGuariti;
  } else if (m_pennello_colori.RitornaBianco().getlimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PennelloMorti;
  } else {
    m_posizione_mouse = MousePos::None;
  }
}

void GUI::CheckColor(sf::Color colore) {
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

MousePos GUI::ClickOfMouse() {
  if (m_posizione_mouse == MousePos::CheckboxAnimazione) {
    m_casella_animazione.setinterncolor(sf::Color::Red);
    m_casella_animazione.change_status(true);
    m_casella_automa.setinterncolor(sf::Color(128, 128, 128));
    m_casella_automa.change_status(false);

    m_pennello_colori.DisattivaInserimento();

    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::CheckboxAutoma) {
    m_casella_animazione.setinterncolor(sf::Color(128, 128, 128));
    m_casella_animazione.change_status(false);
    m_casella_automa.setinterncolor(sf::Color::Red);
    m_casella_automa.change_status(true);

    m_pennello_colori.DisattivaInserimento();

    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::PulsantePlay) {
    m_pulsante_play.setinterncolor(sf::Color::Red);
    m_pulsante_play.change_status(true);
    m_pulsante_pausa.setinterncolor(sf::Color(128, 128, 128));
    m_pulsante_pausa.change_status(false);

    m_pennello_colori.DisattivaInserimento();

    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::PulsantePausa) {
    m_pulsante_play.setinterncolor(sf::Color(128, 128, 128));
    m_pulsante_play.change_status(false);
    m_pulsante_pausa.setinterncolor(sf::Color::Red);
    m_pulsante_pausa.change_status(true);

    m_pennello_colori.DisattivaInserimento();

    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::PennelloVulnerabile && m_pennello_colori.IsActive()) {
    m_pennello_colori.AttivaInserimento();
    m_pennello_colori.AttivaVerde();

    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::PennelloInfetto && m_pennello_colori.IsActive()) {
    m_pennello_colori.AttivaInserimento();
    m_pennello_colori.AttivaRosso();
    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::PennelloGuariti && m_pennello_colori.IsActive()) {
    m_pennello_colori.AttivaInserimento();
    m_pennello_colori.AttivaBlu();
    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::PennelloMorti && m_pennello_colori.IsActive()) {
    m_pennello_colori.AttivaInserimento();
    m_pennello_colori.AttivaBianco();
    return m_posizione_mouse;

  } else {
    m_pennello_colori.DisattivaInserimento();
    return m_posizione_mouse;
  }
}