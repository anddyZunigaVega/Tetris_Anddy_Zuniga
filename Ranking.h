#ifndef RANKING_H
#define RANKING_H

#include <string>
using namespace std;

const int RANKING_MAX = 10;

struct Registro {
    string nombre;
    int puntos;
};

// Ranking top-10 de jugadores guardado en el archivo ranking.txt.
// Se mantiene ordenado descendentemente con ordenamiento por insercion.
class Ranking {
private:
    Registro top[RANKING_MAX];
    int cantidad;
    int algoritmo;
    void ordenarTop();
    void cargar();

public:
    Ranking();
    int agregar(const string& nombre, int puntos);
    void guardar() const;
    int getCantidad() const;
    Registro obtener(int i) const;
    void imprimir() const;
    void elegirAlgoritmo(int a);
    void reordenar();
};

#endif