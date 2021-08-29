#include "finestra.hpp"

#include <SFML/Window.hpp>
#include <iostream>

sf::Vector2f converti(sf::Vector2u t_vettore) { return sf::Vector2f(t_vettore.x, t_vettore.y); }
sf::Vector2f converti(sf::Vector2i t_vettore) { return sf::Vector2f(t_vettore.x, t_vettore.y); }

void Finestra::Setup(const std::string& t_titolo, const sf::Vector2u& t_dimensione) {
  m_titolo = t_titolo;
  m_dimensioni = t_dimensione;
  m_is_closed = false;
  m_vista = Vista::Animazione;
  m_mouse_in = false;
  Create();
  m_posizione = m_mainfinestra.getPosition();
}

void Finestra::Create() {
  auto t_stile = sf::Style::Default;
  m_mainfinestra.create(sf::VideoMode(m_dimensioni.x, m_dimensioni.y), m_titolo, t_stile);
}

void Finestra::Destroy() { m_mainfinestra.close(); }

Finestra::Finestra() { Setup("Defaultwindow", sf::Vector2u(1280, 720)); }

Finestra::Finestra(const std::string& t_titolo, const sf::Vector2u& t_dimensione, GUI* t_overlay, Animazione* t_dinamico, Automa* t_statico,
                   const Bordi& t_bordo_animazione, const Bordi& t_bordo_automa)
    : m_dimensioni_minime{t_dimensione}, m_vista(Vista::Animazione), m_overlay{t_overlay}, m_dinamico{t_dinamico}, m_statico{t_statico} {
  Setup(t_titolo, t_dimensione);

  SpecificheViste(Vista::Animazione, t_bordo_animazione.GetLimiti());  // Acquisizione sf::Views
  SpecificheViste(Vista::Automa, t_bordo_automa.GetLimiti());

  m_punto_alto_sx.x = 0;  // Il punto in alto a destra della finestra appena creata corrisponde con il punto 0,0 del mondo
  m_punto_alto_sx.y = 0;

  m_overlay->GetPointerRiquadro()->AggiungiStatoAutoma(m_statico->GetCensimento());
  m_overlay->GetPointerRiquadro()->AggiornaScritte();

  m_overlay->GetPointerRiquadro()->AggiungiStatoAnimazione(m_dinamico->GetCensimento());
  m_overlay->GetPointerRiquadro()->AggiornaScritte();
}

Finestra::~Finestra() { Destroy(); }

void Finestra::Update() {
  if (m_mainfinestra.getPosition() != m_posizione) {  // blocca l'aggiornamento in caso si sposti la finestra lungo il desktop
    m_statico->AzzeraOrologio();
    m_dinamico->AzzeraOrologio();
    m_posizione = m_mainfinestra.getPosition();
  }

  sf::Event evento;
  while (m_mainfinestra.pollEvent(evento)) {
    if (evento.type == sf::Event::Closed || (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape)) {
      m_is_closed = true;
    }

    if (evento.type == sf::Event::Resized) {
      sf::Vector2u dimensioni_nuove = m_mainfinestra.getSize();
      if (dimensioni_nuove.x < m_dimensioni_minime.x ||
          dimensioni_nuove.y < m_dimensioni_minime.y) {  // impedire che la finestra diventi troppo piccola
        dimensioni_nuove = m_dimensioni_minime;
        m_mainfinestra.setSize(dimensioni_nuove);
      }
      m_dimensioni = dimensioni_nuove;

      m_vista_animazione.setSize(converti(dimensioni_nuove));
      m_vista_automa.setSize(converti(dimensioni_nuove));

      switch (m_vista) {
        case Vista::Animazione:
          m_punto_alto_sx.x = m_vista_animazione.getCenter().x - dimensioni_nuove.x / 2;
          m_punto_alto_sx.y = m_vista_animazione.getCenter().y - dimensioni_nuove.y / 2;
          break;

        case Vista::Automa:
          m_punto_alto_sx.x = m_vista_automa.getCenter().x - dimensioni_nuove.x / 2;
          m_punto_alto_sx.y = m_vista_automa.getCenter().y - dimensioni_nuove.y / 2;
          break;
      }
      UpdVista();
      m_statico->AzzeraOrologio();
      m_dinamico->AzzeraOrologio();  // evitiamo che si perdano pezzi di animazione durante il resize
    }

    if (evento.type == sf::Event::MouseEntered) {
      m_mouse_in = true;
    }

    if (evento.type == sf::Event::MouseLeft) {
      m_overlay->IsOut();
      m_mouse_in = false;
    }

    if (m_mouse_in) {  // gestisce le interazioni con il mouse
      m_posizione_mouse = m_punto_alto_sx + (converti(m_mouse.getPosition(m_mainfinestra)));

      m_overlay->CheckMousePosition(m_posizione_mouse);
      m_overlay->CheckColor(sf::Color::Yellow);

      m_overlay->AggiornaPosizioneRettangoliPennello(m_posizione_mouse);

      if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
        if (m_overlay->IsInserimentoAttivo()) {
          std::pair<int, int> coppia = m_statico->CheckMousePosition(m_posizione_mouse);

          if (coppia != std::pair<int, int>(-1, -1)) {
            m_statico->ChangeStatus(coppia, m_overlay->GetPointerPennello()->RitornaStatoRettangoloInserendo());
            m_statico->AggiornaSenzaAvanzare();
            m_overlay->GetPointerRiquadro()->AggiornaScritte();
            m_overlay->GetPointerPennello()->DisattivaInserimento();
          }
        }

        switch (m_overlay->ClickOfMouse()) {
          case MousePos::CheckboxAnimazione:
            m_vista = Vista::Animazione;

            if (!(m_dinamico->GetStatusOrologi()) && !(m_dinamico->IsStopped())) {
              m_dinamico->StartOrologi();
            }

            m_punto_alto_sx.x = m_vista_animazione.getCenter().x - m_vista_animazione.getSize().x / 2;
            m_punto_alto_sx.y = m_vista_animazione.getCenter().y - m_vista_animazione.getSize().y / 2;

            break;

          case MousePos::CheckboxAutoma:
            m_vista = Vista::Automa;

            if ((m_dinamico->GetStatusOrologi())) {
              m_dinamico->StopOrologi();
            }

            m_punto_alto_sx.x = m_vista_automa.getCenter().x - m_vista_automa.getSize().x / 2;
            m_punto_alto_sx.y = m_vista_automa.getCenter().y - m_vista_automa.getSize().y / 2;
            break;

          case MousePos::PulsantePlay:
            if (m_vista == Vista::Animazione) {
              if (!(m_dinamico->GetStatusOrologi())) {
                m_dinamico->StartOrologi();
              }
            }
            m_dinamico->StartAnimazione();
            m_statico->StartAutoma();
            m_overlay->GetPointerPennello()->Disattiva();
            break;

          case MousePos::PulsantePausa:
            if (m_vista == Vista::Animazione) {
              if ((m_dinamico->GetStatusOrologi())) {
                m_dinamico->StopOrologi();
              }
            }
            m_dinamico->StopAnimazione();
            m_statico->StopAutoma();
            if (m_vista == Vista::Automa) {
              m_overlay->GetPointerPennello()->Attiva();
            }
            break;

          default:
            break;
        }

        UpdVista();
      }
    }
  }
  if (!m_statico->IsStopped() && m_vista == Vista::Automa) {
    m_overlay->GetPointerRiquadro()->AggiungiStatoAutoma(m_statico->GetCensimento());
    m_overlay->GetPointerRiquadro()->AggiornaScritte();
  }

  if (!m_dinamico->IsStopped() && m_vista == Vista::Animazione) {
    m_overlay->GetPointerRiquadro()->AggiungiStatoAnimazione(m_dinamico->GetCensimento());
    m_overlay->GetPointerRiquadro()->AggiornaScritte();
  }
}

void Finestra::UpdVista() {
  switch (m_vista) {
    case Vista::Animazione:
      SetVista(m_vista_animazione);
      m_overlay->AggiornaPosizione(m_punto_alto_sx, m_vista_animazione.getSize());
      m_overlay->GetPointerRiquadro()->MostraDatiAnimazione();
      m_overlay->GetPointerRiquadro()->AggiornaScritte();
      m_overlay->GetPointerPennello()->Disattiva();

      break;

    case Vista::Automa:
      SetVista(m_vista_automa);
      m_overlay->AggiornaPosizione(m_punto_alto_sx, m_vista_automa.getSize());
      m_overlay->GetPointerRiquadro()->MostraDatiAutoma();
      m_overlay->GetPointerRiquadro()->AggiornaScritte();
      if (m_statico->IsStopped()) {
        m_overlay->GetPointerPennello()->Attiva();
      }

      break;
  }
}

void Finestra::SpecificheViste(Vista t_vista, sf::FloatRect t_limite) {  // calcolare le viste sapendo le dimensioni
  sf::View vista(sf::Vector2f(t_limite.left + t_limite.width / 2, t_limite.top + t_limite.height / 2), converti(m_dimensioni));
  switch (t_vista) {
    case Vista::Animazione:
      m_vista_animazione = vista;
      break;

    case Vista::Automa:
      m_vista_automa = vista;
      break;
  }
}

void Finestra::SetVista(sf::View vista) { m_mainfinestra.setView(vista); }

void Finestra::Pulisci() { m_mainfinestra.clear(sf::Color::Black); }
void Finestra::Disegna(sf::Drawable& cosadadisegnare) { m_mainfinestra.draw(cosadadisegnare); }
void Finestra::Mostra() { m_mainfinestra.display(); }

bool Finestra::IsClosed() { return m_is_closed; }
