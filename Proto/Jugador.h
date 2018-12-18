#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include <iostream>
#include <cmath>
#include "Proyectil.h"

#define TAMJUGADOR 20
#define VELMAX 5000 // 5750
#define VELMOV 2050 // 2750
#define FRENADO 0.993f

#define J_VIDA 4

extern RectangleShape jugador;
extern float jVelX, jVelY;
const Color colorJugador = Color::Green;
extern unsigned jIndiceX, jIndiceY;
extern bool jugadorMuerto;
extern int golpesRecibidos;

void InitJugador(float x, float y);
void ControlJugador();
void ControlDisparo();
void ActualizarVidaJugador(bool atacado = false);
void ParpadeoColor(float deltaTime);

#endif // JUGADOR_H_INCLUDED
