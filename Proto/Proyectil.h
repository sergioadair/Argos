#ifndef PROYECTIL_H_INCLUDED
#define PROYECTIL_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include "Figuras.h"

#define DANIOBALA 196

struct Bala
{
    CircleShape figura;
    Vector2f velActual;
    float radio, rapidezMax, distMax, distRecorrida;

    Bala(float _radio,float _rapidezMax,float _distMax, Color _color = Color::Green) :
        radio(_radio), rapidezMax(_rapidezMax), distMax(_distMax)
    {
        InitCirculo(figura, radio, _color);
    }
};

extern std::vector<Bala> balas;

void InitBala(RectangleShape& jugador, Vector2f direccion);
void MoverBalas(float deltaTime);

#endif // PROYECTIL_H_INCLUDED
