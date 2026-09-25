#include "ColaPiezas.h"
#include <cstdlib>
#include <ctime>

ColaPiezas::ColaPiezas() {
    static bool semilla = false;
    if (!semilla) {
        srand((unsigned)time(0));
        semilla = true;
    }
    frente = 0;
    tam = 0;
}

void ColaPiezas::llenarSaco() {
    int saco[7];
    saco[0] = 0;
    saco[1] = 1;
    saco[2] = 2;
    saco[3] = 3;
    saco[4] = 4;
    saco[5] = 5;
    saco[6] = 6;

    int i;
    for (i = 6; i > 0; i--) {
        int j = rand() % (i + 1);
        int aux = saco[i];
        saco[i] = saco[j];
        saco[j] = aux;
    }

    for (i = 0; i < 7; i++) {
        push(saco[i]);
    }
}

void ColaPiezas::push(int tipo) {
    if (tam >= COLA_CAPACIDAD) {
        return;
    }
    int pos = posicionDe(tam);
    piezas[pos] = tipo;
    tam = tam + 1;
}

int ColaPiezas::pop() {
    int valor = piezas[frente];
    frente = posicionDe(1);
    tam = tam - 1;
    return valor;
}

int ColaPiezas::front() const {
    return piezas[frente];
}

bool ColaPiezas::empty() const {
    return tam == 0;
}

int ColaPiezas::tamano() const {
    return tam;
}

int ColaPiezas::posicionDe(int k) const {
    int pos = (frente + k) % COLA_CAPACIDAD;
    return pos;
}

int ColaPiezas::proximo(int k) const {
    int pos = posicionDe(k);
    return piezas[pos];
}

// llenado pre-carga la bolsa siguiente cuando quedan 3 piezas o menos
// preview de las 3 proximas nunca se queda sin piezas.
void ColaPiezas::rellenarSiFalta() {
    bool falta = (tam < 4) && (tam < COLA_CAPACIDAD);
    while (falta) {
        llenarSaco();
        falta = (tam < 4) && (tam < COLA_CAPACIDAD);
    }
}

void ColaPiezas::copiarA(int* buf, int& f, int& t) const {
    int i;
    for (i = 0; i < COLA_CAPACIDAD; i++) {
        buf[i] = piezas[i];
    }
    f = frente;
    t = tam;
}

void ColaPiezas::copiarDesde(const int* buf, int nFrente, int nTam) {
    int i;
    for (i = 0; i < COLA_CAPACIDAD; i++) {
        piezas[i] = buf[i];
    }
    frente = nFrente;
    tam = nTam;
    if (tam > COLA_CAPACIDAD) {
        tam = COLA_CAPACIDAD;
    }
}
