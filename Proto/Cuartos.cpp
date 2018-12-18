#include "Cuartos.h"

int cuartos[4][4];
RectangleShape fondo[16];
RectangleShape pared[64];
RectangleShape portal[64];
RectangleShape meta;

const Color colPortalAbierto = Color(0,255,255);

unsigned indiceEntrada, indiceSalida;
float entradaX, entradaY, salidaX, salidaY;

void GenerarCuadriculaNivel()
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++)
            cuartos[j][i] = 0;
    }

    int actualX, actualY = 0, direccion = 0;

    // Entrada
    jIndiceY = actualY;
    jIndiceX = indiceEntrada = actualX = rand() % 4; // Columna aleatoria entre 0-3
    cuartos[actualX][actualY] = 1;

    while(actualY < 4){
        if(direccion == 0) direccion = rand() % 5;

        if(direccion == 0 || direccion == 1){   //Izquierda
            if(actualX > 0){
                if(cuartos[actualX-1][actualY] == 0) cuartos[--actualX][actualY] = 1;
                // Se iguala a 0 cuando queremos que elija aleatoriamente la dir
                direccion = 0;
            }else{
                if(actualY < 3){
                    cuartos[actualX][actualY] = 2;
                    cuartos[actualX][++actualY] = 3;
                    // Al bajar le damos dirección der para que se forme mejor
                    // el mapa (que no vaya solo hacia abajo)
                    direccion = 2;
                }else actualY++; // Sale del ciclo
            }
        }else if(direccion == 2 || direccion == 3){     //Derecha
            if(actualX < 3){
                if(cuartos[actualX+1][actualY] == 0) cuartos[++actualX][actualY] = 1;
                direccion = 0;
            }else{
                if(actualY < 3){
                    cuartos[actualX][actualY] = 2;
                    cuartos[actualX][++actualY] = 3;
                    // Al bajar le damos dirección izq para que se forme mejor
                    // el mapa (que no vaya solo hacia abajo)
                    direccion = 1;
                }else actualY++; // Sale del ciclo
            }
        }else{      //Abajo
            if(actualY < 3){
                cuartos[actualX][actualY] = 2;
                cuartos[actualX][++actualY] = 3;
                direccion = 0;
            }else actualY++; // Sale del ciclo
        }

    }
    indiceSalida = actualX;

    /*for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            std::cout << cuartos[j][i];
        }
        std::cout << std::endl;
    }*/
}

void InitCuarto(float x, float y, int& indicePared, int& indiceFondo, int colorFondo)
{
    float posX, posY;

     fondo[++indiceFondo].setSize(Vector2f(WCUARTO-WPARED/2, HCUARTO-WPARED/2));
    InitRectangulo(fondo[indiceFondo], Vector2f(x,y), Color(0,0,colorFondo));

    // **NO cambiar el orden en que se inicializan**
    // El orden es para identificar (en colisiones) qué tipo de pared es.

    // Pared izquierda
    pared[++indicePared].setSize(Vector2f(WPARED, HCUARTO - WPARED));
    posX = x - (WCUARTO/2) + (WPARED/2);
    InitRectangulo(pared[indicePared], Vector2f(posX,y), colorParedes);

    // Techo
    pared[++indicePared].setSize(Vector2f(WCUARTO, WPARED*2));
    posY = y - (HCUARTO/2);
    InitRectangulo(pared[indicePared], Vector2f(x,posY), colorParedes);

    // Pared derecha
    indicePared++; // No funciona si está dentro del índice
    pared[indicePared].setSize(pared[indicePared-2].getSize());
    posX += WCUARTO - WPARED;
    InitRectangulo(pared[indicePared], Vector2f(posX,y), colorParedes);

    // Piso
    indicePared++;
    pared[indicePared].setSize(pared[indicePared-2].getSize());
    posY += HCUARTO;
    InitRectangulo(pared[indicePared], Vector2f(x,posY), colorParedes);
}

void InitPortalesCuarto(float x, float y, int jCuarto, int iCuarto, int& indicePortal)
{
    int tipoCuarto = cuartos[jCuarto][iCuarto];
    float posX, posY, wPortal = WPARED/2;

    // Portales laterales
    if(tipoCuarto > 0){
        // Izquierdo
        if(jCuarto > 0){
            portal[++indicePortal].setSize(Vector2f(wPortal,HPORTAL));
            posX = x - WCUARTO/2 + WPARED - wPortal/2 + 1;
            InitRectangulo(portal[indicePortal], Vector2f(posX,y), colorPortales);
        }

        // Derecho
        if(jCuarto < 3){
            portal[++indicePortal].setSize(Vector2f(wPortal,HPORTAL));
            posX = x + WCUARTO/2 - WPARED + wPortal/2 - 1;
            InitRectangulo(portal[indicePortal], Vector2f(posX,y), colorPortales);
        }
    }else{
        // Pone los portales necesarios en un cuarto tipo 0
        // Izq
        if(jCuarto > 0 && cuartos[jCuarto-1][iCuarto] > 0 && cuartos[jCuarto-1][iCuarto] < 4){
            portal[++indicePortal].setSize(Vector2f(wPortal,HPORTAL));
            posX = x - WCUARTO/2 + WPARED - wPortal/2 + 1;
            InitRectangulo(portal[indicePortal], Vector2f(posX,y), colorPortales);
            cuartos[jCuarto][iCuarto] = 4; // Un cuarto con un solo portal.
        }
        // Der
        if(jCuarto < 3 && cuartos[jCuarto+1][iCuarto] > 0 && cuartos[jCuarto+1][iCuarto] < 4){
            portal[++indicePortal].setSize(Vector2f(wPortal,HPORTAL));
            posX = x + WCUARTO/2 - WPARED + wPortal/2 - 1;
            InitRectangulo(portal[indicePortal], Vector2f(posX,y), colorPortales);
            cuartos[jCuarto][iCuarto] = 4; // Un cuarto con un solo portal.
        }
    }

    // Superiores e inferiores
    switch(tipoCuarto){
        case 2:
            // Inferior
            portal[++indicePortal].setSize(Vector2f(HPORTAL,wPortal));
            posY = y + HCUARTO/2 - WPARED + wPortal/2 - 1;
            InitRectangulo(portal[indicePortal], Vector2f(x,posY), colorPortales);
            // Superior
            if(cuartos[jCuarto][iCuarto-1] == 2){
                portal[++indicePortal].setSize(Vector2f(HPORTAL,wPortal));
                posY = y - HCUARTO/2 + WPARED - wPortal/2 + 1;
                InitRectangulo(portal[indicePortal], Vector2f(x,posY), colorPortales);
            }
            break;
        case 3:
            // Superior
            portal[++indicePortal].setSize(Vector2f(HPORTAL,wPortal));
            posY = y - HCUARTO/2 + WPARED - wPortal/2 + 1;
            InitRectangulo(portal[indicePortal], Vector2f(x,posY), colorPortales);
            break;
    }
}

void PrepararCuartos()
{
    float posX = 0, posY = 0;
    int indiceFondo = -1, indicePared = -1, indicePortal = -1;
    int colorFondo = 204;

    for(unsigned i = 0; i < 4; i++){
        for(unsigned j = 0; j < 4; j++){
            InitCuarto(posX, posY, indicePared, indiceFondo, colorFondo);
            InitPortalesCuarto(posX, posY, j, i, indicePortal);
            if( (i == 0 && j != indiceEntrada) || (i == 3 && j != indiceSalida) || (i > 0 && i < 3) ){
                if(cuartos[j][i] > 0 && cuartos[j][i] != 4){
                    switch(rand() % 5){
                        case 0:
                        case 1:
                        case 2:
                        case 3: GenerarEnemigo(j, i, Vector2f(posX,posY));
                            break;
                    }
                }
                if(cuartos[j][i] == 4 && rand() % 2 == 0) InitCorazon(Vector2f(posX,posY));
            }

            if(i == 3 && j == indiceSalida) GenerarJefe(j, i, Vector2f(posX,posY));
            //std::cout << cuartos[j][i] << "  ";

            // Se encuentra la posición de la entrada y salida
            if(i == 0 && j == indiceEntrada){
                entradaX = posX;
                entradaY = posY;
            }else if(i == 3 && j == indiceSalida){
                salidaX = posX;
                salidaY = posY;
            }

            posX += WCUARTO;
        }
        posX = 0;
        posY += HCUARTO;
        colorFondo -= 50;
        //std::cout << "\n";
    }
    CambiarColorPortales();
}

void InitMeta(Vector2f pos)
{
    meta.setSize(Vector2f(TAMMETA,TAMMETA));
    InitRectangulo(meta, pos, colorMeta);
}

void CambiarColorPortales()
{
    for(int i = 0; i < 64; i++){
        if(numEnemigos[4*jIndiceY+jIndiceX] == 0) portal[i].setFillColor(colPortalAbierto);
        else portal[i].setFillColor(colorPortales);
    }
}




