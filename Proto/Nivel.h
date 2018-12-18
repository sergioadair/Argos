#ifndef NIVEL_H_INCLUDED
#define NIVEL_H_INCLUDED

#include <cstdlib>
#include "Ventana.h"
#include "Cuartos.h"

void InitNivel();
void GameLoop();
void RenderizarObjetos();
void Colisiones(Vector2f& posAnterior, float dtEnemigo);
int DireccionPortal(int i);
void MoverCamara(int direccion);
void LimpiarDatos();

#endif // NIVEL_H_INCLUDED
