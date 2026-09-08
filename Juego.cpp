#include "Juego.h"

#include <string>

using namespace sf;

using namespace std;

const int PX_TABLERO = 40;
const int PY_TABLERO = 40;
const int PX_PANEL = PX_TABLERO + COLS_TABLERO * TAM_CELDA + 40;
const int TAM_PREVIEW = 18;
const int TAM_HOLD = 22;

Juego::Juego(RenderWindow& ventana, const Font& fuente)
    : ventana(ventana), fuente(fuente) {
    tablero.limpiar();
    estado = MENU;
    seleccionMenu = 0;
}

void Juego::correr() {
    while (ventana.isOpen()) {
        procesarEventos();
        render();
    }
}

void Juego::procesarEventos() {
    Event evento;
    while (ventana.pollEvent(evento)) {
        if (evento.type == Event::Closed) {
            ventana.close();
            return;
        }
        if (evento.type != Event::KeyPressed) {
            continue;
        }
        Keyboard::Key tecla = evento.key.code;

        if (estado == MENU) {
            if (tecla == Keyboard::Up || tecla == Keyboard::W) {
                seleccionMenu = (seleccionMenu + 2) % 3;
            } else if (tecla == Keyboard::Down ||
                       tecla == Keyboard::S) {
                seleccionMenu = (seleccionMenu + 1) % 3;
            } else if (tecla == Keyboard::Return ||
                       tecla == Keyboard::Escape) {
                if (seleccionMenu == 0) {
                    estado = JUGANDO;
                } else if (seleccionMenu == 1) {
                    estado = VER_RANKING;
                } else {
                    ventana.close();
                }
            }
        } else if (estado == JUGANDO || estado == VER_RANKING) {
            if (tecla == Keyboard::Return ||
                tecla == Keyboard::Escape) {
                estado = MENU;
            }
        }
    }
}

void Juego::render() {
    ventana.clear(Color(20, 20, 30));

    if (estado == MENU || estado == VER_RANKING) {
        dibujarFondo();
        if (estado == MENU) {
            dibujarMenu();
        } else {
            dibujarProximamente("VER RANKING");
        }
        ventana.display();
        return;
    }

    dibujarTablero();
    dibujarPanel();
    ventana.display();
}

void Juego::dibujarFondo() {
    RectangleShape fondo(Vector2f((float)VENTANA_ANCHO, (float)VENTANA_ALTO));
    fondo.setFillColor(Color(18, 18, 28));
    ventana.draw(fondo);
}

void Juego::dibujarTexto(const string& s, int x, int y, unsigned tam, const Color& color) const {
    Text t;
    t.setFont(fuente);
    t.setString(s);
    t.setCharacterSize(tam);
    t.setFillColor(color);
    t.setPosition((float)x, (float)y);
    ventana.draw(t);
}

Color Juego::colorDeTipo(int tipo) const {
    if (tipo == PIEZA_I) {
        return Color(0, 210, 255);
    }
    if (tipo == PIEZA_O) {
        return Color(255, 225, 0);
    }
    if (tipo == PIEZA_T) {
        return Color(190, 0, 255);
    }
    if (tipo == PIEZA_S) {
        return Color(20, 220, 60);
    }
    if (tipo == PIEZA_Z) {
        return Color(255, 40, 40);
    }
    if (tipo == PIEZA_J) {
        return Color(40, 100, 255);
    }
    if (tipo == PIEZA_L) {
        return Color(255, 140, 0);
    }
    return Color(150, 150, 150);
}

void Juego::dibujarPiezaEn(int tipo, int rot, int px, int py, int tam) const {
    if (tipo < 0) {
        return;
    }
    Color color = colorDeTipo(tipo);
    int n = numCeldas(tipo);
    int k;
    RectangleShape celda;
    for (k = 0; k < n; k++) {
        int df = CELDAS[tipo][rot][k][0];
        int dc = CELDAS[tipo][rot][k][1];
        celda.setSize(Vector2f((float)(tam - 2), (float)(tam - 2)));
        celda.setFillColor(color);
        celda.setPosition((float)(px + dc * tam), (float)(py + df * tam));
        ventana.draw(celda);
    }
}

void Juego::dibujarTablero() {
    RectangleShape fondo(
        Vector2f((float)(COLS_TABLERO * TAM_CELDA),(float)(FILAS_TABLERO * TAM_CELDA)));
    fondo.setPosition((float)PX_TABLERO, (float)PY_TABLERO);
    fondo.setFillColor(Color(30, 30, 42));
    fondo.setOutlineColor(Color(120, 120, 140));
    fondo.setOutlineThickness(3.0f);
    ventana.draw(fondo);

    RectangleShape celda;
    int f;
    int c;
    for (f = 0; f < FILAS_TABLERO; f++) {
        for (c = 0; c < COLS_TABLERO; c++) {
            int v = tablero.getCelda(f, c);
            if (v == 0) {
                continue;
            }
            celda.setSize(Vector2f((float)(TAM_CELDA - 2),(float)(TAM_CELDA - 2)));
            celda.setFillColor(colorDeTipo(v - 1));
            celda.setPosition((float)(PX_TABLERO + c * TAM_CELDA + 1),(float)(PY_TABLERO + f * TAM_CELDA + 1));
            ventana.draw(celda);
        }
    }
}

void Juego::dibujarPanel() {
    int px = PX_PANEL;

    dibujarTexto("PUNTOS", px, 45, 20, Color(180, 180, 190));
    dibujarTexto("0", px, 70, 40, Color(255, 220, 60));

    dibujarTexto("NIVEL", px, 120, 20, Color(180, 180, 190));
    dibujarTexto("1", px, 140, 34, Color(220, 120, 255));

    dibujarTexto("LINEAS", px, 180, 20, Color(180, 180, 190));
    dibujarTexto("0", px, 200, 34, Color(255, 120, 120));

    dibujarTexto("TIEMPO", px, 240, 20, Color(180, 180, 190));
    dibujarTexto("0 s", px, 260, 30, Color(120, 220, 255));

    dibujarTexto("PROXIMAS", px, 300, 20, Color(180, 180, 190));
    int yPrev = 325;
    int k;
    int colaVisual[3];
    colaVisual[0] = PIEZA_T;
    colaVisual[1] = PIEZA_S;
    colaVisual[2] = PIEZA_J;
    for (k = 0; k < 3; k++) {
        dibujarPiezaEn(colaVisual[k], 0, px + 8, yPrev, TAM_PREVIEW);
        yPrev = yPrev + 70;
    }

    dibujarTexto("GUARDADA (C)", px, 520, 20, Color(180, 180, 190));
    dibujarPiezaEn(PIEZA_I, 0, px + 8, 545, TAM_HOLD);

    dibujarTexto("IZQ/DER: mover    ARRIBA/X: rotar",px, 630, 16, Color(140, 140, 150));
    dibujarTexto("ABAJO: bajar    ESPACIO: caer",px, 652, 16, Color(140, 140, 150));
    dibujarTexto("Enter/Esc: volver al menu", px, 674, 16, Color(140, 140, 150));
}

void Juego::dibujarMenu() {
    dibujarTexto("TETRIS", 330, 120, 80, Color(255, 220, 60));
    dibujarTexto("Estructuras de Datos - Proyecto 1",280, 220, 26, Color(220, 220, 220));
    dibujarTexto("Anddy Zuniga - UNA 2026 II",320, 258, 20, Color(150, 150, 150));

    const char* opciones[3];
    opciones[0] = "Jugar";
    opciones[1] = "Ver ranking";
    opciones[2] = "Salir";

    int y = 330;
    int i;
    for (i = 0; i < 3; i++) {
        string linea;
        if (i == seleccionMenu) {
            linea = "> ";
        }
        linea = linea + opciones[i];

        Color color;
        if (i == seleccionMenu) {
            color = Color(255, 220, 60);
        } else {
            color = Color(190, 190, 190);
        }
        dibujarTexto(linea, 360, y, 30, color);
        y = y + 48;
    }

    dibujarTexto("Enter: elegir opcion",360, 520, 18, Color(140, 140, 150));
}

void Juego::dibujarProximamente(const string& titulo) {
    dibujarTexto(titulo, 360, 200, 70, Color(255, 220, 60));
    dibujarTexto("Proximamente...",380, 300, 30, Color(200, 200, 200));
    dibujarTexto("Enter/Esc: volver al menu",350, 400, 22, Color(140, 140, 150));
}
