#ifndef PISO_H
#define PISO_H

#include <SFML/Graphics.hpp>
#include "Animaciones.h"
class Piso {
public:
    friend class Animar;
    sf::RectangleShape rectan;

    Piso(float x, float y);
};

#endif
