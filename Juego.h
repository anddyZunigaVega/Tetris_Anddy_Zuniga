#ifndef JUEGO_H
#define JUEGO_H

#include <SFML/Graphics.hpp>
#include <string>

#include "Pieza.h"
#include "Tablero.h"
#include "ColaPiezas.h"
#include "Pila.h"
#include "ListaDoble.h"
#include "Ranking.h"

using namespace sf;
using namespace std;

const int TAM_CELDA = 32;
const int VENTANA_ANCHO = 980;
const int VENTANA_ALTO = 720;
const int VELOCIDAD_CAIDA_MS = 700;

enum EstadoJuego {
    MENU,
    JUGANDO,
    PAUSA,
    VER_RANKING,
    GAME_OVER,
    INGRESAR_NOMBRE,
    REPLAY
};

// Version en avance: interfaz TetrisA, con la pieza cayendo por
// gravedad y pudiendo moverse/rotar. La logica de puntos y lineas
// completas se agrega en los proximos pasos.
class Juego {
private:
    RenderWindow& ventana;
    const Font& fuente;
    Texture texturaJuego;
    Sprite fondoJuego;
    Texture texturaReplay;
    Sprite fondoReplay;
    Texture texturaMenu;
    Sprite fondoMenu;
    Clock relojGravedad;
    Clock relojMarco;
    Clock relojReplay;
    EstadoJuego estado;
    int seleccionMenu;

    Tablero tablero;
    ColaPiezas colaPiezas;
    Pila pilaHold;
    ListaDoble historial;
    Ranking ranking;

    int tipoActual;
    int rotActual;
    int filaActual;
    int colActual;
    int puntaje;
    int nivel;
    int lineas;
    int tiempoMs;
    bool holdUsado;
    string nombreJugador;
    int filasBorrar[20];
    int nFilasBorrar;
    int parpadeoMs;
    int algoritmoRanking;

    void reiniciarPartida();
    void generarPieza();
    bool piezaPuede(int f, int c, int r) const;
    void gravedad();
    void caerInstantaneo();
    void bloquearPieza();
    void usarHold();
    int puntajePorLineas(int n) const;
    void guardarRanking();
    void activarOpcion();
    int opcionEn(int x, int y) const;
    void capturarEstado(Estado& e) const;
    void restaurarEstado(const Estado& e);
    void deshacer();
    void rehacer();
    void iniciarReplay();

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
    void dibujarPausa();
    void dibujarGameOver();
    void dibujarReplayOverlay();
    void dibujarRanking(int px, int py, bool conTitulo);
    void dibujarProximamente(const string& titulo);
    bool clicEnBotonPausa(int x, int y) const;
    void dibujarBotonPausa() const;
    void dibujarPiezaEn(int tipo, int rot, int px, int py, int tam) const;

public:
    Juego(RenderWindow& ventana, const Font& fuente);
    void correr();
};

#endif
