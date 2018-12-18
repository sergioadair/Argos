#ifndef ENEMIGOS_H_INCLUDED
#define ENEMIGOS_H_INCLUDED

#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>
#include "Jugador.h"

#define E_CANTMAX 3
#define E_CANTMIN 1

#define E_TAMMAX 70
#define E_TAMMIN 20
#define E_RAPIDEZMAX 300
#define E_RAPIDEZMIN 150

using namespace sf;

struct Enemigo
{
    RectangleShape figura;
    unsigned indCuartoX, indCuartoY, tipo;
    int vidaInicial, vida;
    float rapidez;
    Vector2f velActual;
    Color color;
    Vector2f posAnterior;
    bool enParedX, enParedY;

    Enemigo(const Color _color) : color(_color){}

    void InitEnemigo(unsigned, unsigned, unsigned, int, unsigned, Vector2f, Vector2f, Vector2f);
    void ColisionPared(unsigned, float);
    bool ColisionBala(size_t i, Vector2f& posEnemigo);
};

extern std::vector<Enemigo> enemigos;
extern unsigned numEnemigos[16];

void GenerarEnemigo(unsigned indCuartoX, unsigned indCuartoY, Vector2f pos);
void GenerarJefe(unsigned indCuartoX, unsigned indCuartoY, Vector2f pos);
float MoverEnemigos();

#endif // ENEMIGOS_H_INCLUDED
