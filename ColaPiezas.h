#ifndef COLAPIEZAS_H
#define COLAPIEZAS_H

const int COLA_CAPACIDAD = 21;

// Cola FIFO de piezas implementada con buffer circular.
// Se rellena con el algoritmo "7-bag": cada saco contiene las
// 7 piezas del Tetris en orden aleatorio (Fisher-Yates).
class ColaPiezas {
private:
    int buffer[COLA_CAPACIDAD];
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
    void exportar(int* buf, int& frente, int& tam) const;
    void restaurar(const int* buf, int frente, int tam);
};

#endif