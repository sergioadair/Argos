//#include <cstdlib>
#include "Nivel.h"

int main()
{
    // Inicializar objetos
    PrerararVentana("Proto", true);

    while(true){
        InitNivel();
        GameLoop();
    }

    //system("pause");
    return 0;
}






