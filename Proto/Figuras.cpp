#include "Figuras.h"

void InitRectangulo(RectangleShape& rect, Vector2f const& pos, Color const& color)
{
    rect.setFillColor(color);
    rect.setPosition(pos);
    rect.setOrigin(rect.getSize() * 0.5f); // El centro del rectángulo
}

void InitCirculo(CircleShape& circ, float radio, Color const& color)
{
    circ.setRadius(radio);
    circ.setFillColor(color);
    circ.setOrigin(radio, radio);
}
