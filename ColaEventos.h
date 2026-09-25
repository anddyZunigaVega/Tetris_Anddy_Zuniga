#ifndef COLAEVENTOS_H
#define COLAEVENTOS_H


enum TipoEvento {
    EVENTO_ACELERAR = 0,
    EVENTO_BONUS_TIEMPO = 1,
    EVENTO_BONUS_PUNTOS = 2
};

struct Evento {
    int momento;
    int tipo;
    Evento() : momento(0), tipo(0) {}
    Evento(int m, int t) : momento(m), tipo(t) {}
};

struct NodoEvento {
    Evento dato;
    NodoEvento* siguiente;
    NodoEvento(const Evento& e) : dato(e), siguiente(0) {}
};

// implementada como lista enlazada simplle
// siempre ordenada por el momento (ms) de disparo: el frente es
// el evento mas proximo a dispararse.
//Cada evento nuevo se inserta en la posicion correcta segun su momento (sin reordenar la lista).

class ColaEventos {
private:
    NodoEvento* frente;
    int cantidad;
    void copiarDe(const ColaEventos& otro);

public:
    ColaEventos();
    ~ColaEventos();
    ColaEventos(const ColaEventos& otro);
    ColaEventos& operator=(const ColaEventos& otro);
    void insertar(const Evento& e);
    Evento pop();
    const Evento& top() const;
    bool empty() const;
    int tamano() const;
    void limpiar();
};

#endif
