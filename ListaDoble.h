#ifndef LISTADOBLE_H
#define LISTADOBLE_H

#include "Pieza.h"
#include "ColaPiezas.h"

// Estado completo del juego para un instante dado (snapshot).
struct Estado {
    int celdas[FILAS_TABLERO][COLS_TABLERO];
    int colaBuf[COLA_CAPACIDAD];
    int colaFrente;
    int colaTam;
    int tipoActual;
    int rotActual;
    int filaActual;
    int colActual;
    int puntaje;
    int nivel;
    int lineas;
    int tipoHold;
    int siguientes[3];
    int orden;
    Estado();
};

struct NodoEstado {
    Estado dato;
    NodoEstado* anterior;
    NodoEstado* siguiente;
};

// Lista enlazada doblemeGuarda el historial de la partida
// (cada pieza generada = un nodo). Permite deshacer, rehacer y
// reproducir recorriendo los nodos.
// Al agregar un estado nuevo se descarta la futura
class ListaDoble {
private:
    NodoEstado* primero;
    NodoEstado* ultimo;
    NodoEstado* actual;
    int tam;
    void borrarFuturos();

public:
    ListaDoble();
    ~ListaDoble();
    void limpiar();
    void agregarEstado(const Estado& e);
    bool deshacer();
    bool rehacer();
    void irAlInicio();
    bool avanzar();
    bool hayAnterior() const;
    bool haySiguiente() const;
    int ordenActual() const;
    int tamano() const;
    Estado obtenerActual() const;
};

#endif
