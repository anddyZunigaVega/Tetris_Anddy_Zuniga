#include "Ranking.h"
#include "Ordenamiento.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

Ranking::Ranking() {
    cantidad = 0;
    capacidad = RANKING_MAX;
    algoritmo = 0;
    top = new Registro[capacidad];
    cargar();
}

Ranking::Ranking(const Ranking& otro) {
    cantidad = otro.cantidad;
    capacidad = otro.capacidad;
    algoritmo = otro.algoritmo;
    top = new Registro[capacidad];
    int i;
    for (i = 0; i < cantidad; i++) {
        top[i] = otro.top[i];
    }
}

Ranking::~Ranking() {
    delete[] top;
}

Ranking& Ranking::operator=(const Ranking& otro) {
    if (this != &otro) {
        delete[] top;
        cantidad = otro.cantidad;
        capacidad = otro.capacidad;
        algoritmo = otro.algoritmo;
        top = new Registro[capacidad];
        int i;
        for (i = 0; i < cantidad; i++) {
            top[i] = otro.top[i];
        }
    }
    return *this;
}

void Ranking::asegurarCapacidad() {
    if (cantidad < capacidad) {
        return;
    }
    int nuevaCap = capacidad * 2;
    Registro* nuevo = new Registro[nuevaCap];
    int i;
    for (i = 0; i < cantidad; i++) {
        nuevo[i] = top[i];
    }
    delete[] top;
    top = nuevo;
    capacidad = nuevaCap;
}

void Ranking::cargar() {
    cantidad = 0;
    ifstream archivo("ranking.txt");
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            if (!linea.empty()) {
                string::size_type ultimoEspacio = linea.rfind(' ');
                if (ultimoEspacio != string::npos) {
                    asegurarCapacidad();
                    top[cantidad].nombre = linea.substr(0, ultimoEspacio);
                    top[cantidad].puntos = atoi(linea.c_str() + ultimoEspacio + 1);
                    cantidad = cantidad + 1;
                }
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

void Ranking::elegirAlgoritmo(int a) {
    algoritmo = a;
    ordenarTop();
}

void Ranking::reordenar() {
    ordenarTop();
}

int Ranking::agregar(const string& nombre, int puntos) {
    asegurarCapacidad();
    top[cantidad].nombre = nombre;
    top[cantidad].puntos = puntos;
    cantidad = cantidad + 1;
    ordenarTop();

    int posicion = -1;
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