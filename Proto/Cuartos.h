#ifndef CUARTOS_H_INCLUDED
#define CUARTOS_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Enemigos.h"
#include "Items.h"

#define WCUARTO 720
#define HCUARTO 405
#define WPARED 25
#define HPORTAL 75

#define TAMMETA 30

const Color colorParedes = Color::Black;
const Color colorPortales = Color(105,105,105);
extern const Color colPortalAbierto;
const Color colorMeta = Color(0,255,255);

extern RectangleShape fondo[16];
extern RectangleShape pared[64];
extern RectangleShape portal[64];
extern RectangleShape meta;
extern unsigned indiceSalida;
extern float entradaX, entradaY;

void GenerarCuadriculaNivel();
void InitCuarto(float x, float y, int& indicePared, int colorFondo);
void InitPortalesCuarto(float x, float y, int j, int i, int& indicePortal);
void PrepararCuartos();
void InitMeta(Vector2f pos);
void CambiarColorPortales();

#endif // CUARTOS_H_INCLUDED
