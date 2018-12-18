#ifndef FIGURAS_H_INCLUDED
#define FIGURAS_H_INCLUDED

#include <SFML/Graphics.hpp>

using namespace sf;

void InitRectangulo(RectangleShape& rect, Vector2f const& pos, Color const& color);
void InitCirculo(CircleShape& circ, float radio, Color const& color);

#endif // FIGURAS_H_INCLUDED
