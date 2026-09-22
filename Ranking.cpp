#include "Ranking.h"
#include "Ordenamiento.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

Ranking::Ranking() {
    cantidad = 0;
    algoritmo = 0;
    cargar();
}

void Ranking::cargar() {
    cantidad = 0;
    ifstream archivo("ranking.txt");
    if (archivo.is_open()) {
        string linea;
        bool seguir = true;
        while (seguir) {
            if (getline(archivo, linea)) {
                if (!linea.empty()) {
                    string::size_type ultimoEspacio = linea.rfind(' ');
                    if (ultimoEspacio != string::npos) {
                        top[cantidad].nombre = linea.substr(0, ultimoEspacio);
                        int p = atoi(linea.c_str() + ultimoEspacio + 1);
                        top[cantidad].puntos = p;
                        cantidad = cantidad + 1;
                    }
                }
                if (cantidad >= RANKING_MAX) {
                    seguir = false;
                }
            } else {
                seguir = false;
            }
        }
        archivo.close();
    }
    ordenarTop();
}

void Ranking::ordenarTop() {
    if (algoritmo == 1) {
        ordenarRegistrosQuick(top, 0, cantidad - 1);
        return;
    }
    ordenarRegistrosInsercion(top, cantidad);
}

void Ranking::setAlgoritmo(int a) {
    algoritmo = a;
    ordenarTop();
}

void Ranking::reordenar() {
    ordenarTop();
}

int Ranking::agregar(const string& nombre, int puntos) {
    int posicion = -1;
    if (cantidad < RANKING_MAX) {
        top[cantidad].nombre = nombre;
        top[cantidad].puntos = puntos;
        cantidad = cantidad + 1;
        ordenarTop();
    } else {
        if (puntos > top[RANKING_MAX - 1].puntos) {
            top[RANKING_MAX - 1].nombre = nombre;
            top[RANKING_MAX - 1].puntos = puntos;
            ordenarTop();
        } else {
            return -1;
        }
    }

    int i;
    for (i = 0; i < cantidad; i++) {
        if (top[i].puntos == puntos) {
            if (top[i].nombre == nombre) {
                posicion = i + 1;
            }
        }
    }
    return posicion;
}

void Ranking::guardar() const {
    ofstream archivo("ranking.txt");
    if (archivo.is_open()) {
        int i;
        for (i = 0; i < cantidad; i++) {
            archivo << top[i].nombre << " " << top[i].puntos << endl;
        }
        archivo.close();
    }
}

int Ranking::getCantidad() const {
    return cantidad;
}

Registro Ranking::obtener(int i) const {
    Registro r;
    r.nombre = "";
    r.puntos = 0;
    if (i >= 0) {
        if (i < cantidad) {
            r = top[i];
        }
    }
    return r;
}

void Ranking::imprimir() const {
    int i;
    for (i = 0; i < cantidad; i++) {
        cout << (i + 1) << ". " << top[i].nombre
             << " " << top[i].puntos << endl;
    }
}