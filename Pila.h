#ifndef PILA_H
#define PILA_H

const int PILA_CAPACIDAD = 1;

// Pila LIFO de capacidad 1: guarda la pieza en espera (hold).
class Pila {
private:
    int datos[PILA_CAPACIDAD];
    int tope;

public:
    Pila();
    void vaciar();
    void push(int valor);
    int pop();
    int arriba() const;
    bool vacia() const;
    bool llena() const;
};

#endif