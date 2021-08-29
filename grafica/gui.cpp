#include "gui.hpp"

#include <SFML/Graphics.hpp>
#include <stdexcept>

GUI::GUI(sf::Vector2f t_dimensione)
    : m_limiti_sfondo_grigio(sf::Vector2f(t_dimensione)),
      m_testo_animazione(10, &m_font, sf::Color::Black),
      m_testo_automa(10, &m_font, sf::Color::Black),
      m_casella_animazione(sf::Vector2f(40, 40)),
      m_casella_automa(sf::Vector2f(40, 40)),
      m_pulsante_play(30, TipoPulsante::Play),
      m_pulsante_pausa(30, TipoPulsante::Pausa),
      m_pennello_colori(sf::Vector2f(t_dimensione.x / 1.3, t_dimensione.y * 0.4), &m_font),
      m_riquadro_informazioni(sf::Vector2f(t_dimensione.x * 2, t_dimensione.y / 4), &m_font),
      m_posizione_mouse(MousePos::None) {
  if (!m_font.loadFromFile("Arial.ttf")) {
    throw std::runtime_error("font non caricato");
  }

  m_sfondo_grigio.setFillColor(sf::Color(128, 128, 128));  // Grigio
  m_sfondo_grigio.setSize(t_dimensione);

  m_casella_animazione.ChangeStatus(true);
  m_casella_animazione.SetColoreInterno(sf::Color::Red);
  m_pulsante_pausa.ChangeStatus(true);
  m_pulsante_pausa.SetColoreInterno(sf::Color::Red);

  m_testo_animazione.Scrivi("Animazione");
  m_testo_automa.Scrivi("Automa");

  m_pennello_colori.Scrivi();

  if (m_testo_animazione.GetLimiti().width > m_limiti_sfondo_grigio.GetLimiti().width ||
      m_testo_automa.GetLimiti().width > m_limiti_sfondo_grigio.GetLimiti().width) {
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

void GUI::AggiornaPosizione(sf::Vector2f t_punto_in_altosx, sf::Vector2f dimensioni_finestra) {  // prende input in finestra.cpp
  sf::Vector2f t_posizione_aggancio_alto_sx(t_punto_in_altosx.x,
                                            t_punto_in_altosx.y + dimensioni_finestra.y / 2 -
                                                m_limiti_sfondo_grigio.GetLimiti().height / 2);  // punta al vertice alto sx dello sfondo grigio

  float t_altezza_sfondo_grigio = m_limiti_sfondo_grigio.GetLimiti().height;
  float t_larghezza_sfondo_grigio = m_limiti_sfondo_grigio.GetLimiti().width;

  m_sfondo_grigio.setPosition(t_posizione_aggancio_alto_sx);
  m_limiti_sfondo_grigio.SetPosizione(t_posizione_aggancio_alto_sx);

  m_casella_animazione.SetPosizione(
      sf::Vector2f(t_punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 6));
  m_casella_automa.SetPosizione(
      sf::Vector2f(t_punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 3));

  m_testo_animazione.SetPosizione(
      sf::Vector2f(t_punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, m_casella_animazione.GetLimiti().top - 10));  // riferiti alle checkbox
  m_testo_automa.SetPosizione(sf::Vector2f(t_punto_in_altosx.x + t_larghezza_sfondo_grigio / 2, m_casella_automa.GetLimiti().top - 10));

  m_pulsante_play.SetPosizione(
      sf::Vector2f(t_posizione_aggancio_alto_sx.x + t_larghezza_sfondo_grigio / 4, t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 2));
  m_pulsante_pausa.SetPosizione(sf::Vector2f(t_posizione_aggancio_alto_sx.x + (t_larghezza_sfondo_grigio / 4) * 3,
                                             t_posizione_aggancio_alto_sx.y + t_altezza_sfondo_grigio / 2));

  m_pennello_colori.ImpostaPosizione(sf::Vector2f(t_posizione_aggancio_alto_sx.x + t_larghezza_sfondo_grigio / 2,
                                                  t_posizione_aggancio_alto_sx.y + (t_altezza_sfondo_grigio / 4) * 3));

  m_riquadro_informazioni.AggiornaPosizione(sf::Vector2f(t_punto_in_altosx.x + dimensioni_finestra.x, t_punto_in_altosx.y));
}

void GUI::AggiornaPosizioneRettangoliPennello(sf::Vector2f t_coordinate_mouse) {
  if (m_pennello_colori.IsInserimentoAttivo()) {
    m_pennello_colori.AggiornaPosizioneRettangoliDaInserire(t_coordinate_mouse);
  }
}

void GUI::AttivaInserimento() { m_pennello_colori.Attiva(); }  // attiva l'opacità della tavolozza

void GUI::DisattivaInserimento() { m_pennello_colori.Disattiva(); }

void GUI::CheckMousePosition(
    sf::Vector2f t_coordinate_mouse) {  // funzione chiamata nel finestra.cpp, confronta le coordinate mouse con le cose sullo schermo
  if (m_casella_animazione.GetLimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::CheckboxAnimazione;

  } else if (m_casella_automa.GetLimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::CheckboxAutoma;

  } else if (m_pulsante_play.GetLimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PulsantePlay;

  } else if (m_pulsante_pausa.GetLimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PulsantePausa;

  } else if (m_pennello_colori.RitornaVerde().GetLimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PennelloVulnerabile;

  } else if (m_pennello_colori.RitornaRosso().GetLimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PennelloInfetto;

  } else if (m_pennello_colori.RitornaBlu().GetLimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PennelloGuariti;

  } else if (m_pennello_colori.RitornaBianco().GetLimiti().contains(t_coordinate_mouse)) {
    m_posizione_mouse = MousePos::PennelloMorti;

  } else {
    m_posizione_mouse = MousePos::None;
  }
}

void GUI::CheckColor(sf::Color t_colore) {  // funzione chiamata nel finestra.cpp, serve in base alla posizione del mouse, ad illuminare i pulsanti
  switch (m_posizione_mouse) {
    case MousePos::CheckboxAnimazione:

      if (!m_casella_animazione.ReturnStatus()) {
        m_casella_animazione.SetColoreInterno(t_colore);
      }
      if (!m_casella_automa.ReturnStatus()) {
        m_casella_automa.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_pulsante_play.ReturnStatus()) {
        m_pulsante_play.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_pulsante_pausa.ReturnStatus()) {
        m_pulsante_pausa.SetColoreInterno(sf::Color(128, 128, 128));
      }
      break;

    case MousePos::CheckboxAutoma:
      if (!m_casella_automa.ReturnStatus()) {
        m_casella_automa.SetColoreInterno(t_colore);
      }
      if (!m_casella_animazione.ReturnStatus()) {
        m_casella_animazione.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_pulsante_play.ReturnStatus()) {
        m_pulsante_play.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_pulsante_pausa.ReturnStatus()) {
        m_pulsante_pausa.SetColoreInterno(sf::Color(128, 128, 128));
      }
      break;

    case MousePos::PulsantePlay:
      if (!m_pulsante_play.ReturnStatus()) {
        m_pulsante_play.SetColoreInterno(t_colore);
      }
      if (!m_casella_automa.ReturnStatus()) {
        m_casella_automa.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_casella_animazione.ReturnStatus()) {
        m_casella_animazione.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_pulsante_pausa.ReturnStatus()) {
        m_pulsante_pausa.SetColoreInterno(sf::Color(128, 128, 128));
      }
      break;

    case MousePos::PulsantePausa:
      if (!m_pulsante_pausa.ReturnStatus()) {
        m_pulsante_pausa.SetColoreInterno(t_colore);
      }
      if (!m_casella_automa.ReturnStatus()) {
        m_casella_automa.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_casella_animazione.ReturnStatus()) {
        m_casella_animazione.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_pulsante_play.ReturnStatus()) {
        m_pulsante_play.SetColoreInterno(sf::Color(128, 128, 128));
      }
      break;

    case MousePos::None:
      if (!m_casella_automa.ReturnStatus()) {
        m_casella_automa.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_casella_animazione.ReturnStatus()) {
        m_casella_animazione.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_pulsante_play.ReturnStatus()) {
        m_pulsante_play.SetColoreInterno(sf::Color(128, 128, 128));
      }
      if (!m_pulsante_pausa.ReturnStatus()) {
        m_pulsante_pausa.SetColoreInterno(sf::Color(128, 128, 128));
      }
      break;

    default:
      break;
  }
}

MousePos GUI::ClickOfMouse() {  // funzione chiamata nel finestra.cpp, attivata al click del mouse
  if (m_posizione_mouse == MousePos::CheckboxAnimazione) {
    m_casella_animazione.SetColoreInterno(sf::Color::Red);
    m_casella_animazione.ChangeStatus(true);
    m_casella_automa.SetColoreInterno(sf::Color(128, 128, 128));
    m_casella_automa.ChangeStatus(false);

    m_pennello_colori.DisattivaInserimento();

    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::CheckboxAutoma) {
    m_casella_animazione.SetColoreInterno(sf::Color(128, 128, 128));
    m_casella_animazione.ChangeStatus(false);
    m_casella_automa.SetColoreInterno(sf::Color::Red);
    m_casella_automa.ChangeStatus(true);

    m_pennello_colori.DisattivaInserimento();

    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::PulsantePlay) {
    m_pulsante_play.SetColoreInterno(sf::Color::Red);
    m_pulsante_play.ChangeStatus(true);
    m_pulsante_pausa.SetColoreInterno(sf::Color(128, 128, 128));
    m_pulsante_pausa.ChangeStatus(false);

    m_pennello_colori.DisattivaInserimento();

    return m_posizione_mouse;
  } else if (m_posizione_mouse == MousePos::PulsantePausa) {
    m_pulsante_play.SetColoreInterno(sf::Color(128, 128, 128));
    m_pulsante_play.ChangeStatus(false);
    m_pulsante_pausa.SetColoreInterno(sf::Color::Red);
    m_pulsante_pausa.ChangeStatus(true);

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

void GUI::IsOut() { CheckColor(sf::Color(128, 128, 128)); }

Pennello* GUI::GetPointerPennello() { return &m_pennello_colori; }

Informazioni* GUI::GetPointerRiquadro() { return &m_riquadro_informazioni; }

bool GUI::IsInserimentoAttivo() { return m_pennello_colori.IsInserimentoAttivo(); } //controlla se stai inserendo un rettangolo