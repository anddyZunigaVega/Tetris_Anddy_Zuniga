#include "Juego.h"

#include <iostream>
#include <string>

using namespace sf;

using namespace std;

const int PX_TABLERO = VENTANA_ANCHO - COLS_TABLERO * TAM_CELDA - 80;
const int PY_TABLERO = 40;
const int PX_PANEL = 40;
const int PX_HOLD = (PX_TABLERO + PX_PANEL) / 2;
const int TAM_PREVIEW = 20;
const int TAM_HOLD = 22;
const int MENU_BTN_X = 310;
const int MENU_BTN_Y = 230;
const int MENU_BTN_ANCHO = 360;
const int MENU_BTN_ALTO = 64;
const int MENU_BTN_SEPARACION = 76;
const Color COLOR_MENU_AZUL = Color(66, 133, 244);
const Color COLOR_MENU_AZUL_MOUSE = Color(110, 168, 255);
const Color COLOR_MENU_VERDE = Color(52, 168, 83);
const Color COLOR_MENU_VERDE_MOUSE = Color(96, 210, 124);
const Color COLOR_MENU_ROJO = Color(219, 68, 55);
const Color COLOR_MENU_ROJO_MOUSE = Color(245, 105, 90);

const int BTN_PAUSA_X = 912;
const int BTN_PAUSA_Y = 8;
const int BTN_PAUSA_ANCHO = 60;
const int BTN_PAUSA_ALTO = 30;

Juego::Juego(RenderWindow& ventana, const Font& fuente)
    : ventana(ventana), fuente(fuente) {
    tablero.limpiar();
    colaPiezas = ColaPiezas();
    colaPiezas.rellenarSiFalta();
estado = MENU;
    seleccionMenu = 0;
    replayDesdeGameOver = false;
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
    relojMarco.restart();
    if (!texturaJuego.loadFromFile("assets/imagenes/juego.png")) {
        cout << "Aviso: no se encontro assets/imagenes/juego.png\n";
    }
    fondoJuego.setTexture(texturaJuego);
    if (!texturaReplay.loadFromFile("assets/imagenes/replay.png")) {
        cout << "Aviso: no se encontro assets/imagenes/replay.png\n";
    }
    fondoReplay.setTexture(texturaReplay);
    if (!texturaMenu.loadFromFile("assets/imagenes/mainMenu.png")) {
        cout << "Aviso: no se encontro assets/imagenes/mainMenu.png\n";
    }
    fondoMenu.setTexture(texturaMenu);
    if (!texturaPausa.loadFromFile("assets/imagenes/pausa.png")) {
        cout << "Aviso: no se encontro assets/imagenes/pausa.png\n";
    }
    fondoPausa.setTexture(texturaPausa);
    if (!texturaGameOver.loadFromFile("assets/imagenes/fondoGameOver.png")) {
        cout << "Aviso: no se encontro assets/imagenes/fondoGameOver.png\n";
    }
    fondoGameOver.setTexture(texturaGameOver);
    if (!texturaRanking.loadFromFile("assets/imagenes/ranking.png")) {
        cout << "Aviso: no se encontro assets/imagenes/ranking.png\n";
    }
    fondoRanking.setTexture(texturaRanking);
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
    historial.limpiar();
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
    relojGravedad.restart();
    relojMarco.restart();
}

void Juego::generarPieza() {
    tipoActual = colaPiezas.pop();
    colaPiezas.rellenarSiFalta();
    rotActual = 0;
    filaActual = PIEZA_SPAWN_FILA;
    colActual = PIEZA_SPAWN_COL;
    holdUsado = false;
if (!piezaPuede(filaActual, colActual, rotActual)) {
        estado = GAME_OVER;
        return;
    }

    Estado e;
    capturarEstado(e);
    historial.agregarEstado(e);
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

void Juego::caerInstantaneo() {
    int pasos = 0;
    bool puede = piezaPuede(filaActual + 1, colActual, rotActual);
    while (puede) {
        filaActual = filaActual + 1;
        pasos = pasos + 1;
        puede = piezaPuede(filaActual + 1, colActual, rotActual);
    }
    puntaje = puntaje + 2 * pasos;
    bloquearPieza();
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
    relojGravedad.restart();
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

void Juego::capturarEstado(Estado& e) const {
    int f;
    int c;
    for (f = 0; f < FILAS_TABLERO; f++) {
        for (c = 0; c < COLS_TABLERO; c++) {
            e.celdas[f][c] = tablero.getCelda(f, c);
        }
    }
    colaPiezas.copiarA(e.colaBuf, e.colaFrente, e.colaTam);
    e.tipoActual = tipoActual;
    e.rotActual = rotActual;
    e.filaActual = filaActual;
    e.colActual = colActual;
    e.puntaje = puntaje;
    e.nivel = nivel;
    e.lineas = lineas;

    if (pilaHold.vacia()) {
        e.tipoHold = -1;
    } else {
        e.tipoHold = pilaHold.arriba();
    }

    int k;
    for (k = 0; k < 3; k++) {
        if (k < colaPiezas.tamano()) {
            e.siguientes[k] = colaPiezas.proximo(k);
        } else {
            e.siguientes[k] = -1;
        }
    }
}

void Juego::restaurarEstado(const Estado& e) {
    tablero.limpiar();
    int f;
    int c;
    for (f = 0; f < FILAS_TABLERO; f++) {
        for (c = 0; c < COLS_TABLERO; c++) {
            tablero.setCelda(f, c, e.celdas[f][c]);
        }
    }
    colaPiezas.copiarDesde(e.colaBuf, e.colaFrente, e.colaTam);
    tipoActual = e.tipoActual;
    rotActual = e.rotActual;
    filaActual = e.filaActual;
    colActual = e.colActual;
    puntaje = e.puntaje;
    nivel = e.nivel;
    lineas = e.lineas;
    pilaHold.vaciar();
    if (e.tipoHold >= 0) {
        pilaHold.push(e.tipoHold);
    }
    holdUsado = false;
    nFilasBorrar = 0;
    parpadeoMs = 0;
    relojGravedad.restart();
}

void Juego::deshacer() {
    bool ok = historial.deshacer();
    if (ok) {
        restaurarEstado(historial.obtenerActual());
        estado = JUGANDO;
    }
}

void Juego::rehacer() {
    bool ok = historial.rehacer();
    if (ok) {
        restaurarEstado(historial.obtenerActual());
    }
}

void Juego::iniciarReplay() {
    replayDesdeGameOver = (estado == GAME_OVER);
    historial.irAlInicio();
    restaurarEstado(historial.obtenerActual());
    relojReplay.restart();
    estado = REPLAY;
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
        holdUsado = true;
    } else {
        int intercambio = pilaHold.pop();
        pilaHold.push(tipoActual);
        tipoActual = intercambio;
        rotActual = 0;
        filaActual = PIEZA_SPAWN_FILA;
        colActual = PIEZA_SPAWN_COL;
        holdUsado = true;

if (!piezaPuede(filaActual, colActual, rotActual)) {
            estado = GAME_OVER;
        }
    }
}

void Juego::guardarRanking() {
    if (nombreJugador.empty()) {
        nombreJugador = "JUGADOR";
    }
    ranking.agregar(nombreJugador, puntaje);
    ranking.guardar();
    estado = MENU;
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
            if (estado == JUGANDO) {
                if (clicEnBotonPausa(evento.mouseButton.x,evento.mouseButton.y)) {
                    relojGravedad.restart();
                    relojMarco.restart();
                    estado = PAUSA;
                }
            }
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
            if (estado == GAME_OVER) {
                if (evento.text.unicode == 8) {
                    if (nombreJugador.size() > 0) {
                        nombreJugador.erase(nombreJugador.size() - 1);
                    }
                } else if (evento.text.unicode == 13) {
                    guardarRanking();
                } else if (evento.text.unicode >= 32) {
                    if (evento.text.unicode < 127) {
                        if (nombreJugador.size() < 10) {
                            nombreJugador = nombreJugador + (char)evento.text.unicode;
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
            if (tecla == Keyboard::Escape) {
                ventana.close();
            }
        } else if (estado == JUGANDO) {
if (tecla == Keyboard::Left) {
                if (piezaPuede(filaActual, colActual - 1, rotActual)) {
                    colActual = colActual - 1;
                }
            } else if (tecla == Keyboard::Right) {
                if (piezaPuede(filaActual, colActual + 1, rotActual)) {
                    colActual = colActual + 1;
                }
            } else if (tecla == Keyboard::Up) {
                int nuevaRot = (rotActual + 1) % 4;
                if (piezaPuede(filaActual, colActual, nuevaRot)) {
                    rotActual = nuevaRot;
                }
            } else if (tecla == Keyboard::Down || tecla == Keyboard::S) {
                if (piezaPuede(filaActual + 1, colActual, rotActual)) {
                    filaActual = filaActual + 1;
                    puntaje = puntaje + 1;
                } else {
                    bloquearPieza();
                }
                relojGravedad.restart();
            } else if (tecla == Keyboard::Space) {
                caerInstantaneo();
            } else if (tecla == Keyboard::C) {
                usarHold();
} else if (tecla == Keyboard::D) {
                deshacer();
            } else if (tecla == Keyboard::A) {
                rehacer();
            } else if (tecla == Keyboard::P || tecla == Keyboard::Escape) {
                relojGravedad.restart();
                relojMarco.restart();
                estado = PAUSA;
            }
        } else if (estado == PAUSA) {
            if (tecla == Keyboard::P) {
                relojGravedad.restart();
                relojMarco.restart();
                estado = JUGANDO;
            } else if (tecla == Keyboard::R) {
                if (historial.tamano() > 0) {
                    iniciarReplay();
                }
            } else if (tecla == Keyboard::Escape) {
                estado = MENU;
            }
} else if (estado == GAME_OVER) {
            if (tecla == Keyboard::R) {
                if (historial.tamano() > 0) {
                    iniciarReplay();
                }
            } else if (tecla == Keyboard::Escape) {
                estado = MENU;
            }
        } else if (estado == REPLAY) {
            if (tecla == Keyboard::Left || tecla == Keyboard::A) {
                bool ok = historial.deshacer();
                if (ok) {
                    restaurarEstado(historial.obtenerActual());
                    relojReplay.restart();
                }
            } else if (tecla == Keyboard::Right || tecla == Keyboard::D) {
                bool ok = historial.avanzar();
                if (ok) {
                    restaurarEstado(historial.obtenerActual());
                    relojReplay.restart();
                }
} else if (tecla == Keyboard::Return || tecla == Keyboard::Escape) {
                if (replayDesdeGameOver) {
                    estado = GAME_OVER;
                } else {
                    estado = MENU;
                }
            }
        } else if (estado == VER_RANKING) {
            if (tecla == Keyboard::Num1) {
                algoritmoRanking = 0;
                ranking.elegirAlgoritmo(algoritmoRanking);
                ranking.reordenar();
            } else if (tecla == Keyboard::Num2) {
                algoritmoRanking = 1;
                ranking.elegirAlgoritmo(algoritmoRanking);
                ranking.reordenar();
            } else if (tecla == Keyboard::Return || tecla == Keyboard::Escape) {
                estado = MENU;
            }
        }
    }
}

void Juego::actualizar() {
    Time dt = relojMarco.restart();
    int ms = dt.asMilliseconds();

    if (estado == JUGANDO) {
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

        int msGravedad = relojGravedad.getElapsedTime().asMilliseconds();
        if (msGravedad >= VELOCIDAD_CAIDA_MS) {
            relojGravedad.restart();
            gravedad();
        }
    } else if (estado == REPLAY) {
        int msReplay = relojReplay.getElapsedTime().asMilliseconds();
        if (msReplay >= 400) {
            relojReplay.restart();
            bool siguiente = historial.avanzar();
            if (siguiente) {
                restaurarEstado(historial.obtenerActual());
            }
        }
    }
}

void Juego::render() {
    ventana.clear(Color(40, 40, 45));
    if (estado == MENU) {
        ventana.draw(fondoMenu);
    } else if (estado == PAUSA) {
        ventana.draw(fondoPausa);
    } else if (estado == REPLAY) {
        ventana.draw(fondoReplay);
    } else if (estado == GAME_OVER) {
        ventana.draw(fondoGameOver);
    } else if (estado == VER_RANKING) {
        ventana.draw(fondoRanking);
    } else {
        ventana.draw(fondoJuego);
    }

    if (estado != JUGANDO && estado != REPLAY) {
        if (estado != MENU && estado != PAUSA && estado != GAME_OVER
            && estado != VER_RANKING) {
            dibujarFondo();
        }
        if (estado == MENU) {
            dibujarMenu();
        } else if (estado == VER_RANKING) {
            dibujarRanking();
} else if (estado == GAME_OVER) {
            dibujarGameOver();
        } else if (estado == PAUSA) {
            dibujarPausa();
        }
        ventana.display();
        return;
    }

    dibujarTablero();
    dibujarPanel();

    if (estado == JUGANDO) {
        dibujarBotonPausa();
    } else if (estado == REPLAY) {
        dibujarReplayOverlay();
    }

    ventana.display();
}

void Juego::dibujarFondo() {
    RectangleShape fondo(Vector2f((float)VENTANA_ANCHO, (float)VENTANA_ALTO));
    fondo.setFillColor(Color(40, 40, 45));
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

    if (tipoActual >= 0 && nFilasBorrar == 0) {
        dibujarPiezaEn(tipoActual, rotActual,PX_TABLERO + colActual * TAM_CELDA,PY_TABLERO 
					                                    + filaActual * TAM_CELDA, TAM_CELDA);
    }
}

void Juego::dibujarPanel() {
    int px = PX_PANEL;

    dibujarTexto(to_string(puntaje), px, 70, 40, Color(255, 220, 60));

    dibujarTexto(to_string(nivel), px, 140, 34, Color(220, 120, 255));

    dibujarTexto(to_string(lineas), px, 200, 34, Color(255, 120, 120));

    int segundos = tiempoMs / 1000;
    string txtTiempo = to_string(segundos) + " s";
    dibujarTexto(txtTiempo, px, 260, 30, Color(120, 220, 255));

    int tipoHold = -1;
    if (!pilaHold.vacia()) {
        tipoHold = pilaHold.arriba();
    }
    dibujarPiezaEn(tipoHold, 0, PX_HOLD + 40, 105, TAM_HOLD);

    int yPrev = 285;
    int k;
    for (k = 0; k < 3; k++) {
        if (k < colaPiezas.tamano()) {
            int pieza = colaPiezas.proximo(k);
            dibujarPiezaEn(pieza, 0, PX_HOLD + 20, yPrev, TAM_PREVIEW);
        }
        yPrev = yPrev + 96;
    }
}

bool Juego::clicEnBotonPausa(int x, int y) const {
    return x >= BTN_PAUSA_X && x < BTN_PAUSA_X + BTN_PAUSA_ANCHO &&
           y >= BTN_PAUSA_Y && y < BTN_PAUSA_Y + BTN_PAUSA_ALTO;
}

void Juego::dibujarBotonPausa() const {
    RectangleShape boton(Vector2f((float)BTN_PAUSA_ANCHO,(float)BTN_PAUSA_ALTO));
    boton.setPosition((float)BTN_PAUSA_X, (float)BTN_PAUSA_Y);
    boton.setFillColor(Color(255, 255, 255, 0));
    boton.setOutlineColor(Color(180, 180, 190, 0));
    boton.setOutlineThickness(2.0f);
    ventana.draw(boton);
}

void Juego::dibujarPausa() {
}

void Juego::dibujarReplayOverlay() {
    int pasoActual = historial.ordenActual() + 1;
    int total = historial.tamano();
    string txt = to_string(pasoActual)+ " de "+ to_string(total);
    Text t(txt, fuente, 25);
    FloatRect b = t.getLocalBounds();
    dibujarTexto(txt, PX_HOLD + 45 - (int)(b.width / 2), 570, 20,Color(255, 220, 60));
}

void Juego::dibujarMenu() {
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

        RectangleShape boton(Vector2f((float)MENU_BTN_ANCHO,(float)MENU_BTN_ALTO));
        boton.setPosition((float)MENU_BTN_X, (float)y);
        boton.setFillColor(colorBoton);
        if (i == seleccionMenu) {
            boton.setOutlineColor(Color(255, 255, 255, 200));
            boton.setOutlineThickness(3.0f);
        }
        ventana.draw(boton);

        RectangleShape brillo(Vector2f((float)MENU_BTN_ANCHO,(float)MENU_BTN_ALTO));
        brillo.setPosition((float)MENU_BTN_X, (float)y);
        brillo.setFillColor(Color(255, 255, 255, 28));
        ventana.draw(brillo);

        Text t(opciones[i], fuente, 26);
        FloatRect b = t.getLocalBounds();
        dibujarTexto(opciones[i],MENU_BTN_X + (MENU_BTN_ANCHO - (int)b.width) / 2,
		             y + 8, 26, Color(255, 255, 255));

        y = y + MENU_BTN_SEPARACION;
    }
}

void Juego::dibujarGameOver() {
    Text t;
    t.setFont(fuente);
    t.setString(nombreJugador + "_");
    t.setCharacterSize(40);
    t.setFillColor(Color(120, 220, 255));
    FloatRect b = t.getLocalBounds();
    t.setPosition(490.0f - b.width / 2.0f, 342.0f);
    ventana.draw(t);
}

void Juego::dibujarRanking() {
    string indicador = "Insercion";
    if (algoritmoRanking == 1) {
        indicador = "Quicksort";
    }
    dibujarTexto(indicador, 450, 100, 22, Color(255, 220, 60));

    int y = 200;
    int n = ranking.getCantidad();
    if (n == 0) {
        dibujarTexto("(sin registros)", 420, y, 22, Color(150, 150, 150));
        return;
    }

    int i;
    for (i = 0; i < n; i++) {
        if (i < 10) {
            Registro r = ranking.obtener(i);
            string linea = to_string(i + 1) + ". " + r.nombre + "  " + to_string(r.puntos);
            dibujarTexto(linea, 420, y, 22, Color(220, 220, 220));
            y = y + 30;
        }
    }
}

void Juego::dibujarProximamente(const string& titulo) {
    dibujarTexto(titulo, 360, 200, 70, Color(255, 220, 60));
    dibujarTexto("Proximamente...",380, 300, 30, Color(200, 200, 200));
    dibujarTexto("Enter/Esc: volver al menu",350, 400, 22, Color(140, 140, 150));
}
