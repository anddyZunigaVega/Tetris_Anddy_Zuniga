#include "Pila.h"

Pila::Pila() {
    vaciar();
}

void Pila::vaciar() {
    tope = -1;
}

void Pila::push(int valor) {
    if (!llena()) {
        tope = tope + 1;
        datos[tope] = valor;
    }
}

int Pila::pop() {
    int valor = -1;
    if (!vacia()) {
        valor = datos[tope];
        tope = tope - 1;
    }
    return valor;
}

int Pila::arriba() const {
    int valor = -1;
    if (tope >= 0) {
        valor = datos[tope];
    }
    return valor;
}

bool Pila::vacia() const {
    return tope == -1;
}

bool Pila::llena() const {
    return tope == PILA_CAPACIDAD - 1;
}