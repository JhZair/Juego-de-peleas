#include <SFML/Graphics.hpp>
#include "Luchador.h"
#include <vector>
#include <iostream> 
Luchador::Luchador(float x, float y, sf::Color color) : velocidad(200.0f), velocidadNormal(200.0f), velocidadReducida(100.0f),  gravity(1100.5f), velocityY(0), jumpStrength(-480.0f),
                                                        isJumping(false), isDefending(false), maxhealth(200), health(maxhealth), lives(2) 
{
    rectan.setSize(sf::Vector2f(50.0f, 100.0f));
    rectan.setPosition(x, y);
    rectan.setFillColor(color);

    hitbox.setSize(sf::Vector2f(150.0f, 150.0f));
    hitbox.setOrigin(50.0f, 75.0f);
    hitbox.setPosition(rectan.getPosition());
    hitbox.setFillColor(sf::Color(0, 0, 0, 0));
}

void Luchador::lanzarCuchillo()
{
    if (clock.getElapsedTime().asSeconds() >= 2) {
        Cuchillo cuchillo(rectan.getPosition().x + rectan.getSize().x, rectan.getPosition().y + rectan.getSize().y / 2);
        cuchillos.push_back(cuchillo);
        clock.restart(); // Reinicia el reloj para medir el tiempo nuevamente
    }
}

void Luchador::move(float tiempoDelta, sf::Keyboard::Key izquierda, sf::Keyboard::Key derecha, sf::Keyboard::Key up, float pisoY, sf::Keyboard::Key defensa)
{
    if (sf::Keyboard::isKeyPressed(defensa)) {
        std::cout << "Defendiendo" <<std::endl;
        isDefending = true;
        velocidad = velocidadReducida; // Reduce la velocidad al defenderse
    } else {
        std::cout << "no defendiendo" << std::endl;
        isDefending = false;
        velocidad = velocidadNormal; // Restaura la velocidad normal
    }

    // when the we press a key and then the position of ther player is inside the boundaries of the game 
    if (sf::Keyboard::isKeyPressed(izquierda) && rectan.getPosition().x > 0)
    {
        rectan.move(-velocidad * tiempoDelta, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(derecha) && rectan.getPosition().x < 750)
    {
        rectan.move(velocidad * tiempoDelta, 0.0f);
    }
    // variable jumping and works with gravity and so on 
    if (isJumping)
    {
        velocityY += gravity * tiempoDelta;
    }
    // logic of jumping 
    if (sf::Keyboard::isKeyPressed(up) && !isJumping)
    {
        velocityY = jumpStrength;
        isJumping = true;
    }



    rectan.move(0.0f, velocityY * tiempoDelta);
    hitbox.setPosition(rectan.getPosition());

    // comprobar si toca el suelo
    if (rectan.getPosition().y + rectan.getSize().y >= pisoY)
    {
        rectan.setPosition(rectan.getPosition().x, pisoY - rectan.getSize().y);
        velocityY = 0;
        isJumping = false;
    }
}

void Luchador::recibirAtaque(float damage)
{
    if (isDefending) {
        damage /= 2; // Reduce el daño a la mitad cuando está defendiendo
    }
    health -= damage;
    if (health <= 0)
    {
        health = maxhealth;
        lives--;
    }
}

void Luchador::actualizarCuchillos(float tiempoDelta)
{
    for (auto &cuchillo : cuchillos)
    {
        cuchillo.mover(tiempoDelta);
    }

    cuchillos.erase(
        std::remove_if(cuchillos.begin(), cuchillos.end(), [](Cuchillo &cuchillo)
                       { return cuchillo.getPosicion().x > 800; }),
        cuchillos.end());
}

void Luchador::drawHealthBar(sf::RenderWindow &window, sf::Vector2f position)
{
    int anchoBarraMax = 200;
    int altoBarra = 30;
    float anchoBarraActual = (static_cast<float>(health) / maxhealth) * anchoBarraMax;

    sf::RectangleShape barraTotal(sf::Vector2f(anchoBarraMax, altoBarra));
    barraTotal.setPosition(position);
    barraTotal.setFillColor(sf::Color(100, 100, 100));
    barraTotal.setOutlineColor(sf::Color::Black);
    barraTotal.setOutlineThickness(2);

    sf::RectangleShape barraSalud(sf::Vector2f(anchoBarraActual, altoBarra));
    barraSalud.setPosition(position);
    barraSalud.setFillColor(sf::Color::Green);

    window.draw(barraTotal);
    window.draw(barraSalud);
}
