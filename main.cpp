#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Juego.h"
#include "Ordenamiento.h"

int main() {
    cout << "=== Comparacion de algoritmos de ordenamiento ===\n";
    cout << "Insercion (O(n^2)) vs Quicksort (O(n log n))\n";
    compararOrdenamientos();
    cout << "Resultados guardados en ordenamientos.txt\n";

    RenderWindow ventana(VideoMode(VENTANA_ANCHO, VENTANA_ALTO),
                         "Tetris - Anddy Zuniga - Estructura de Datos",
                         Style::Titlebar | Style::Close);
    ventana.setFramerateLimit(60);

    Font fuente;
    if (!fuente.loadFromFile("assets/letras/arial.ttf")) {
        cout << "Aviso: no se encontro assets/letras/arial.ttf\n";
    }

    Juego juego(ventana, fuente);
    juego.correr();
    return 0;
}
