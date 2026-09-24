#include "Ordenamiento.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;
using namespace std::chrono;

void ordenarInsercion(int* a, int n) {
    int i;
    int j;
    int clave;
    for (i = 1; i < n; i++) {
        clave = a[i];
        j = i - 1;
        while (j >= 0 && a[j] > clave) {
            a[j + 1] = a[j];
            j = j - 1;
        }
        a[j + 1] = clave;
    }
}

void ordenarQuick(int* a, int izq, int der) {
    if (izq >= der) {
        return;
    }
    int i = izq;
    int j = der;
    int pivote = a[(izq + der) / 2];

    while (i <= j) {
        while (a[i] < pivote) {
            i = i + 1;
        }
        while (a[j] > pivote) {
            j = j - 1;
        }
        if (i <= j) {
            int aux = a[i];
            a[i] = a[j];
            a[j] = aux;
            i = i + 1;
            j = j - 1;
        }
    }

    if (izq < j) {
        ordenarQuick(a, izq, j);
    }
    if (i < der) {
        ordenarQuick(a, i, der);
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
    int t;
    double usIns = 0.0;
    double usQuick = 0.0;

    printf("=== Comparacion de algoritmos de ordenamiento ===\n");
    printf("Insercion (O(n^2)) vs Quicksort (O(n log n))\n");
    printf("# Tiempo promedio de 10 corridas por algoritmo, en milisegundos\n");
    printf("\n");
    printf("%-8s %-14s %-12s %-16s %s\n\n",
           "Tamano", "Insercion (ms)", "Quick (ms)",
           "Mismo resultado", "Tiempo igual");

    ofstream archivo("ordenamientos.txt");
    if (archivo.is_open()) {
        archivo << "=== Comparacion de algoritmos de ordenamiento ===\n";
        archivo << "Insercion (O(n^2)) vs Quicksort (O(n log n))\n";
        archivo << "# Tiempo promedio de 10 corridas por algoritmo, en milisegundos\n";
        archivo << "Tamano Insercion_ms Quick_ms Mismo_resultado Tiempo_igual\n";
    }

    for (t = 0; t < 4; t++) {
        int n = TAMANOS[t];

        int* datos = new int[n];
        int i;
        for (i = 0; i < n; i++) {
            datos[i] = rand() % 100000;
        }

        int* a = new int[n];
        double totalIns = 0.0;
        time_point<high_resolution_clock> inicio;
        time_point<high_resolution_clock> fin;
        int r;
        for (r = 0; r < REPETICIONES; r++) {
            copiarArreglo(datos, a, n);
            inicio = high_resolution_clock::now();
            ordenarInsercion(a, n);
            fin = high_resolution_clock::now();
            totalIns = totalIns +
                duration<double, micro>(fin - inicio).count();
        }
        usIns = totalIns / REPETICIONES;

        int* b = new int[n];
        double totalQuick = 0.0;
        for (r = 0; r < REPETICIONES; r++) {
            copiarArreglo(datos, b, n);
            inicio = high_resolution_clock::now();
            ordenarQuick(b, 0, n - 1);
            fin = high_resolution_clock::now();
            totalQuick = totalQuick +
                duration<double, micro>(fin - inicio).count();
        }
        usQuick = totalQuick / REPETICIONES;

        bool iguales = true;
        for (i = 0; i < n; i++) {
            if (a[i] != b[i]) {
                iguales = false;
            }
        }

        long msInsRedondeado = (long)(usIns / 1000.0 * 1000.0 + 0.5);
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

        sprintf(linea, "%-8d %-14.3f %-12.3f %-16s %s",
                n, usIns / 1000.0, usQuick / 1000.0,
                resultado, tiempoIgual);
        printf("%s\n", linea);
        if (archivo.is_open()) {
            archivo << linea << "\n";
        }

        delete[] datos;
        delete[] a;
        delete[] b;
    }

    printf("\nPara %d datos: insercion %.6f s, quicksort %.6f s\n",
           TAMANOS[3], usIns / 1000000.0, usQuick / 1000000.0);
    printf("Resultados guardados en ordenamientos.txt\n");

    if (archivo.is_open()) {
        archivo << "\nPara " << TAMANOS[3] << " datos: insercion "
                << usIns / 1000000.0 << " s, quicksort "
                << usQuick / 1000000.0 << " s\n";
        archivo << "Resultados guardados en ordenamientos.txt\n";
        archivo.close();
    }
}

void ordenarRegistrosInsercion(Registro* r, int n) {
    int i;
    int j;
    Registro clave;
    for (i = 1; i < n; i++) {
        clave = r[i];
        j = i - 1;
        while (j >= 0 && r[j].puntos < clave.puntos) {
            r[j + 1] = r[j];
            j = j - 1;
        }
        r[j + 1] = clave;
    }
}

void ordenarRegistrosQuick(Registro* r, int izq, int der) {
    if (izq < der) {
        Registro pivote = r[der];
        int i = izq;
        int j = izq;
        for (; j < der; j++) {
            if (r[j].puntos > pivote.puntos) {
                Registro aux = r[i];
                r[i] = r[j];
                r[j] = aux;
                i = i + 1;
            }
        }
        Registro aux2 = r[i];
        r[i] = r[der];
        r[der] = aux2;
        ordenarRegistrosQuick(r, izq, i - 1);
        ordenarRegistrosQuick(r, i + 1, der);
    }
}