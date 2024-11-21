#include <iostream>
#include <SFML/Graphics.hpp>
#include "Juego.h"


Juego::Juego()
    : window(sf::VideoMode(800, 600), "Peleitas"), 
      piso(0.0f, 550.0f), 
      tiempoDelta(0.0f), 
      tiempoPartida(90.0f), 
      inicioTiempo(reloj.getElapsedTime().asSeconds())
{
    reiniciarJugadores();
}

Juego::~Juego()
{
    delete jugador1;
    delete jugador2;
}

void Juego::reiniciarJugadores()
{
    jugador1 = new Hanzo(175.0f, 450.0f, sf::Color::Yellow);
    jugador2 = new Samurai(375.0f, 450.0f, sf::Color::Red);
    inicializarAnimaciones();
}

void Juego::manejarAtaques(const sf::Event &event)
{
    if (event.key.code == sf::Keyboard::R && jugador1->hitbox.getGlobalBounds().intersects(jugador2->rectan.getGlobalBounds()))
    {
        float direccion = (jugador1->rectan.getPosition().x < jugador2->rectan.getPosition().x) ? 1.0f : -1.0f;
        jugador2->recibirAtaque(20.0f, sf::Vector2f(direccion * 150.0f, -150.0f));
        verificarDerrota(jugador2, "Jugador 1");
    }
    if (event.key.code == sf::Keyboard::P && jugador2->hitbox.getGlobalBounds().intersects(jugador1->rectan.getGlobalBounds()))
    {
        float direccion = (jugador2->rectan.getPosition().x < jugador1->rectan.getPosition().x) ? 1.0f : -1.0f;
        jugador1->recibirAtaque(20.0f, sf::Vector2f(direccion * 150.0f, -150.0f));
        verificarDerrota(jugador1, "Jugador 2");
    }
}

void Juego::manejarProyectiles(const sf::Event &event)
{
    if (event.key.code == sf::Keyboard::Q)
    {
        jugador1->lanzarCuchillo();
    }
    if (event.key.code == sf::Keyboard::O)
    {
        jugador2->lanzarCuchillo();
    }
}


void Juego::actualizar()
{
    tiempoDelta = relojMov.restart().asSeconds();
    // Actualizar movimiento de jugadores
    jugador1->move(tiempoDelta, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, piso.rectan.getPosition().y);
    jugador2->move(tiempoDelta, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, piso.rectan.getPosition().y);

    jugador1->setPosition(jugador1->rectan.getPosition().x,jugador1->rectan.getPosition().y);
    jugador1->updateAnimation(tiempoDelta);

    // Actualizar proyectiles
    jugador1->actualizarCuchillos(tiempoDelta);
    jugador2->actualizarCuchillos(tiempoDelta);
}

void Juego::verificarDerrota(Luchador *jugador, const std::string &ganador)
{
    if (jugador->lives == 0)
    {
        std::cout << ganador << " gana!" << std::endl;
        window.close();
    }
}

void Juego::dibujarTiempoRestante()
{
    float tiempoRestante = calcularTiempoRestante();
    int minutos = static_cast<int>(tiempoRestante) / 60;
    int segundos = static_cast<int>(tiempoRestante) % 60;

    sf::Font fuente;
    if (!fuente.loadFromFile("assets/fonts/upheavtt.ttf")) // Ajusta la ruta si es necesario
    {
        std::cerr << "Error cargando la fuente." << std::endl;
        return;
    }

    sf::Text textoTiempo;
    textoTiempo.setFont(fuente);
    textoTiempo.setCharacterSize(50);
    textoTiempo.setFillColor(sf::Color::White);
    textoTiempo.setString(
        (minutos < 10 ? "0" : "") + std::to_string(minutos) + ":" +
        (segundos < 10 ? "0" : "") + std::to_string(segundos));
    textoTiempo.setPosition(window.getSize().x / 2.0f - textoTiempo.getGlobalBounds().width / 2.0f, 25.0f);

    window.draw(textoTiempo);
}

float Juego::calcularTiempoRestante() 
{
    return tiempoPartida - (reloj.getElapsedTime().asSeconds() - inicioTiempo);
}


void Juego::determinarGanador()
{
    if (jugador1->health > jugador2->health)
    {
        std::cout << "Jugador 1 gana por salud!" << std::endl;
    }
    else if (jugador2->health > jugador1->health)
    {
        std::cout << "Jugador 2 gana por salud!" << std::endl;
    }
    else
    {
        std::cout << "¡Empate!" << std::endl;
    }
    window.close();
}

void Juego::inicializarAnimaciones() {
        if (!animationsInitialized) {
        if (jugador1->CargarTexture("assets/anims/hanzo/Idle_Hanzo.png")) {
            jugador1->setTransparentColor(sf::Color(64, 176, 72));
            jugador1->IniciarAnimation(100, 118, 10, 0.1f, true);
            jugador1->playAnimation();  // Aseguramos que la animación está activa
            animationsInitialized = true;
            std::cout << "Animación inicializada correctamente" << std::endl;
        } else {
            std::cerr << "Error al cargar la textura de la animación!" << std::endl;
        }
    }
}

void Juego::ejecutar() {
    Menu menu;
    menu.run_menu();  // Ejecutar el menú

    if (menu.state == Menu::GameState::Game) {  // Si el jugador seleccionó "Jugar"
        while (window.isOpen()) {
            procesarEventos();
            actualizar();
            renderizar();

            // Verificar si el tiempo ha terminado
            if (calcularTiempoRestante() <= 0) {
                determinarGanador();
                break;  // Terminar el juego
            }
        }
    } else if (menu.state == Menu::GameState::Controls) {  // Si el jugador seleccionó "Controles"
        menu.showControls();  // Mostrar la ventana de controles
    }
}



void Juego::procesarEventos()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            manejarAtaques(event);
            manejarProyectiles(event);
        }
    }
}

void Juego::renderizar()
{
    window.clear(sf::Color::Blue);

    // Dibujar el escenario
    window.draw(piso.rectan);

    // Dibujar jugadores
    // window.draw(jugador1->rectan);
    window.draw(jugador2->rectan);

    jugador1->draw(window);

    // Dibujar hitboxes (opcional, para depuración)
    // window.draw(jugador1->hitbox);
    // window.draw(jugador2->hitbox);

    // Dibujar barras de salud
    jugador1->drawHealthBar(window, sf::Vector2f(25.0f, 50.0f));
    jugador2->drawHealthBar(window, sf::Vector2f(575.0f, 50.0f));

    // Dibujar proyectiles
    for (auto &cuchillo : jugador1->cuchillos)
    {
        window.draw(cuchillo.forma);
    }
    for (auto &cuchillo : jugador2->cuchillos)
    {
        window.draw(cuchillo.forma);
    }

    dibujarTiempoRestante();

    window.display();
}