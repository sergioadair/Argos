#include "Ventana.h"

RenderWindow window;
View camara;

void PrerararVentana(const std::string titulo, bool fs)
{
    std::vector<VideoMode> videoModesDisp = VideoMode::getFullscreenModes();
    if(fs) window.create(videoModesDisp[0], titulo, Style::Fullscreen);
    else window.create(VideoMode(720,405), titulo);
    //window.setFramerateLimit(60); // NO lo descomentes
    window.setMouseCursorVisible(false);
}

void Eventos()
{
    Event event;
    while (window.pollEvent(event)){
        if (event.type == Event::Closed){
            // ¿Algo que hacer antes de cerrar?
            window.close();
            exit(0);
        }
    }
}







