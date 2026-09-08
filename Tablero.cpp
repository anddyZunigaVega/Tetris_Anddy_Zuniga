#include "Tablero.h"

NodoFila::NodoFila() {
    int c;
    for (c = 0; c < COLS_TABLERO; c++) {
        celdas[c] = 0;
    }
    anterior = 0;
    siguiente = 0;
}

Tablero::Tablero() {
    cabeza = 0;
    NodoFila* ultimo = 0;
    int i;
    for (i = 0; i < FILAS_TABLERO; i++) {
        NodoFila* n = new NodoFila();
        if (i == 0) {
            cabeza = n;
        } else {
            ultimo->siguiente = n;
            n->anterior = ultimo;
        }
        ultimo = n;
    }
    cola = ultimo;
}

Tablero::~Tablero() {
    NodoFila* n = cabeza;
    while (n != 0) {
        NodoFila* p = n->siguiente;
        delete n;
        n = p;
    }
}

void Tablero::limpiar() {
    NodoFila* n = cabeza;
    while (n != 0) {
        int c;
        for (c = 0; c < COLS_TABLERO; c++) {
            n->celdas[c] = 0;
        }
        n = n->siguiente;
    }
}

int* Tablero::fila(int i) {
    int* resultado = 0;
    NodoFila* n = cabeza;
    int k;
    for (k = 0; k < i; k++) {
        if (n != 0) {
            n = n->siguiente;
        }
    }
    if (n != 0) {
        resultado = n->celdas;
    }
    return resultado;
}

const int* Tablero::fila(int i) const {
    const int* resultado = 0;
    NodoFila* n = cabeza;
    int k;
    for (k = 0; k < i; k++) {
        if (n != 0) {
            n = n->siguiente;
        }
    }
    if (n != 0) {
        resultado = n->celdas;
    }
    return resultado;
}

void Tablero::setCelda(int f, int c, int valor) {
    int* filaP = fila(f);
    if (filaP != 0) {
        filaP[c] = valor;
    }
}

int Tablero::getCelda(int f, int c) const {
    int resultado = 0;
    const int* filaP = fila(f);
    if (filaP != 0) {
        resultado = filaP[c];
    }
    return resultado;
}

bool Tablero::colision(int tipo, int rot, int f, int c) const {
    int n = numCeldas(tipo);
    int k;
    for (k = 0; k < n; k++) {
        int df = CELDAS[tipo][rot][k][0];
        int dc = CELDAS[tipo][rot][k][1];
        int rf = f + df;
        int rc = c + dc;
        if (rf < 0) {
            return true;
        }
        if (rf >= FILAS_TABLERO) {
            return true;
        }
        if (rc < 0) {
            return true;
        }
        if (rc >= COLS_TABLERO) {
            return true;
        }
        if (getCelda(rf, rc) != 0) {
            return true;
        }
    }
    return false;
}

void Tablero::ponesFicha(int tipo, int rot, int f, int c) {
    int n = numCeldas(tipo);
    int k;
    for (k = 0; k < n; k++) {
        int df = CELDAS[tipo][rot][k][0];
        int dc = CELDAS[tipo][rot][k][1];
        int rf = f + df;
        int rc = c + dc;
        setCelda(rf, rc, tipo + 1);
    }
}

int Tablero::filasCompletas(int salida[20]) const {
    int cantidad = 0;
    NodoFila* n = cabeza;
    int f;
    for (f = 0; (f < FILAS_TABLERO) && (n != 0); f++) {
        bool completa = true;
        int c;
        for (c = 0; c < COLS_TABLERO; c++) {
            if (n->celdas[c] == 0) {
                completa = false;
            }
        }
        if (completa) {
            salida[cantidad] = f;
            cantidad = cantidad + 1;
        }
        n = n->siguiente;
    }
    return cantidad;
}

int Tablero::borrarFilas(int* indices, int n) {
    int i;
    NodoFila* nodo;
    NodoFila* sig;
    NodoFila* nuevo;

    bool* eliminar = new bool[FILAS_TABLERO];
    for (i = 0; i < FILAS_TABLERO; i++) {
        eliminar[i] = false;
    }
    for (i = 0; i < n; i++) {
        if (indices[i] >= 0) {
            if (indices[i] < FILAS_TABLERO) {
                eliminar[indices[i]] = true;
            }
        }
    }

    nodo = cabeza;
    int f = 0;
    while (nodo != 0) {
        sig = nodo->siguiente;
        if (eliminar[f]) {
            if (nodo->anterior != 0) {
                nodo->anterior->siguiente = nodo->siguiente;
            } else {
                cabeza = nodo->siguiente;
            }
            if (nodo->siguiente != 0) {
                nodo->siguiente->anterior = nodo->anterior;
            } else {
                cola = nodo->anterior;
            }
            delete nodo;
        }
        nodo = sig;
        f = f + 1;
    }

    for (i = 0; i < n; i++) {
        nuevo = new NodoFila();
        nuevo->siguiente = cabeza;
        if (cabeza != 0) {
            cabeza->anterior = nuevo;
        }
        cabeza = nuevo;
        if (cola == 0) {
            cola = cabeza;
        }
    }

    delete[] eliminar;
    return n;
}

int Tablero::celdasLlenas() const {
    int total = 0;
    NodoFila* n = cabeza;
    while (n != 0) {
        int c;
        for (c = 0; c < COLS_TABLERO; c++) {
            if (n->celdas[c] != 0) {
                total = total + 1;
            }
        }
        n = n->siguiente;
    }
    return total;
}