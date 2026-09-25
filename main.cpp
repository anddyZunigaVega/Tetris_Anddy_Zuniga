#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Juego.h"
#include "GestorUI.h"
#include "Ordenamiento.h"

int main() {
    compararOrdenamientos();

    RenderWindow ventana(VideoMode(VENTANA_ANCHO, VENTANA_ALTO),"Tetris - Anddy Zuniga - Estructura de Datos",
						 Style::Titlebar | Style::Close);
    ventana.setFramerateLimit(60);

    Font fuente;
    if (!fuente.loadFromFile("assets/letras/arial.ttf")) {
        cout << "Aviso: no se encontro assets/letras/arial.ttf\n";
    }

    Juego juego;
    GestorUI ui(ventana, fuente);
    ui.inicializarRecursos(fuente);

    while (ventana.isOpen()) {
        Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == Event::Closed) {
                ventana.close();
            }
            ui.procesarEventos(evento, juego);
        }
        juego.actualizar();
        ui.dibujar(ventana, juego);
    }
    return 0;
}
