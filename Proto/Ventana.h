#ifndef VENTANA_H_INCLUDED
#define VENTANA_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

using namespace sf;

extern RenderWindow window;
extern View camara;

void PrerararVentana(const std::string titulo, bool fs = false);
void Eventos();

#endif // VENTANA_H_INCLUDED
