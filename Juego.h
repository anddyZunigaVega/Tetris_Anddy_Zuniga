#ifndef JUEGO_H
#define JUEGO_H

#include <SFML/Graphics.hpp>
#include <string>

#include "Pieza.h"
#include "Tablero.h"

using namespace sf;
using namespace std;

const int TAM_CELDA = 32;
const int VENTANA_ANCHO = 980;
const int VENTANA_ALTO = 720;

enum EstadoJuego {
    MENU,
    JUGANDO,
    VER_RANKING
};

class Juego {
private:
    RenderWindow& ventana;
    const Font& fuente;
    EstadoJuego estado;
    int seleccionMenu;

    Tablero tablero;

    void procesarEventos();
    void render();

    void dibujarTexto(const string& s, int x, int y, unsigned tam,const Color& color) const;
    Color colorDeTipo(int tipo) const;
    void dibujarFondo();
    void dibujarTablero();
    void dibujarPanel();
    void dibujarMenu();
    void dibujarProximamente(const string& titulo);
    void dibujarPiezaEn(int tipo, int rot, int px, int py, int tam) const;

public:
    Juego(RenderWindow& ventana, const Font& fuente);
    void correr();
};

#endif
