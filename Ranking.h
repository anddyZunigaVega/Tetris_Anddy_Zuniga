#ifndef RANKING_H
#define RANKING_H

#include <string>
using namespace std;

const int RANKING_MAX = 10;

struct Registro {
    string nombre;
    int puntos;
};

// Ranking de jugadores guardado en el archivo ranking.txt. Se acumulan
// todos los resultados y al mostrarse solo se ven los mejores RANKING_MAX.
// Los registros se mantienen ordenados de mayor a menor con insercion o
// quicksort segun el algoritmo elegido, con array dinamico.
class Ranking {
private:
    Registro* top;
    int cantidad;
    int capacidad;
    int algoritmo;
    void asegurarCapacidad();
    void ordenarTop();
    void cargar();

public:
    Ranking();
    Ranking(const Ranking& otro);
    ~Ranking();
    Ranking& operator=(const Ranking& otro);
    int agregar(const string& nombre, int puntos);
    void guardar() const;
    int getCantidad() const;
    Registro obtener(int i) const;
    void imprimir() const;
    void elegirAlgoritmo(int a);
    void reordenar();
};

#endif
