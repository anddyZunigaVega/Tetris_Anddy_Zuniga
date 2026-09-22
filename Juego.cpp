#include "Juego.h"

#include <string>

using namespace sf;

using namespace std;

const int PX_TABLERO = VENTANA_ANCHO - COLS_TABLERO * TAM_CELDA - 40;
const int PY_TABLERO = 40;
const int PX_PANEL = 40;
const int PX_HOLD = (PX_TABLERO + PX_PANEL) / 2;
const int TAM_PREVIEW = 18;
const int TAM_HOLD = 22;
const int MENU_BTN_X = 360;
const int MENU_BTN_Y = 330;
const int MENU_BTN_ANCHO = 260;
const int MENU_BTN_ALTO = 48;
const int MENU_BTN_SEPARACION = 64;
const Color COLOR_MENU_AZUL = Color(66, 133, 244);
const Color COLOR_MENU_AZUL_MOUSE = Color(110, 168, 255);
const Color COLOR_MENU_VERDE = Color(52, 168, 83);
const Color COLOR_MENU_VERDE_MOUSE = Color(96, 210, 124);
const Color COLOR_MENU_ROJO = Color(219, 68, 55);
const Color COLOR_MENU_ROJO_MOUSE = Color(245, 105, 90);

Juego::Juego(RenderWindow& ventana, const Font& fuente)
    : ventana(ventana), fuente(fuente) {
    tablero.limpiar();
    colaPiezas = ColaPiezas();
    colaPiezas.rellenarSiFalta();
    estado = MENU;
    seleccionMenu = 0;
    tipoActual = -1;
    rotActual = 0;
    filaActual = 0;
    colActual = 0;
    puntaje = 0;
    nivel = 1;
    lineas = 0;
    tiempoMs = 0;
    holdUsado = false;
    nombreJugador = "";
    nFilasBorrar = 0;
    parpadeoMs = 0;
    algoritmoRanking = 0;
    relojGravedad.restart();
}

void Juego::correr() {
    while (ventana.isOpen()) {
        procesarEventos();
        actualizar();
        render();
    }
}

void Juego::reiniciarPartida() {
    tablero.limpiar();
    colaPiezas = ColaPiezas();
    colaPiezas.rellenarSiFalta();
    pilaHold.vaciar();
    tipoActual = -1;
    rotActual = 0;
    filaActual = 0;
    colActual = 0;
    puntaje = 0;
    nivel = 1;
    lineas = 0;
    tiempoMs = 0;
    holdUsado = false;
    nombreJugador = "";
    nFilasBorrar = 0;
    parpadeoMs = 0;
}

void Juego::generarPieza() {
    tipoActual = colaPiezas.pop();
    colaPiezas.rellenarSiFalta();
    rotActual = 0;
    filaActual = PIEZA_SPAWN_FILA;
    colActual = PIEZA_SPAWN_COL;
    holdUsado = false;
    if (!piezaPuede(filaActual, colActual, rotActual)) {
        if (puntaje > 0) {
            estado = INGRESAR_NOMBRE;
        } else {
            estado = GAME_OVER;
        }
    }
}

bool Juego::piezaPuede(int f, int c, int r) const {
    bool choca = tablero.colision(tipoActual, r, f, c);
    return !choca;
}

void Juego::gravedad() {
    bool puede = piezaPuede(filaActual + 1, colActual, rotActual);
    if (puede) {
        filaActual = filaActual + 1;
    } else {
        bloquearPieza();
    }
}

void Juego::bloquearPieza() {
    if (estado != JUGANDO) {
        return;
    }
    tablero.ponesFicha(tipoActual, rotActual, filaActual, colActual);

    int completas[20];
    int n = tablero.filasCompletas(completas);
    if (n > 0) {
        int i;
        for (i = 0; i < n; i++) {
            filasBorrar[i] = completas[i];
        }
        nFilasBorrar = n;
        parpadeoMs = 0;
        return;
    }

    generarPieza();
}

int Juego::puntajePorLineas(int n) const {
    if (n == 1) {
        return 40;
    }
    if (n == 2) {
        return 100;
    }
    if (n == 3) {
        return 300;
    }
    return 1200;
}

void Juego::usarHold() {
    if (holdUsado) {
        return;
    }
    if (tipoActual < 0) {
        return;
    }

    if (pilaHold.vacia()) {
        pilaHold.push(tipoActual);
        generarPieza();
    } else {
        int intercambio = pilaHold.pop();
        pilaHold.push(tipoActual);
        tipoActual = intercambio;
        rotActual = 0;
        filaActual = PIEZA_SPAWN_FILA;
        colActual = PIEZA_SPAWN_COL;
        holdUsado = true;

        if (!piezaPuede(filaActual, colActual, rotActual)) {
            if (puntaje > 0) {
                estado = INGRESAR_NOMBRE;
            } else {
                estado = GAME_OVER;
            }
        }
    }
}

void Juego::guardarRanking() {
    if (nombreJugador.empty()) {
        nombreJugador = "JUGADOR";
    }
    ranking.agregar(nombreJugador, puntaje);
    ranking.guardar();
    estado = GAME_OVER;
}

void Juego::activarOpcion() {
    if (seleccionMenu == 0) {
        reiniciarPartida();
        estado = JUGANDO;
        generarPieza();
    } else if (seleccionMenu == 1) {
        estado = VER_RANKING;
    } else {
        ventana.close();
    }
}

int Juego::opcionEn(int x, int y) const {
    int resultado = -1;
    int i;
    int px;
    int py;
    for (i = 0; i < 3; i++) {
        px = MENU_BTN_X;
        py = MENU_BTN_Y + i * MENU_BTN_SEPARACION;
        if (x >= px) {
            if (x < px + MENU_BTN_ANCHO) {
                if (y >= py) {
                    if (y < py + MENU_BTN_ALTO) {
                        resultado = i;
                    }
                }
            }
        }
    }
    return resultado;
}

void Juego::procesarEventos() {
    Event evento;
    while (ventana.pollEvent(evento)) {
        if (evento.type == Event::Closed) {
            ventana.close();
            return;
        }

        if (evento.type == Event::MouseMoved) {
            if (estado == MENU) {
                int i = opcionEn(evento.mouseMove.x, evento.mouseMove.y);
                if (i >= 0) {
                    seleccionMenu = i;
                }
            }
            continue;
        }

        if (evento.type == Event::MouseButtonPressed) {
            if (evento.mouseButton.button == Mouse::Left) {
                if (estado == MENU) {
                    int i = opcionEn(evento.mouseButton.x, evento.mouseButton.y);
                    if (i >= 0) {
                        seleccionMenu = i;
                        activarOpcion();
                    }
                }
            }
            continue;
        }

        if (evento.type == Event::TextEntered) {
            if (estado == INGRESAR_NOMBRE) {
                if (evento.text.unicode == 8) {
                    if (nombreJugador.size() > 0) {
                        nombreJugador.erase(nombreJugador.size() - 1);
                    }
                } else if (evento.text.unicode == 13) {
                    guardarRanking();
                } else if (evento.text.unicode >= 32) {
                    if (evento.text.unicode < 127) {
                        if (nombreJugador.size() < 10) {
                            nombreJugador =
                                nombreJugador + (char)evento.text.unicode;
                        }
                    }
                }
                continue;
            }
        }

        if (evento.type != Event::KeyPressed) {
            continue;
        }
        Keyboard::Key tecla = evento.key.code;

        if (estado == MENU) {
            if (tecla == Keyboard::Up || tecla == Keyboard::W) {
                seleccionMenu = (seleccionMenu + 2) % 3;
            } else if (tecla == Keyboard::Down || tecla == Keyboard::S) {
                seleccionMenu = (seleccionMenu + 1) % 3;
            } else if (tecla == Keyboard::Return) {
                activarOpcion();
            } else if (tecla == Keyboard::Escape) {
                ventana.close();
            }
        } else if (estado == JUGANDO) {
            if (tecla == Keyboard::Left || tecla == Keyboard::A) {
                if (piezaPuede(filaActual, colActual - 1, rotActual)) {
                    colActual = colActual - 1;
                }
            } else if (tecla == Keyboard::Right || tecla == Keyboard::D) {
                if (piezaPuede(filaActual, colActual + 1, rotActual)) {
                    colActual = colActual + 1;
                }
            } else if (tecla == Keyboard::Up || tecla == Keyboard::X) {
                int nuevaRot = (rotActual + 1) % 4;
                if (piezaPuede(filaActual, colActual, nuevaRot)) {
                    rotActual = nuevaRot;
                }
            } else if (tecla == Keyboard::Down || tecla == Keyboard::S) {
                if (piezaPuede(filaActual + 1, colActual, rotActual)) {
                    filaActual = filaActual + 1;
                } else {
                    bloquearPieza();
                }
            } else if (tecla == Keyboard::Space) {
                bool puede = piezaPuede(filaActual + 1, colActual, rotActual);
                while (puede) {
                    filaActual = filaActual + 1;
                    puede = piezaPuede(filaActual + 1, colActual, rotActual);
                }
                bloquearPieza();
            } else if (tecla == Keyboard::C) {
                usarHold();
            } else if (tecla == Keyboard::Return ||
                       tecla == Keyboard::Escape) {
                estado = MENU;
            }
        } else if (estado == GAME_OVER) {
            if (tecla == Keyboard::Return || tecla == Keyboard::Escape) {
                estado = MENU;
            }
        } else if (estado == INGRESAR_NOMBRE) {
            if (tecla == Keyboard::Escape) {
                estado = GAME_OVER;
            }
        } else if (estado == VER_RANKING) {
            if (tecla == Keyboard::Num1) {
                algoritmoRanking = 0;
                ranking.setAlgoritmo(algoritmoRanking);
                ranking.reordenar();
            } else if (tecla == Keyboard::Num2) {
                algoritmoRanking = 1;
                ranking.setAlgoritmo(algoritmoRanking);
                ranking.reordenar();
            } else if (tecla == Keyboard::Return || tecla == Keyboard::Escape) {
                estado = MENU;
            }
        }
    }
}

void Juego::actualizar() {
    if (estado != JUGANDO) {
        return;
    }
    Time dt = relojGravedad.getElapsedTime();
    int ms = dt.asMilliseconds();
    tiempoMs = tiempoMs + ms;

    if (nFilasBorrar > 0) {
        parpadeoMs = parpadeoMs + ms;
        if (parpadeoMs >= 400) {
            lineas = lineas + nFilasBorrar;
            puntaje = puntaje + puntajePorLineas(nFilasBorrar) * nivel;
            nivel = lineas / 10 + 1;
            tablero.borrarFilas(filasBorrar, nFilasBorrar);
            nFilasBorrar = 0;
            parpadeoMs = 0;
            relojGravedad.restart();
            generarPieza();
        }
        return;
    }

    if (ms >= VELOCIDAD_CAIDA_MS) {
        relojGravedad.restart();
        gravedad();
    }
}

void Juego::render() {
    ventana.clear(Color(40, 40, 45));

    if (estado == MENU || estado == VER_RANKING || estado == GAME_OVER ||
        estado == INGRESAR_NOMBRE) {
        dibujarFondo();
        if (estado == MENU) {
            dibujarMenu();
        } else if (estado == VER_RANKING) {
            dibujarRanking(260, 110, true);
        } else if (estado == GAME_OVER || estado == INGRESAR_NOMBRE) {
            dibujarGameOver();
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
    fondo.setFillColor(Color(40, 40, 45));
    ventana.draw(fondo);
}

void Juego::dibujarTexto(const string& s, int x, int y, unsigned tam,
                         const Color& color) const {
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
    RectangleShape fondo(Vector2f((float)(COLS_TABLERO * TAM_CELDA),
                                  (float)(FILAS_TABLERO * TAM_CELDA)));
    fondo.setPosition((float)PX_TABLERO, (float)PY_TABLERO);
    fondo.setFillColor(Color(52, 52, 58));
    fondo.setOutlineColor(Color(140, 140, 150));
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
            celda.setSize(Vector2f((float)(TAM_CELDA - 2),
                                   (float)(TAM_CELDA - 2)));
            Color colorCelda = colorDeTipo(v - 1);
            int b;
            bool parpadea = false;
            for (b = 0; b < nFilasBorrar; b++) {
                if (filasBorrar[b] == f) {
                    parpadea = true;
                }
            }
            if (parpadea && (parpadeoMs / 90) % 2 == 0) {
                colorCelda = Color(255, 255, 255);
            }
            celda.setFillColor(colorCelda);
            celda.setPosition((float)(PX_TABLERO + c * TAM_CELDA + 1),
                              (float)(PY_TABLERO + f * TAM_CELDA + 1));
            ventana.draw(celda);
        }
    }

    if (tipoActual >= 0) {
        dibujarPiezaEn(tipoActual, rotActual,
                       PX_TABLERO + colActual * TAM_CELDA,
                       PY_TABLERO + filaActual * TAM_CELDA, TAM_CELDA);
    }
}

void Juego::dibujarPanel() {
    int px = PX_PANEL;

    dibujarTexto("PUNTOS", px, 45, 20, Color(180, 180, 190));
    dibujarTexto(to_string(puntaje), px, 70, 40, Color(255, 220, 60));

    dibujarTexto("NIVEL", px, 120, 20, Color(180, 180, 190));
    dibujarTexto(to_string(nivel), px, 140, 34, Color(220, 120, 255));

    dibujarTexto("LINEAS", px, 180, 20, Color(180, 180, 190));
    dibujarTexto(to_string(lineas), px, 200, 34, Color(255, 120, 120));

    dibujarTexto("TIEMPO", px, 240, 20, Color(180, 180, 190));
    int segundos = tiempoMs / 1000;
    string txtTiempo = to_string(segundos) + " s";
    dibujarTexto(txtTiempo, px, 260, 30, Color(120, 220, 255));

    dibujarTexto("PROXIMAS", px, 300, 20, Color(180, 180, 190));
    int yPrev = 325;
    int k;
    for (k = 0; k < 3; k++) {
        if (k < colaPiezas.tamano()) {
            int pieza = colaPiezas.proximo(k);
            dibujarPiezaEn(pieza, 0, px + 8, yPrev, TAM_PREVIEW);
        }
        yPrev = yPrev + 70;
    }

    dibujarTexto("GUARDADA (C)", PX_HOLD, 45, 20, Color(180, 180, 190));
    int tipoHold = -1;
    if (!pilaHold.vacia()) {
        tipoHold = pilaHold.arriba();
    }
    dibujarPiezaEn(tipoHold, 0, PX_HOLD + 8, 70, TAM_HOLD);

    dibujarTexto("IZQ/DER: mover    ARRIBA/X: rotar",
                 px, 630, 16, Color(140, 140, 150));
    dibujarTexto("ABAJO/ESPACIO: bajar/caer    C: guardar",
                 px, 652, 16, Color(140, 140, 150));
    dibujarTexto("Enter/Esc: volver al menu",
                 px, 674, 16, Color(140, 140, 150));
}

void Juego::dibujarMenu() {
    dibujarTexto("TETRIS", 330, 120, 80, Color(255, 220, 60));
    dibujarTexto("Estructuras de Datos - Proyecto 1",
                 280, 220, 26, Color(220, 220, 220));
    dibujarTexto("Anddy Zuniga - UNA 2026 II",
                 320, 258, 20, Color(150, 150, 150));

    const char* opciones[3];
    opciones[0] = "Jugar";
    opciones[1] = "Ver ranking";
    opciones[2] = "Salir";

    int y = MENU_BTN_Y;
    int i;
    for (i = 0; i < 3; i++) {
        Color colorBase;
        Color colorMouse;
        if (i == 0) {
            colorBase = COLOR_MENU_AZUL;
            colorMouse = COLOR_MENU_AZUL_MOUSE;
        } else if (i == 1) {
            colorBase = COLOR_MENU_VERDE;
            colorMouse = COLOR_MENU_VERDE_MOUSE;
        } else {
            colorBase = COLOR_MENU_ROJO;
            colorMouse = COLOR_MENU_ROJO_MOUSE;
        }

        Color colorBoton;
        if (i == seleccionMenu) {
            colorBoton = colorMouse;
        } else {
            colorBoton = colorBase;
        }

        RectangleShape boton(Vector2f((float)MENU_BTN_ANCHO,
                                      (float)MENU_BTN_ALTO));
        boton.setPosition((float)MENU_BTN_X, (float)y);
        boton.setFillColor(colorBoton);
        if (i == seleccionMenu) {
            boton.setOutlineColor(Color(255, 255, 255, 200));
            boton.setOutlineThickness(3.0f);
        }
        ventana.draw(boton);

        RectangleShape brillo(Vector2f((float)MENU_BTN_ANCHO,
                                       (float)MENU_BTN_ALTO));
        brillo.setPosition((float)MENU_BTN_X, (float)y);
        brillo.setFillColor(Color(255, 255, 255, 28));
        ventana.draw(brillo);

        dibujarTexto(opciones[i], MENU_BTN_X + 20, y + 8, 26,
                     Color(255, 255, 255));

        y = y + MENU_BTN_SEPARACION;
    }

    dibujarTexto("Enter o click: elegir     Esc: salir",
                 360, 520, 18, Color(140, 140, 150));
}

void Juego::dibujarGameOver() {
    if (estado == INGRESAR_NOMBRE) {
        dibujarTexto("FIN DE LA PARTIDA", 280, 130, 60, Color(255, 80, 80));
        string txtPuntaje = "Puntaje: " + to_string(puntaje);
        dibujarTexto(txtPuntaje, 350, 210, 34, Color(255, 220, 60));
        dibujarTexto("Ingresa tu nombre (Enter para guardar)",
                     300, 290, 24, Color(220, 220, 220));
        dibujarTexto("Esc: no guardar en el ranking",
                     300, 325, 20, Color(150, 150, 150));
        string txtNombre = nombreJugador + "_";
        dibujarTexto(txtNombre, 360, 370, 40, Color(120, 220, 255));
        return;
    }

    dibujarTexto("GAME OVER", 290, 100, 70, Color(255, 80, 80));
    string txtPuntaje2 = "Puntaje: " + to_string(puntaje);
    dibujarTexto(txtPuntaje2, 350, 190, 34, Color(255, 220, 60));
    dibujarRanking(330, 250, true);
    dibujarTexto("Enter: menu", 330, 620, 20, Color(140, 140, 150));
}

void Juego::dibujarRanking(int px, int py, bool conTitulo) {
    int y;
    if (conTitulo) {
        string titulo = "RANKING TOP-10";
        if (algoritmoRanking == 0) {
            titulo = titulo + "  (insercion)";
        } else {
            titulo = titulo + "  (quicksort)";
        }
        dibujarTexto(titulo, px, py, 30, Color(255, 220, 60));
        y = py + 50;
    } else {
        y = py;
    }

    int n = ranking.getCantidad();
    if (n == 0) {
        dibujarTexto("(sin registros)", px, y, 22, Color(150, 150, 150));
        return;
    }

    int i;
    for (i = 0; i < n; i++) {
        if (i < 10) {
            Registro r = ranking.obtener(i);
            string linea = to_string(i + 1) + ". " + r.nombre +
                           "  " + to_string(r.puntos);
            dibujarTexto(linea, px, y, 22, Color(220, 220, 220));
            y = y + 30;
        }
    }

    if (conTitulo) {
        dibujarTexto("Teclea 1=insercion  2=quicksort para cambiar",
                     px, y + 5, 18, Color(140, 140, 150));
    }
}

void Juego::dibujarProximamente(const string& titulo) {
    dibujarTexto(titulo, 360, 200, 70, Color(255, 220, 60));
    dibujarTexto("Proximamente...",
                 380, 300, 30, Color(200, 200, 200));
    dibujarTexto("Enter/Esc: volver al menu",
                 350, 400, 22, Color(140, 140, 150));
}