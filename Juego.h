#ifndef JUEGO_H
#define JUEGO_H

#include <SFML/Graphics.hpp>
#include <string>

#include "Pieza.h"
#include "Tablero.h"
#include "ColaPiezas.h"

using namespace sf;
using namespace std;

const int TAM_CELDA = 32;
const int VENTANA_ANCHO = 980;
const int VENTANA_ALTO = 720;
const int VELOCIDAD_CAIDA_MS = 700;

enum EstadoJuego {
    MENU,
    JUGANDO,
    VER_RANKING
};

// Version en avance: interfaz como TetrisB, con la pieza cayendo por
// gravedad y pudiendo moverse/rotar. La logica de puntos y lineas
// completas se agrega en los proximos pasos.
class Juego {
private:
    RenderWindow& ventana;
    const Font& fuente;
    Clock relojGravedad;
    EstadoJuego estado;
    int seleccionMenu;

    Tablero tablero;
    ColaPiezas colaPiezas;

    int tipoActual;
    int rotActual;
    int filaActual;
    int colActual;

    void reiniciarPartida();
    void generarPieza();
    bool piezaPuede(int f, int c, int r) const;
    void gravedad();
    void bloquearPieza();
    void activarOpcion();
    int opcionEn(int x, int y) const;

    void procesarEventos();
    void actualizar();
    void render();

    void dibujarTexto(const string& s, int x, int y, unsigned tam,
                      const Color& color) const;
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