#include "pennello.hpp"

void Pennello::ImpostaPosizione(sf::Vector2f t_posizione_centro) {  // la consideriamo rispetto al centro
  m_riquadro_esterno.set_posizione(t_posizione_centro -
                                   sf::Vector2f(m_riquadro_esterno.GetLimiti().width / 2, m_riquadro_esterno.GetLimiti().height / 2));

  m_rettangolo_opaco.setPosition(t_posizione_centro);

  m_box_verde.set_posizione(sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.GetLimiti().top + (m_riquadro_esterno.GetLimiti().height / 5) * 1));
  m_box_rosso.set_posizione(sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.GetLimiti().top + (m_riquadro_esterno.GetLimiti().height / 5) * 2));
  m_box_blu.set_posizione(sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.GetLimiti().top + (m_riquadro_esterno.GetLimiti().height / 5) * 3));
  m_box_bianco.set_posizione(
      sf::Vector2f(t_posizione_centro.x, m_riquadro_esterno.GetLimiti().top + (m_riquadro_esterno.GetLimiti().height / 5) * 4));

  m_testo_verde.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_verde.GetLimiti().top - 10));
  m_testo_rosso.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_rosso.GetLimiti().top - 10));
  m_testo_blu.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_blu.GetLimiti().top - 10));
  m_testo_bianco.set_posizione(sf::Vector2f(t_posizione_centro.x, m_box_bianco.GetLimiti().top - 10));
}

void Pennello::Scrivi() {  // funzione presente altrimenti scrive prima di caricare il font e mi restituisce 0 di larghezza , il loading del font
                          // avviene dopo
                          // in GUI
  m_testo_verde.scrivi("Vulnerabile");
  m_testo_rosso.scrivi("Infetto");
  m_testo_blu.scrivi("Guarito");
  m_testo_bianco.scrivi("Morto");

  if (m_testo_verde.GetLimiti().width > m_riquadro_esterno.GetLimiti().width ||
      m_testo_rosso.GetLimiti().width > m_riquadro_esterno.GetLimiti().width ||
      m_testo_blu.GetLimiti().width > m_riquadro_esterno.GetLimiti().width ||
      m_testo_bianco.GetLimiti().width > m_riquadro_esterno.GetLimiti().width) {
    throw std::runtime_error("dimensione pennello non sufficiente per ospitare testo");
  }
}

void Pennello::draw(sf::RenderTarget& target, sf::RenderStates) const {
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

void Pennello::AggiornaPosizioneRettangoliDaInserire(sf::Vector2f t_posizione_mouse) {
  m_dainserire_verde.setPosition(t_posizione_mouse);
  m_dainserire_rosso.setPosition(t_posizione_mouse);
  m_dainserire_blu.setPosition(t_posizione_mouse);
  m_dainserire_bianco.setPosition(t_posizione_mouse);
}

bool Pennello::IsActive() { return m_is_active; }

void Pennello::Attiva() { m_is_active = true; }

void Pennello::Disattiva() { m_is_active = false; }

Checkbox Pennello::RitornaVerde() { return m_box_verde; }
Checkbox Pennello::RitornaRosso() { return m_box_rosso; }
Checkbox Pennello::RitornaBlu() { return m_box_blu; }
Checkbox Pennello::RitornaBianco() { return m_box_bianco; }

bool Pennello::IsInserimentoAttivo() { return m_inserimento_attivo; }
void Pennello::AttivaInserimento() { m_inserimento_attivo = true; }
void Pennello::DisattivaInserimento() { m_inserimento_attivo = false; }

Stato Pennello::RitornaStatoRettangoloInserendo() {
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

void Pennello::AttivaVerde() {
  m_verde_visibile = true;
  m_rosso_visibile = false;
  m_blu_visibile = false;
  m_bianco_visibile = false;
}
void Pennello::AttivaRosso() {
  m_verde_visibile = false;
  m_rosso_visibile = true;
  m_blu_visibile = false;
  m_bianco_visibile = false;
}
void Pennello::AttivaBlu() {
  m_verde_visibile = false;
  m_rosso_visibile = false;
  m_blu_visibile = true;
  m_bianco_visibile = false;
}
void Pennello::AttivaBianco() {
  m_verde_visibile = false;
  m_rosso_visibile = false;
  m_blu_visibile = false;
  m_bianco_visibile = true;
}
