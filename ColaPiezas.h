#ifndef COLAPIEZAS_H
#define COLAPIEZAS_H

const int COLA_CAPACIDAD = 10;

// Cola FIFO de piezas implementada con arreglo circular.
// Se rellena con el algoritmo "7-bag": una sola bolsa de 7 piezas del
// Tetris en orden aleatorio; cuando le quedan 3 piezas se pre-carga la
// siguiente bolsa, asi el jugador siempre ve 3 piezas proximas.
class ColaPiezas {
private:
    int piezas[COLA_CAPACIDAD];
    int frente;
    int tam;
    int posicionDe(int k) const;
    void llenarSaco();

public:
    ColaPiezas();
    void push(int tipo);
    int pop();
    int front() const;
    bool empty() const;
    int tamano() const;
    int proximo(int k) const;
    void rellenarSiFalta();
    void copiarA(int* buf, int& frente, int& tam) const;
    void copiarDesde(const int* buf, int frente, int tam);
};

#endif
