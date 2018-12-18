#include "Proyectil.h"

Bala b1(5.f, 350.f, 320.f);
std::vector<Bala> balas;

void InitBala(RectangleShape& jugador, Vector2f direccion)
{
    b1.figura.setPosition(jugador.getPosition());
    b1.velActual = direccion * b1.rapidezMax;
    balas.push_back(b1);
}

void MoverBalas(float deltaTime)
{
    for(size_t i = 0; i < balas.size(); i++)
        if(balas[i].distRecorrida < balas[i].distMax){
            balas[i].figura.move(balas[i].velActual*deltaTime);
            balas[i].distRecorrida += balas[i].rapidezMax * deltaTime;
        }else{
            balas.erase(balas.begin() + i);
        }
}
