#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include "Figuras.h"

struct Corazon
{
    CircleShape figura;
    float radio;

    Corazon(float _radio, Color _color = Color(255,0,255)) : radio(_radio)
    {
        InitCirculo(figura, radio, _color);
    }
};

extern std::vector<Corazon> corazones;

void InitCorazon(Vector2f pos);

#endif // ITEMS_H_INCLUDED
