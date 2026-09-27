#include "Ordenamiento.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;
using namespace std::chrono;

void ordenarInsercion(int* arreglo, int n) {
    int i;
    int j;
    int clave;
    for (i = 1; i < n; i++) {
        clave = arreglo[i];
        j = i - 1;
        while (j >= 0 && arreglo[j] > clave) {
            arreglo[j + 1] = arreglo[j];
            j = j - 1;
        }
        arreglo[j + 1] = clave;
    }
}

void ordenarQuick(int* arreglo, int izq, int der) {
    if (izq < der) {
        // Mover el pivote central al final para evitar el peor caso O(n^2), pag17
        int medio = izq + (der - izq) / 2;
        int auxMedio = arreglo[medio];
        arreglo[medio] = arreglo[der];
        arreglo[der] = auxMedio;

        int pivote = arreglo[der];
        int i = izq;
        for (int j = izq; j < der; j++) {
            if (arreglo[j] <= pivote) {
                int aux = arreglo[i];
                arreglo[i] = arreglo[j];
                arreglo[j] = aux;
                i++;
            }
        }
        int aux2 = arreglo[i];
        arreglo[i] = arreglo[der];
        arreglo[der] = aux2;

        ordenarQuick(arreglo, izq, i - 1);
        ordenarQuick(arreglo, i + 1, der);
    }
}

void copiarArreglo(const int* origen, int* destino, int n) {
    int i;
    for (i = 0; i < n; i++) {
        destino[i] = origen[i];
    }
}

void compararOrdenamientos() {
    static bool semilla = false;
    if (!semilla) {
        srand((unsigned)time(0));
        semilla = true;
    }

    const int TAMANOS[4] = {10, 100, 1000, 10000};
    const int REPETICIONES = 10;
    char linea[256];
    int pruebaActual;
    double usInsercion = 0.0;
    double usQuick = 0.0;

    printf("=== Comparacion de algoritmos de ordenamiento ===\n");
    printf("Insercion (O(n^2)) vs Quicksort (O(n log n))\n");
    printf("# Tiempo promedio de 10 corridas por algoritmo, en milisegundos\n\n");
    printf("%-8s %-14s %-12s %-16s %s\n\n", "Tamano", "Insercion (ms)", "Quick (ms)", "Mismo resultado", "Tiempo igual");

    ofstream archivo("ordenamientos.txt");
    if (archivo.is_open()) {
        archivo << "=== Comparacion de algoritmos de ordenamiento ===\n";
        archivo << "Insercion (O(n^2)) vs Quicksort (O(n log n))\n";
        archivo << "# Tiempo promedio de 10 corridas por algoritmo, en milisegundos\n";
        archivo << "Tamano Insercion_ms Quick_ms Mismo_resultado Tiempo_igual\n";
    }

    for (pruebaActual = 0; pruebaActual < 4; pruebaActual++) {
        int n = TAMANOS[pruebaActual];

        int* datos = new int[n];
        int i;
        for (i = 0; i < n; i++) {
            datos[i] = rand() % 100000;
        }

        int* copiaParaInsercion = new int[n];
        double totalIns = 0.0;
        time_point<high_resolution_clock> inicio;
        time_point<high_resolution_clock> fin;
        int repeticion;
        for (repeticion = 0; repeticion < REPETICIONES; repeticion++) {
            copiarArreglo(datos, copiaParaInsercion, n);
            inicio = high_resolution_clock::now();
            ordenarInsercion(copiaParaInsercion, n);
            fin = high_resolution_clock::now();
            totalIns = totalIns + duration<double, micro>(fin - inicio).count();
        }
        usInsercion = totalIns / REPETICIONES;

        int* copiaParaQuick = new int[n];
        double totalQuick = 0.0;
        for (repeticion = 0; repeticion < REPETICIONES; repeticion++) {
            copiarArreglo(datos, copiaParaQuick, n);
            inicio = high_resolution_clock::now();
            ordenarQuick(copiaParaQuick, 0, n - 1);
            fin = high_resolution_clock::now();
            totalQuick = totalQuick + duration<double, micro>(fin - inicio).count();
        }
        usQuick = totalQuick / REPETICIONES;

        bool iguales = true;
        for (i = 0; i < n; i++) {
            if (copiaParaInsercion[i] != copiaParaQuick[i]) {
                iguales = false;
            }
        }

        long msInsRedondeado = (long)(usInsercion / 1000.0 * 1000.0 + 0.5);
        long msQuickRedondeado = (long)(usQuick / 1000.0 * 1000.0 + 0.5);
        bool tiemposIguales = (msInsRedondeado == msQuickRedondeado);

        char resultado[4];
        if (iguales) {
            sprintf(resultado, "SI");
        } else {
            sprintf(resultado, "NO");
        }
        char tiempoIgual[4];
        if (tiemposIguales) {
            sprintf(tiempoIgual, "SI");
        } else {
            sprintf(tiempoIgual, "NO");
        }

        sprintf(linea, "%-8d %-14.3f %-12.3f %-16s %s", n, usInsercion / 1000.0, usQuick / 1000.0, resultado, tiempoIgual);
        printf("%s\n", linea);
        if (archivo.is_open()) {
            archivo << linea << "\n";
        }

        delete[] datos;
        delete[] copiaParaInsercion;
        delete[] copiaParaQuick;
    }

    printf("\nPara %d datos: insercion %.6f s, quicksort %.6f s\n", TAMANOS[3], usInsercion / 1000000.0, usQuick / 1000000.0);
    printf("Resultados guardados en ordenamientos.txt\n");

    if (archivo.is_open()) {
        archivo << "\nPara " << TAMANOS[3] << " datos: insercion " << usInsercion / 1000000.0 << " s, quicksort " << usQuick / 1000000.0 << " s\n";
        archivo << "Resultados guardados en ordenamientos.txt\n";
        archivo.close();
    }
}

void ordenarRegistrosInsercion(Registro* registros, int n) {
    int i;
    int j;
    Registro clave;
    for (i = 1; i < n; i++) {
        clave = registros[i];
        j = i - 1;
        while (j >= 0 && registros[j].puntos < clave.puntos) {
            registros[j + 1] = registros[j];
            j = j - 1;
        }
        registros[j + 1] = clave;
    }
}

void ordenarRegistrosQuick(Registro* registros, int izq, int der) {
    if (izq < der) {
        Registro pivote = registros[der];
        int i = izq;
        int j = izq;
        for (; j < der; j++) {
            if (registros[j].puntos > pivote.puntos) {
                Registro aux = registros[i];
                registros[i] = registros[j];
                registros[j] = aux;
                i = i + 1;
            }
        }
        Registro aux2 = registros[i];
        registros[i] = registros[der];
        registros[der] = aux2;
        ordenarRegistrosQuick(registros, izq, i - 1);
        ordenarRegistrosQuick(registros, i + 1, der);
    }
}
