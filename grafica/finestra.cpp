#include "finestra.hpp"

#include <SFML/Window.hpp>
#include <iostream>

#include "animazione.hpp"
#include "automa.hpp"
#include "gui.hpp"

sf::Vector2f converti(sf::Vector2u vettore) { return sf::Vector2f(vettore.x, vettore.y); }
sf::Vector2f converti(sf::Vector2i vettore) { return sf::Vector2f(vettore.x, vettore.y); }

void Finestra::Setup(const std::string& temptitolo, const sf::Vector2u& tempdimensione) {
  v_titolo = temptitolo;
  v_dimensioni = tempdimensione;
  v_Isfullscreen = false;
  v_Isclosed = false;
  v_vista = Vista::Animazione;
  mouse_in = false;
  Create();
}

void Finestra::Create() {
  auto stile = (v_Isfullscreen ? sf::Style::Fullscreen : sf::Style::Default);
  v_mainfinestra.create(sf::VideoMode(v_dimensioni.x, v_dimensioni.y), v_titolo, stile);
}

void Finestra::Destroy() { v_mainfinestra.close(); }

Finestra::Finestra() { Setup("Defaultwindow", sf::Vector2u(1280, 720)); }

Finestra::Finestra(const std::string& titolo, const sf::Vector2u& dimensione, GUI* overlay, Animazione* t_dinamico, Automa* t_statico,
                   const Bordi& t_bordo_animazione, const Bordi& t_bordo_automa)
    : v_dimensioni_minime{dimensione}, v_vista(Vista::Animazione), v_overlay{overlay}, m_dinamico{t_dinamico}, m_statico{t_statico} {
  Setup(titolo, dimensione);
  specifiche_viste(Vista::Animazione, t_bordo_animazione.getlimiti());
  specifiche_viste(Vista::Automa, t_bordo_automa.getlimiti());
  punto_alto_sx.x = 0;
  punto_alto_sx.y = 0;
}

Finestra::~Finestra() { Destroy(); }

void Finestra::Update() {
  sf::Event evento;
  while (v_mainfinestra.pollEvent(evento)) {
    if (evento.type == sf::Event::Closed || (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape)) {
      v_Isclosed = true;
    }
    if (evento.type == sf::Event::Resized) {
      sf::Vector2u dimensioni_nuove = v_mainfinestra.getSize();
      if (dimensioni_nuove.x < v_dimensioni_minime.x || dimensioni_nuove.y < v_dimensioni_minime.y) {
        dimensioni_nuove = v_dimensioni_minime;
        v_mainfinestra.setSize(dimensioni_nuove);
      }
      v_dimensioni = dimensioni_nuove;

      vista_animazione.setSize(converti(dimensioni_nuove));  // le dimensioni si devono aggiornare per entrambe le viste ad ogni resize
      vista_automa.setSize(converti(dimensioni_nuove));
      switch (v_vista) {
        case Vista::Animazione:

          punto_alto_sx.x = vista_animazione.getCenter().x - dimensioni_nuove.x / 2;
          punto_alto_sx.y = vista_animazione.getCenter().y - dimensioni_nuove.y / 2;
          break;

        case Vista::Automa:

          punto_alto_sx.x = vista_automa.getCenter().x - dimensioni_nuove.x / 2;
          punto_alto_sx.y = vista_automa.getCenter().y - dimensioni_nuove.y / 2;
          break;
      }

      upd_vista();
      /* sf::View view2(sf::Vector2f(400, 300), converti(dimensioni_nuove)); */
      m_statico->AzzeraOrologio();
      m_dinamico->AzzeraOrologio();  // evitiamo che si perdano pezzi di animazione durante il resize
    }

    if (evento.type == sf::Event::MouseEntered) {
      mouse_in = true;
    }
    if (evento.type == sf::Event::MouseLeft) {
      v_overlay->is_out();
      mouse_in = false;
    }
    if (mouse_in /* && evento.type == sf::Event::MouseMoved */) {
      posizione_mouse = punto_alto_sx + (converti(v_mouse.getPosition(v_mainfinestra)));

      v_overlay->check_mouse_position(posizione_mouse);  // vedere se funziona
      /* std::cout << posizione_mouse.x << " " << posizione_mouse.y << '\n'; */
      v_overlay->check_color(sf::Color::Yellow);

      v_overlay->AggiornaPosizioneRettangoliPaletta(posizione_mouse);

      if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
        switch (v_overlay->mouse_clicked()) {
          case MousePos::CheckboxAnimazione:
            v_vista = Vista::Animazione;
            punto_alto_sx.x = vista_animazione.getCenter().x - vista_animazione.getSize().x / 2;
            punto_alto_sx.y = vista_animazione.getCenter().y - vista_animazione.getSize().y / 2;
            break;

          case MousePos::CheckboxAutoma:
            v_vista = Vista::Automa;
            punto_alto_sx.x = vista_automa.getCenter().x - vista_automa.getSize().x / 2;
            punto_alto_sx.y = vista_automa.getCenter().y - vista_automa.getSize().y / 2;
            break;

          case MousePos::PulsantePlay:

            m_dinamico->StartAnimazione();
            m_statico->StartAutoma();
            v_overlay->DisattivaInserimento();
            break;
          case MousePos::PulsantePausa:
            m_dinamico->StopAnimazione();
            m_statico->StopAutoma();
            v_overlay->AttivaInserimento();
            break;
        }

        if (m_statico->IsStopped()) {
          m_statico->CheckMousePosition(posizione_mouse);
        }

        upd_vista();
      }
    }
  }
  if (!m_statico->IsStopped() && v_vista == Vista::Automa) {
    v_overlay->GetPointerRiquadro()->AggiungiStatoAutoma(m_statico->GetCensimento());
  }

  if (!m_dinamico->IsStopped() && v_vista == Vista::Animazione) {
    v_overlay->GetPointerRiquadro()->AggiungiStatoAnimazione(m_dinamico->GetCensimento());
  }
  v_overlay->GetPointerRiquadro()->AggiornaScritte();
  /*
    if (v_overlay->cliccabile()){
      v_mouse.
    } */
}

void Finestra::upd_vista() {
  switch (v_vista) {
    case Vista::Animazione:
      SetVista(vista_animazione);
      v_overlay->aggiorna_posizione(punto_alto_sx, vista_animazione.getSize());
      v_overlay->GetPointerRiquadro()->MostraDatiAnimazione();
      v_overlay->GetPointerRiquadro()->AggiornaScritte();
      break;

    case Vista::Automa:
      SetVista(vista_automa);
      v_overlay->aggiorna_posizione(punto_alto_sx, vista_automa.getSize());
      v_overlay->GetPointerRiquadro()->MostraDatiAutoma();
      v_overlay->GetPointerRiquadro()->AggiornaScritte();
      break;
  }
}

void Finestra::specifiche_viste(Vista t_vista, sf::FloatRect t_limite) {  // mettere nel costruttore di finestra
  sf::View vista(sf::Vector2f(t_limite.left + t_limite.width / 2, t_limite.top + t_limite.height / 2), converti(v_dimensioni));
  switch (t_vista) {
    case Vista::Animazione:
      vista_animazione = vista;
      break;

    case Vista::Automa:
      vista_automa = vista;
      break;
  }
}

void Finestra::SetVista(sf::View vista) { v_mainfinestra.setView(vista); }

void Finestra::Pulisci() { v_mainfinestra.clear(sf::Color::Black); }
void Finestra::Disegna(sf::Drawable& cosadadisegnare) { v_mainfinestra.draw(cosadadisegnare); }  // capire come funziona sta roba
void Finestra::Mostra() { v_mainfinestra.display(); }

sf::Vector2u Finestra::Getdimensions() { return v_dimensioni; }

bool Finestra::Isclosed() { return v_Isclosed; }
bool Finestra::Isfullscreen() { return v_Isfullscreen; }