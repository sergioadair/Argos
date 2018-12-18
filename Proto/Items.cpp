#include "Items.h"

Corazon c1(7.f);
std::vector<Corazon> corazones;


void InitCorazon(Vector2f pos)
{
    c1.figura.setPosition(pos);
    corazones.push_back(c1);
}
