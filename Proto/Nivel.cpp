#include "Nivel.h"

void InitNivel()
{
    srand (time(NULL));

    GenerarCuadriculaNivel();
    PrepararCuartos();
    InitJugador(entradaX, entradaY);
}

void GameLoop()
{
    camara.reset(FloatRect(0, 0, 720, 405));
    camara.setCenter(jugador.getPosition());
    window.setView(camara);

    while (window.isOpen()){
        // Eventos
        Eventos();

        if(!jugadorMuerto){
            // Inputs y colisiones
            Vector2f posAnterior = jugador.getPosition();
            ControlJugador();
            ControlDisparo();
            float deltaTimeE = MoverEnemigos();
            ActualizarVidaJugador();

            Colisiones(posAnterior, deltaTimeE);

            // Limpia, renderiza y muestra
            RenderizarObjetos();
        }else{
            window.clear(Color::Black);
            window.display();
            LimpiarDatos();
            return;
        }
    }
}

void RenderizarObjetos()
{
    // Limpiar escena
    window.clear(Color::Black);

    // Renderizar objetos
    for(int i = 0; i < 16; i++) window.draw(fondo[i]);
    for(size_t i = 0; i < corazones.size(); i++) window.draw(corazones[i].figura);
    for(size_t i = 0; i < enemigos.size(); i++) window.draw(enemigos[i].figura);
    for(size_t i = 0; i < balas.size(); i++) window.draw(balas[i].figura);
    window.draw(jugador);
    for(int i = 0; i < 64; i++) window.draw(pared[i]);
    for(int i = 0; i < 64; i++) window.draw(portal[i]);
    window.draw(meta);

    window.display();
}

void Colisiones(Vector2f& posAnterior, float dtEnemigo)
{
    for(int i = 0; i < 64; i++){
        // PARED-JUGADOR
        if(jugador.getGlobalBounds().intersects(pared[i].getGlobalBounds())){
            if(i % 2 == 0){ // Es pared lateral
                jVelX = 0;
                jugador.setPosition(posAnterior.x, jugador.getPosition().y);
            }else{ // Es techo o piso
                jVelY = 0;
                jugador.setPosition(jugador.getPosition().x, posAnterior.y);
            }
        }
        // PARED-BALA
        for(size_t j = 0; j < balas.size(); j++)
            if(balas[j].figura.getGlobalBounds().intersects(pared[i].getGlobalBounds())){
                balas.erase(balas.begin() + j);
                break;
            }
        // PARED-ENEMIGO
        for(size_t j = 0; j < enemigos.size(); j++)
            if(enemigos[j].figura.getGlobalBounds().intersects(pared[i].getGlobalBounds())){
                enemigos[j].ColisionPared(i, dtEnemigo);
            }

        // PORTAL-JUGADOR
        if(jugador.getGlobalBounds().intersects(portal[i].getGlobalBounds())){
            if(numEnemigos[4*jIndiceY+jIndiceX] == 0){
                MoverCamara(DireccionPortal(i));
                CambiarColorPortales();
            }
        }
    }

    for(size_t i = 0; i < enemigos.size(); i++){
        // ENEMIGO-BALA
        for(size_t j = 0; j < balas.size(); j++)
            if(balas[j].figura.getGlobalBounds().intersects(enemigos[i].figura.getGlobalBounds())){
                balas.erase(balas.begin() + j);
                Vector2f posEnemigo;
                bool enemigoMuerto = enemigos[i].ColisionBala(i, posEnemigo);
                if(enemigoMuerto){
                    CambiarColorPortales();
                    if(jIndiceX == indiceSalida && jIndiceY == 3)
                        InitMeta(posEnemigo);
                }
                break;
            }
        // ENEMIGO-JUGADOR
        if(jugador.getGlobalBounds().intersects(enemigos[i].figura.getGlobalBounds()))
            ActualizarVidaJugador(true);
    }

    // CORAZÓN-JUGADOR
    for(size_t i = 0; i < corazones.size(); i++)
        if(jugador.getGlobalBounds().intersects(corazones[i].figura.getGlobalBounds())){
            if(golpesRecibidos > 0){
                corazones.erase(corazones.begin() + i);
                golpesRecibidos--;
                ActualizarVidaJugador();
            }
        }

    // META-JUGADOR
    if(jugador.getGlobalBounds().intersects(meta.getGlobalBounds())){
        window.close();
        exit(0);
    }
}

int DireccionPortal(int i)
{
    float x = camara.getCenter().x;
    float y = camara.getCenter().y;
    float cantMover = WPARED*2 + jugador.getSize().x + 5;

    // Se checa la velocidad para que el jugador entre solo cuando tenga la intención de entrar.

    // IZQ
    if(portal[i].getPosition().x < x && jVelX < 0){
        jVelX = 0;
        jVelY = 0;
        jIndiceX--;
        jugador.move(portal[i].getPosition().x - jugador.getPosition().x + portal[i].getSize().x/2 - cantMover, portal[i].getPosition().y - jugador.getPosition().y);
        return 0;
    }
    // SUP
    if(portal[i].getPosition().y < y && jVelY < 0){
        jVelX = 0;
        jVelY = 0;
        jIndiceY--;
        jugador.move(portal[i].getPosition().x - jugador.getPosition().x, portal[i].getPosition().y - jugador.getPosition().y + portal[i].getSize().y/2 - cantMover);
        return 1;
    }
    // DER
    if(portal[i].getPosition().x > x && jVelX > 0){
        jVelX = 0;
        jVelY = 0;
        jIndiceX++;
        jugador.move(portal[i].getPosition().x - jugador.getPosition().x - portal[i].getSize().x/2 + cantMover, portal[i].getPosition().y - jugador.getPosition().y);
        return 2;
    }
    // INF
    if(portal[i].getPosition().y > y && jVelY > 0){
        jVelX = 0;
        jVelY = 0;
        jIndiceY++;
        jugador.move(portal[i].getPosition().x - jugador.getPosition().x, portal[i].getPosition().y - jugador.getPosition().y - portal[i].getSize().y/2 + cantMover);
        return 3;
    }

    return -1; // Cuando toca al portal pero no quiere entrar.
}

void MoverCamara(int direccion)
{
    if(direccion >= 0){
        const float rangoError = 10.0f;
        const float velocidad = 1440;
        Vector2f objetivo, movimiento;

        switch(direccion){
            case 0: // Izq
                objetivo = Vector2f(camara.getCenter().x-camara.getSize().x, camara.getCenter().y);
                movimiento = Vector2f(-velocidad, 0.f);
                break;
            case 1: // Sup
                objetivo = Vector2f(camara.getCenter().x, camara.getCenter().y-camara.getSize().y);
                movimiento = Vector2f(0.f, -velocidad);
                break;
            case 2: // Der
                objetivo = Vector2f(camara.getCenter().x+camara.getSize().x, camara.getCenter().y);
                movimiento = Vector2f(velocidad, 0.f);
                break;
            case 3: // Inf
                objetivo = Vector2f(camara.getCenter().x, camara.getCenter().y+camara.getSize().y);
                movimiento = Vector2f(0.f, velocidad);
                break;
        }

        Clock clock;

        while(camara.getCenter() != objetivo){
            Time deltaTime = clock.restart();
            camara.move(movimiento*deltaTime.asSeconds());

            // Izq
            if(camara.getCenter().x > objetivo.x && camara.getCenter().x < objetivo.x+rangoError)
                camara.setCenter(objetivo.x, camara.getCenter().y);
            // Der
            if(camara.getCenter().x < objetivo.x && camara.getCenter().x > objetivo.x-rangoError)
                camara.setCenter(objetivo.x, camara.getCenter().y);

            // Arriba
            if(camara.getCenter().y > objetivo.y && camara.getCenter().y < objetivo.y+rangoError)
                camara.setCenter(camara.getCenter().x, objetivo.y);
            // Abajo
            if(camara.getCenter().y > objetivo.y && camara.getCenter().y < objetivo.y-rangoError)
                camara.setCenter(camara.getCenter().x, objetivo.y);

            window.setView(camara);
            RenderizarObjetos();
        }
    }
}

void LimpiarDatos()
{
    while(!corazones.empty()) corazones.pop_back();
    while(!enemigos.empty()) enemigos.pop_back();
    while(!balas.empty()) balas.pop_back();
    for(int i = 0; i < 16; i++) numEnemigos[i] = 0;
}




