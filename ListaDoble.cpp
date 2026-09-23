#include "ListaDoble.h"

Estado::Estado() {
    int f;
    int c;
    for (f = 0; f < FILAS_TABLERO; f++) {
        for (c = 0; c < COLS_TABLERO; c++) {
            celdas[f][c] = 0;
        }
    }
    int i;
    for (i = 0; i < COLA_CAPACIDAD; i++) {
        colaBuf[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        siguientes[i] = 0;
    }
    colaFrente = 0;
    colaTam = 0;
    tipoActual = -1;
    rotActual = 0;
    filaActual = 0;
    colActual = 0;
    puntaje = 0;
    nivel = 1;
    lineas = 0;
    tipoHold = -1;
    orden = -1;
}

ListaDoble::ListaDoble() {
    primero = 0;
    ultimo = 0;
    actual = 0;
    tam = 0;
}

ListaDoble::~ListaDoble() {
    limpiar();
}

void ListaDoble::limpiar() {
    NodoEstado* n = primero;
    while (n != 0) {
        NodoEstado* s = n->siguiente;
        delete n;
        n = s;
    }
    primero = 0;
    ultimo = 0;
    actual = 0;
    tam = 0;
}

void ListaDoble::borrarFuturos() {
    if (actual != 0) {
        int borrados = 0;
        NodoEstado* n = actual->siguiente;
        while (n != 0) {
            NodoEstado* s = n->siguiente;
            delete n;
            n = s;
            borrados = borrados + 1;
        }
        actual->siguiente = 0;
        ultimo = actual;
        tam = tam - borrados;
    }
}

void ListaDoble::agregarEstado(const Estado& e) {
    borrarFuturos();
    NodoEstado* nuevo = new NodoEstado;
    nuevo->dato = e;
    nuevo->anterior = actual;
    nuevo->siguiente = 0;

    if (actual != 0) {
        nuevo->dato.orden = actual->dato.orden + 1;
        actual->siguiente = nuevo;
    } else {
        nuevo->dato.orden = 0;
        primero = nuevo;
    }

    actual = nuevo;
    ultimo = actual;
    tam = tam + 1;
}

bool ListaDoble::deshacer() {
    bool ok = false;
    if (actual != 0) {
        if (actual->anterior != 0) {
            actual = actual->anterior;
            ok = true;
        }
    }
    return ok;
}

bool ListaDoble::rehacer() {
    bool ok = false;
    if (actual != 0) {
        if (actual->siguiente != 0) {
            actual = actual->siguiente;
            ok = true;
        }
    }
    return ok;
}

void ListaDoble::irAlInicio() {
    actual = primero;
}

bool ListaDoble::avanzar() {
    bool ok = false;
    if (actual != 0) {
        if (actual->siguiente != 0) {
            actual = actual->siguiente;
            ok = true;
        }
    }
    return ok;
}

bool ListaDoble::hayAnterior() const {
    bool hay = false;
    if (actual != 0) {
        if (actual->anterior != 0) {
            hay = true;
        }
    }
    return hay;
}

bool ListaDoble::haySiguiente() const {
    bool hay = false;
    if (actual != 0) {
        if (actual->siguiente != 0) {
            hay = true;
        }
    }
    return hay;
}

int ListaDoble::ordenActual() const {
    int orden = -1;
    if (actual != 0) {
        orden = actual->dato.orden;
    }
    return orden;
}

int ListaDoble::tamano() const {
    return tam;
}

Estado ListaDoble::obtenerActual() const {
    Estado e;
    if (actual != 0) {
        e = actual->dato;
    }
    return e;
}