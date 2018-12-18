#include "Enemigos.h"

Enemigo e1(Color::Red);
std::vector<Enemigo> enemigos;

Clock relojEnemigo;
unsigned numEnemigos[16];


void Enemigo::InitEnemigo(unsigned _indCuartoX, unsigned _indCuartoY, unsigned _tipo, int _vida, unsigned _rapidez, Vector2f _tam, Vector2f _direccion, Vector2f _pos)
{
    indCuartoX = _indCuartoX;
    indCuartoY = _indCuartoY;
    tipo = _tipo;
    vidaInicial = vida = _vida;
    rapidez = _rapidez;
    figura.setSize(_tam);
    velActual = _direccion * rapidez;
    posAnterior = _pos;
    enParedX = false;
    enParedY = false;
    InitRectangulo(figura, _pos, color);
    enemigos.push_back(e1);
}

void Enemigo::ColisionPared(unsigned tipoPared, float deltaTime)
{
    switch(tipo){
        case 0: // Jefe
            if(tipoPared % 2 == 0){
                if(enParedY && !enParedX){
                    velActual = Vector2f(velActual.x, -velActual.y);
                    figura.setPosition(posAnterior);
                    enParedX = true;
                    enParedY = false;
                }else if(!enParedY && enParedX){
                    figura.setPosition(posAnterior.x, figura.getPosition().y);
                }
            }else{
                if(velActual.x == 0){
                    int direcX = rand() % 2;
                    if(direcX == 0) direcX = -1;
                    velActual = Vector2f(direcX*rapidez, velActual.y);
                    figura.setPosition(figura.getPosition().x, posAnterior.y);
                    enParedY = true;
                }else{
                    if(enParedX && !enParedY){
                        velActual = Vector2f(-velActual.x, velActual.y);
                        figura.setPosition(posAnterior);
                        enParedY = true;
                        enParedX = false;
                    }else if(!enParedX && enParedY){
                        figura.setPosition(figura.getPosition().x, posAnterior.y);
                    }
                }
            }
            break;
        case 1:
            if(tipoPared % 2 == 0)
                velActual = Vector2f(-velActual.x, velActual.y);
            else
                velActual = Vector2f(velActual.x, -velActual.y);
            // Si el frame toma mucho tiempo el enemigo no se mueve.
            if(deltaTime < 0.07f) figura.move(velActual*deltaTime);
            break;
        case 2:
            if(tipoPared % 2 == 0)
                figura.setPosition(posAnterior.x, figura.getPosition().y);
            else
                figura.setPosition(figura.getPosition().x, posAnterior.y);
            break;
    }
}

bool Enemigo::ColisionBala(size_t i, Vector2f& posEnemigo)
{
    vida -= DANIOBALA;
    if(vida > 0){
        int nColor = (vida * 255 / vidaInicial);
        figura.setFillColor(Color(255, 255-nColor, 0));
        return false;
    }else{
        posEnemigo = figura.getPosition();
        enemigos.erase(enemigos.begin() + i);
        numEnemigos[4*jIndiceY+jIndiceX]--;
        return true;
    }
}

void GenerarEnemigo(unsigned indCuartoX, unsigned indCuartoY, Vector2f pos)
{
    // ****** Aqui se van a crear los distintos tipos de enemigos ****** //

    // Cantidad de enemigos en un cuarto.
    unsigned cantEnemigos = rand() % (E_CANTMAX - E_CANTMIN + 1) + E_CANTMIN;
    numEnemigos[4*indCuartoY+indCuartoX] = cantEnemigos;

    for(unsigned i = 0; i < cantEnemigos; i++){
        unsigned tipo = rand() % 2 + 1;
        int tamX = rand() % (E_TAMMAX - E_TAMMIN + 1) + E_TAMMIN;
        int tamY = rand() % (E_TAMMAX - E_TAMMIN + 1) + E_TAMMIN;
        int vida = tamX * tamY;
        int rapidez = rand() % (E_RAPIDEZMAX - E_RAPIDEZMIN + 1) + E_RAPIDEZMIN;
        Vector2f dir;

        switch(tipo){
            case 1: {
                int direcX = rand() % 2;
                if(direcX == 0) direcX = -1;

                int direcY = rand() % 2;
                if(direcY == 0) direcY = -1;
                dir = Vector2f(direcX, direcY);
            }
            case 2:
                /*  En este caso no se puede calcular la dirección aún
                    así que se le da cualquiera. La dir se calculará en el movimiento.
                */
                break;
        }

        e1.InitEnemigo(indCuartoX, indCuartoY, tipo, vida, rapidez, Vector2f(tamX, tamY), dir, pos);
    }
}

void GenerarJefe(unsigned indCuartoX, unsigned indCuartoY, Vector2f pos)
{
    numEnemigos[4*indCuartoY+indCuartoX] = 1;
    unsigned tipo = 0;
    int tamX = E_TAMMAX;
    int tamY = E_TAMMAX;
    int vida = tamX * tamY * 2;
    int rapidez = E_RAPIDEZMAX*1.5;
    int direcY = rand() % 2;
    if(direcY == 0) direcY = -1;
    Vector2f dir(0,direcY);

    e1.InitEnemigo(indCuartoX, indCuartoY, tipo, vida, rapidez, Vector2f(tamX, tamY), dir, pos);
}

float MoverEnemigos()
{
    Time deltaTime = relojEnemigo.restart();

    // Si el frame toma mucho tiempo el enemigo no se mueve.
    if(deltaTime.asSeconds() < 0.07f)
        for(size_t i = 0; i < enemigos.size(); i++){
            enemigos[i].posAnterior = enemigos[i].figura.getPosition();

            if(jIndiceX == enemigos[i].indCuartoX && jIndiceY == enemigos[i].indCuartoY){
                if(enemigos[i].tipo == 2){
                    Vector2f dir = jugador.getPosition() - enemigos[i].figura.getPosition();
                    Vector2f dirNorm = dir / (float)sqrt(pow(dir.x, 2) + pow(dir.y, 2));
                    enemigos[i].velActual = dirNorm * enemigos[i].rapidez;
                }else if(enemigos[i].tipo == 0){
                    if(enemigos[i].velActual.x != 0 && !enemigos[i].enParedX)
                        if(std::abs(enemigos[i].figura.getPosition().x - jugador.getPosition().x) <= 1)
                            enemigos[i].velActual = Vector2f(0, -enemigos[i].velActual.y);
                }
                enemigos[i].figura.move(enemigos[i].velActual*deltaTime.asSeconds());
            }
        }

    return deltaTime.asSeconds();
}




