#ifndef JUEGO_H
#define JUEGO_H

#include <SFML/System.hpp>
#include <string>

#include "Pieza.h"
#include "Tablero.h"
#include "ColaPiezas.h"
#include "Pila.h"
#include "ColaEventos.h"
#include "ListaDoble.h"
#include "Ranking.h"

using namespace sf;
using namespace std;

const int TAM_CELDA = 32;
const int VENTANA_ANCHO = 980;
const int VENTANA_ALTO = 720;
const int VELOCIDAD_INICIAL_MS = 800;

enum EstadoJuego {
    MENU,
    JUGANDO,
    PAUSA,
    VER_RANKING,
    GAME_OVER,
    REPLAY
};

// Version en avance: Juego es el MODELO (logica dura del Tetris) en una
// arquitectura MVC. No contiene imagen, fondo ni dibujo: la Vista
// (GestorUI) se comunica a traves de comandos y getters con Juego&.
class Juego {
private:
    Clock relojGravedad;
    Clock relojMarco;
    Clock relojReplay;
    bool replayDesdeGameOver;
    EstadoJuego estado;

    Tablero tablero;
    ColaPiezas colaPiezas;
    Pila pilaHold;
    ColaEventos colaEventos;
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
    int velocidadMs;
    string mensajeEvento;
    int tiempoEventoMs;
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
    void bloquearPieza();
    void manejarEventosCola();
    int puntajePorLineas(int n) const;
    void capturarEstado(Estado& e) const;
    void restaurarEstado(const Estado& e);
    void registrarMovimiento();

public:
    Juego();
    void actualizar();

    // Comandos de navegacion de estados (para GestorUI)
    void iniciarNuevoJuego();
    void irARanking();
    void pausar();
    void reanudar();
    void volverAlMenu();
    void volverDeReplay();
    void setEstado(EstadoJuego e);
    void elegirAlgoritmoRanking(int alg);
    void guardarRanking();
    void iniciarReplay();
    void pasoReplayAtras();
    void pasoReplayAdelante();

    // Comandos de juego
    void moverIzquierda();
    void moverDerecha();
    void rotar();
    void bajarSuave();
    void caerInstantaneo();
    void usarHold();

    // Entrada de nombre (fin de partida)
    void agregarLetra(char c);
    void borrarLetra();

    // Getters para la Vista
    EstadoJuego getEstado() const;
    int getPuntaje() const;
    int getNivel() const;
    int getLineas() const;
    int getTiempoMs() const;
    int getVelocidadMs() const;
    string getMensajeEvento() const;
    int getTiempoEventoMs() const;
    string getNombreJugador() const;
    int getTipoActual() const;
    int getRotActual() const;
    int getFilaActual() const;
    int getColActual() const;
    int getTipoHold() const;
    int getCeldaTablero(int f, int c) const;
    int getProximaPieza(int k) const;
    int getCantidadProximas() const;
    int getNFilasBorrar() const;
    int getFilaBorrar(int i) const;
    int getParpadeoMs() const;
    bool getReplayDesdeGameOver() const;
    int getPasoReplay() const;
    int getTotalPasos() const;
    int getAlgoritmoRanking() const;
    int getCantidadRanking() const;
    Registro getRegistroRanking(int i) const;
};

#endif