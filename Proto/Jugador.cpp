#include "Jugador.h"

RectangleShape jugador;
float jVelX = 0, jVelY = 0;
unsigned jIndiceX, jIndiceY;
bool jugadorMuerto = false;

float tiempoRecarga = 0.4f, recargaActual = 1;

int golpesRecibidos = 0;
float tiempoRecuperacion = 1, recuperacionActual = 1, tParpadeosColor = 0.2, tpcActual = 1;
Color colorJActual;

Clock relojDisparo, relojContoles, relojVida;
/*Si más de una función ocupa hacer un clock.restart()
 cada función debe hacerlo a un reloj diferente*/

void InitJugador(float x, float y)
{
    jugadorMuerto = false;
    golpesRecibidos = 0;
    jVelX = 0, jVelY = 0;
    jugador.setSize(Vector2f(TAMJUGADOR, TAMJUGADOR));
    colorJActual = colorJugador;
    InitRectangulo(jugador, Vector2f(x,y), colorJugador);
}

void ControlJugador()
{
    float deltaTime = relojContoles.restart().asSeconds();

    // Si el frame toma mucho tiempo el jugador no puede mover.
    if(deltaTime < 0.07f){
        if(std::abs(jVelX) < VELMAX){
            if(Keyboard::isKeyPressed(Keyboard::A)) jVelX -= VELMOV*deltaTime;
            if(Keyboard::isKeyPressed(Keyboard::D)) jVelX += VELMOV*deltaTime;
        }
        if(std::abs(jVelY) < VELMAX){
            if(Keyboard::isKeyPressed(Keyboard::W)) jVelY -= VELMOV*deltaTime;
            if(Keyboard::isKeyPressed(Keyboard::S)) jVelY += VELMOV*deltaTime;
        }

        jugador.move(Vector2f(jVelX, jVelY)*deltaTime);
        jVelX *= FRENADO;
        jVelY *= FRENADO;

        if(jVelX < 0.001f && jVelX > 0) jVelX = 0;
        if(jVelX < 0 && jVelX > -0.001f) jVelX = 0;
        if(jVelY < 0.001f && jVelY > 0) jVelY = 0;
        if(jVelY < 0 && jVelY > -0.001f) jVelY = 0;
    }
}

void ControlDisparo()
{
    Time deltaTime = relojDisparo.restart();

    // Si el frame toma mucho tiempo el jugador no puede disparar.
    if(deltaTime.asSeconds() < 0.07f){
        recargaActual += deltaTime.asSeconds();
        bool izqPres = Keyboard::isKeyPressed(Keyboard::Left);
        bool derPres = Keyboard::isKeyPressed(Keyboard::Right);
        bool arribaPres = Keyboard::isKeyPressed(Keyboard::Up);
        bool abajoPres = Keyboard::isKeyPressed(Keyboard::Down);

        if(recargaActual >= tiempoRecarga){
            if(izqPres && !derPres && !arribaPres && !abajoPres){
                InitBala(jugador, Vector2f(-1,0));
                recargaActual = 0;
            }
            if(derPres && !izqPres && !arribaPres && !abajoPres){
                InitBala(jugador, Vector2f(1,0));
                recargaActual = 0;
            }
            if(arribaPres && !izqPres && !derPres && !abajoPres){
                InitBala(jugador, Vector2f(0,-1));
                recargaActual = 0;
            }
            if(abajoPres && !izqPres && !derPres && !arribaPres){
                InitBala(jugador, Vector2f(0,1));
                recargaActual = 0;
            }
        }
        if(balas.size() > 0) MoverBalas(deltaTime.asSeconds());
    }
}

void ActualizarVidaJugador(bool atacado/* = false*/)
{
    Time deltaTime = relojVida.restart();

    if(deltaTime.asSeconds() < 0.07f){
        recuperacionActual += deltaTime.asSeconds();

        if(recuperacionActual >= tiempoRecuperacion)
            if(atacado){
                golpesRecibidos++;
                if(golpesRecibidos < J_VIDA){
                    colorJActual = jugador.getFillColor();
                    //ParpadeoColor(deltaTime);
                    recuperacionActual = 0;
                }else{
                    // ------- GAME OVER ------- //
                    jugadorMuerto = true;
                }
            }else{
                if(!jugadorMuerto){
                    int rojo, verde;
                    if(golpesRecibidos < J_VIDA/2){
                        rojo = golpesRecibidos * 255 / (J_VIDA/2) + 255/(J_VIDA/2);
                        verde = 255;
                    }else{
                        verde = (J_VIDA-golpesRecibidos) * 255 / (J_VIDA/2) - 255/(J_VIDA/2);
                        rojo = 255;
                    }
                    colorJActual = Color(rojo, verde, 0);
                    jugador.setFillColor(colorJActual);
                }
            }
        else ParpadeoColor(deltaTime.asSeconds());
    }
}

void ParpadeoColor(float deltaTime)
{
    //int nColor = 255 - ((J_VIDA-golpesRecibidos) * 255 / J_VIDA);

    tpcActual += deltaTime;
    if(tpcActual >= tParpadeosColor){
        if(jugador.getFillColor() == colorJActual){
            //jugador.setFillColor(Color(nColor, 255, nColor));
            jugador.setFillColor(Color::White);
        }else{
            jugador.setFillColor(colorJActual);
        }
        tpcActual = 0;
    }
}




