#ifndef TABLERO_H
#define TABLERO_H

#include "Pieza.h"

// Cada fila del tablero es un nodo de una lista enlazada doble.
// cabeza = fila 0 (arriba), cola = fila 19 (abajo).
// Cuando se borran filas se eliminan los nodos y se insertan nodos
// vacios al inicio (cabeza).
struct NodoFila {
    int celdas[COLS_TABLERO];
    NodoFila* anterior;
    NodoFila* siguiente;
    NodoFila();
};

class Tablero {
private:
    NodoFila* cabeza;
    NodoFila* cola;

public:
    Tablero();
    ~Tablero();
    void limpiar();
    int* fila(int i);
    const int* fila(int i) const;
    void setCelda(int fila, int col, int valor);
    int getCelda(int fila, int col) const;
    void ponesFicha(int tipo, int rot, int fila, int col);
    bool colision(int tipo, int rot, int fila, int col) const;
    int filasCompletas(int salida[20]) const;
    int borrarFilas(int* indices, int n);
    int celdasLlenas() const;
};

#endif
