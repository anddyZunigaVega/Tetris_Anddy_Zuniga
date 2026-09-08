#ifndef PIEZA_H
#define PIEZA_H

const int FILAS_TABLERO = 20;
const int COLS_TABLERO = 10;
const int PIEZA_SPAWN_COL = 3;
const int PIEZA_SPAWN_FILA = 0;

enum TipoPieza {
    PIEZA_I = 0,
    PIEZA_O = 1,
    PIEZA_T = 2,
    PIEZA_S = 3,
    PIEZA_Z = 4,
    PIEZA_J = 5,
    PIEZA_L = 6
};


// La pieza BOMBA falta

// Las tablas de cada pieza (4 rotaciones x 4 celdas) estan
// definidas en Pieza.cpp.
extern const int CELDAS[7][4][4][2];

int numCeldas(int tipo);

#endif
