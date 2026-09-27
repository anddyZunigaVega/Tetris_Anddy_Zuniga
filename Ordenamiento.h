#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include "Ranking.h"

void ordenarInsercion(int* arreglo, int n);
void ordenarQuick(int* arreglo, int izq, int der);
void copiarArreglo(const int* origen, int* destino, int n);
void compararOrdenamientos();
void ordenarRegistrosInsercion(Registro* registros, int n);
void ordenarRegistrosQuick(Registro* registros, int izq, int der);

#endif
