#include "ColaEventos.h"

ColaEventos::ColaEventos() {
    frente = 0;
    cantidad = 0;
}

ColaEventos::~ColaEventos() {
    limpiar();
}

void ColaEventos::copiarDe(const ColaEventos& otro) {
    limpiar();
    NodoEvento* actual = otro.frente;
    while (actual != 0) {
        insertar(actual->dato);
        actual = actual->siguiente;
    }
}

ColaEventos::ColaEventos(const ColaEventos& otro) {
    frente = 0;
    cantidad = 0;
    copiarDe(otro);
}

ColaEventos& ColaEventos::operator=(const ColaEventos& otro) {
    if (this != &otro) {
        copiarDe(otro);
    }
    return *this;
}

bool ColaEventos::empty() const {
    return frente == 0;
}

int ColaEventos::tamano() const {
    return cantidad;
}

const Evento& ColaEventos::top() const {
    return frente->dato;
}

void ColaEventos::insertar(const Evento& e) {
    NodoEvento* nuevo = new NodoEvento(e);
    if (frente == 0 || e.momento < frente->dato.momento) {
        nuevo->siguiente = frente;
        frente = nuevo;
    } else {
        NodoEvento* anterior = frente;
        while (anterior->siguiente != 0 &&
               anterior->siguiente->dato.momento <= e.momento) {
            anterior = anterior->siguiente;
        }
        nuevo->siguiente = anterior->siguiente;
        anterior->siguiente = nuevo;
    }
    cantidad = cantidad + 1;
}

Evento ColaEventos::pop() {
    Evento raiz = frente->dato;
    NodoEvento* eliminar = frente;
    frente = frente->siguiente;
    delete eliminar;
    cantidad = cantidad - 1;
    return raiz;
}

void ColaEventos::limpiar() {
    NodoEvento* actual = frente;
    while (actual != 0) {
        NodoEvento* eliminar = actual;
        actual = actual->siguiente;
        delete eliminar;
    }
    frente = 0;
    cantidad = 0;
}
