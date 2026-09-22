#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include "Ranking.h"

void ordenarInsercion(int* a, int n);
void ordenarQuick(int* a, int izq, int der);
void copiarArreglo(const int* origen, int* destino, int n);
void compararOrdenamientos();
void ordenarRegistrosInsercion(Registro* r, int n);
void ordenarRegistrosQuick(Registro* r, int izq, int der);

#endif