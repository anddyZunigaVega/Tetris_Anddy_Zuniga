#include "GestorUI.h"

#include <iostream>

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

GestorUI::GestorUI(RenderWindow& ventana, const Font& fuente)
    : ventana(ventana), fuente(fuente) {
    seleccionMenu = 0;
}

void GestorUI::inicializarRecursos(const Font& fuente) {
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

void GestorUI::procesarEventos(sf::Event& evento, Juego& contexto) {
    if (evento.type == Event::MouseMoved) {
        if (contexto.getEstado() == MENU) {
            int i = opcionEn(evento.mouseMove.x, evento.mouseMove.y);
            if (i >= 0) {
                seleccionMenu = i;
            }
        }
        return;
    }

    if (evento.type == Event::MouseButtonPressed) {
        if (evento.mouseButton.button == Mouse::Left) {
            if (contexto.getEstado() == JUGANDO) {
                if (clicEnBotonPausa(evento.mouseButton.x, evento.mouseButton.y)) {
                    contexto.pausar();
                }
            }
            if (contexto.getEstado() == MENU) {
                int i = opcionEn(evento.mouseButton.x, evento.mouseButton.y);
                if (i >= 0) {
                    seleccionMenu = i;
                    ejecutarOpcion(contexto, i);
                }
            }
        }
        return;
    }

    if (evento.type == Event::TextEntered) {
        if (contexto.getEstado() == GAME_OVER) {
            if (evento.text.unicode == 8) {
                contexto.borrarLetra();
            } else if (evento.text.unicode == 13) {
                contexto.guardarRanking();
            } else if (evento.text.unicode >= 32 && evento.text.unicode < 127) {
                contexto.agregarLetra((char)evento.text.unicode);
            }
        }
        return;
    }

    if (evento.type != Event::KeyPressed) {
        return;
    }
    Keyboard::Key tecla = evento.key.code;

    if (contexto.getEstado() == MENU) {
        if (tecla == Keyboard::Escape) {
            ventana.close();
        }
    } else if (contexto.getEstado() == JUGANDO) {
        if (contexto.getNFilasBorrar() > 0
            && tecla != Keyboard::P && tecla != Keyboard::Escape) {
            return;
        }
        if (tecla == Keyboard::Left) {
            contexto.moverIzquierda();
        } else if (tecla == Keyboard::Right) {
            contexto.moverDerecha();
        } else if (tecla == Keyboard::Up) {
            contexto.rotar();
        } else if (tecla == Keyboard::Down) {
            contexto.bajarSuave();
        } else if (tecla == Keyboard::Space) {
            contexto.caerInstantaneo();
} else if (tecla == Keyboard::C) {
            contexto.usarHold();
        } else if (tecla == Keyboard::P || tecla == Keyboard::Escape) {
            contexto.pausar();
        }
    } else if (contexto.getEstado() == PAUSA) {
        if (tecla == Keyboard::P) {
            contexto.reanudar();
        } else if (tecla == Keyboard::R) {
            if (contexto.getTotalPasos() > 0) {
                contexto.iniciarReplay();
            }
        } else if (tecla == Keyboard::Escape) {
            contexto.volverAlMenu();
        }
    } else if (contexto.getEstado() == GAME_OVER) {
        if (tecla == Keyboard::R) {
            if (contexto.getTotalPasos() > 0) {
                contexto.iniciarReplay();
            }
        } else if (tecla == Keyboard::Escape) {
            contexto.volverAlMenu();
        }
    } else if (contexto.getEstado() == REPLAY) {
        if (tecla == Keyboard::Left || tecla == Keyboard::A) {
            contexto.pasoReplayAtras();
        } else if (tecla == Keyboard::Right || tecla == Keyboard::D) {
            contexto.pasoReplayAdelante();
        } else if (tecla == Keyboard::Return || tecla == Keyboard::Escape) {
            contexto.volverDeReplay();
        }
    } else if (contexto.getEstado() == VER_RANKING) {
        if (tecla == Keyboard::Num1) {
            contexto.elegirAlgoritmoRanking(0);
        } else if (tecla == Keyboard::Num2) {
            contexto.elegirAlgoritmoRanking(1);
        } else if (tecla == Keyboard::Return || tecla == Keyboard::Escape) {
            contexto.volverAlMenu();
        }
    }
}

void GestorUI::dibujar(RenderWindow& ventana, Juego& contexto) {
    ventana.clear(Color(40, 40, 45));
    EstadoJuego estado = contexto.getEstado();
    switch (estado) {
        case MENU:
            ventana.draw(fondoMenu);
            dibujarMenu(ventana, contexto);
            break;
        case PAUSA:
            ventana.draw(fondoPausa);
            break;
        case GAME_OVER:
            ventana.draw(fondoGameOver);
            dibujarGameOver(ventana, contexto);
            break;
        case VER_RANKING:
            ventana.draw(fondoRanking);
            dibujarRanking(ventana, contexto);
            break;
        case REPLAY:
            ventana.draw(fondoReplay);
            dibujarTablero(ventana, contexto);
            dibujarPanel(ventana, contexto);
            dibujarReplayOverlay(ventana, contexto);
            break;
        case JUGANDO:
            ventana.draw(fondoJuego);
            dibujarTablero(ventana, contexto);
            dibujarPanel(ventana, contexto);
            dibujarBotonPausa(ventana);
            break;
    }
    ventana.display();
}

void GestorUI::dibujarTexto(RenderWindow& ventana, const string& s, int x, int y, int tam, const Color& color) const {
    Text t;
    t.setFont(fuente);
    t.setString(s);
    t.setCharacterSize(tam);
    t.setFillColor(color);
    t.setPosition((float)x, (float)y);
    ventana.draw(t);
}

Color GestorUI::colorDeTipo(int tipo) const {
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

void GestorUI::dibujarPiezaEn(RenderWindow& ventana, int tipo, int rot, int px,
                              int py, int tam) const {
    if (tipo < 0) {
        return;
    }
    Color color = colorDeTipo(tipo);
    int n = numCeldas(tipo);
    RectangleShape celda;
    for (int k = 0; k < n; k++) {
        int df = CELDAS[tipo][rot][k][0];
        int dc = CELDAS[tipo][rot][k][1];
        celda.setSize(Vector2f((float)(tam - 2), (float)(tam - 2)));
        celda.setFillColor(color);
        celda.setPosition((float)(px + dc * tam), (float)(py + df * tam));
        ventana.draw(celda);
    }
}

void GestorUI::dibujarTablero(RenderWindow& ventana, const Juego& juego) const {
    RectangleShape celda;
    for (int f = 0; f < FILAS_TABLERO; f++) {
        for (int c = 0; c < COLS_TABLERO; c++) {
            int v = juego.getCeldaTablero(f, c);
            if (v == 0) {
                continue;
            }
            celda.setSize(Vector2f((float)(TAM_CELDA - 2), (float)(TAM_CELDA - 2)));
            Color colorCelda = colorDeTipo(v - 1);
            bool parpadea = false;
            for (int b = 0; b < juego.getNFilasBorrar(); b++) {
                if (juego.getFilaBorrar(b) == f) {
                    parpadea = true;
                }
            }
            if (parpadea && (juego.getParpadeoMs() / 90) % 2 == 0) {
                colorCelda = Color(255, 255, 255);
            }
            celda.setFillColor(colorCelda);
            celda.setPosition((float)(PX_TABLERO + c * TAM_CELDA + 1), (float)(PY_TABLERO + f * TAM_CELDA + 1));
            ventana.draw(celda);
        }
    }

    if (juego.getTipoActual() >= 0 && juego.getNFilasBorrar() == 0) {
        dibujarPiezaEn(ventana, juego.getTipoActual(), juego.getRotActual(), PX_TABLERO + juego.getColActual() * TAM_CELDA,
                       PY_TABLERO + juego.getFilaActual() * TAM_CELDA, TAM_CELDA);
    }
}

void GestorUI::dibujarPanel(RenderWindow& ventana, const Juego& juego) const {
    int px = PX_PANEL;

    dibujarTexto(ventana, to_string(juego.getPuntaje()), px, 70, 40, Color(255, 220, 60));
    dibujarTexto(ventana, to_string(juego.getNivel()), px, 140, 34, Color(220, 120, 255));
    dibujarTexto(ventana, to_string(juego.getLineas()), px, 200, 34, Color(255, 120, 120));

    int segundos = juego.getTiempoMs() / 1000;
    string txtTiempo = to_string(segundos) + " s";
    dibujarTexto(ventana, txtTiempo, px, 260, 30, Color(120, 220, 255));

    dibujarPiezaEn(ventana, juego.getTipoHold(), 0, PX_HOLD + 40, 105, TAM_HOLD);

    int yPrev = 285;

    for (int k = 0; k < 3; k++) {
        if (k < juego.getCantidadProximas()) {
            dibujarPiezaEn(ventana, juego.getProximaPieza(k), 0,
                           PX_HOLD + 20, yPrev, TAM_PREVIEW);
        }
        yPrev = yPrev + 96;
    }

    if (juego.getTiempoEventoMs() > 0 && !juego.getMensajeEvento().empty()) {
        dibujarTexto(ventana, juego.getMensajeEvento(), px, 470, 18, Color(120, 255, 160));
    }
}

void GestorUI::dibujarBotonPausa(RenderWindow& ventana) const {
    RectangleShape boton(Vector2f((float)BTN_PAUSA_ANCHO, (float)BTN_PAUSA_ALTO));
    boton.setPosition((float)BTN_PAUSA_X, (float)BTN_PAUSA_Y);
    boton.setFillColor(Color(255, 255, 255, 0));
    boton.setOutlineColor(Color(180, 180, 190, 0));
    boton.setOutlineThickness(2.0f);
    ventana.draw(boton);
}

void GestorUI::dibujarReplayOverlay(RenderWindow& ventana, const Juego& juego) const {
    int pasoActual = juego.getPasoReplay();
    int total = juego.getTotalPasos();
    string txt = to_string(pasoActual) + " de " + to_string(total);
    Text t(txt, fuente, 25);
    FloatRect b = t.getLocalBounds();
    dibujarTexto(ventana, txt, PX_HOLD + 45 - (int)(b.width / 2), 570, 20, Color(255, 220, 60));
}

void GestorUI::dibujarMenu(RenderWindow& ventana, const Juego& juego) const {
    const char* opciones[3];
    opciones[0] = "Jugar";
    opciones[1] = "Ver ranking";
    opciones[2] = "Salir";

    int y = MENU_BTN_Y;
    for (int i = 0; i < 3; i++) {
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

        RectangleShape boton(Vector2f((float)MENU_BTN_ANCHO, (float)MENU_BTN_ALTO));
        boton.setPosition((float)MENU_BTN_X, (float)y);
        boton.setFillColor(colorBoton);
        if (i == seleccionMenu) {
            boton.setOutlineColor(Color(255, 255, 255, 200));
            boton.setOutlineThickness(3.0f);
        }
        ventana.draw(boton);

        RectangleShape brillo(Vector2f((float)MENU_BTN_ANCHO, (float)MENU_BTN_ALTO));
        brillo.setPosition((float)MENU_BTN_X, (float)y);
        brillo.setFillColor(Color(255, 255, 255, 28));
        ventana.draw(brillo);

        Text t(opciones[i], fuente, 26);
        FloatRect b = t.getLocalBounds();
        dibujarTexto(ventana, opciones[i], MENU_BTN_X + (MENU_BTN_ANCHO - (int)b.width) / 2,
                     y + 8, 26, Color(255, 255, 255));

        y = y + MENU_BTN_SEPARACION;
    }
}

void GestorUI::dibujarGameOver(RenderWindow& ventana, const Juego& juego) const {
    Text t;
    t.setFont(fuente);
    t.setString(juego.getNombreJugador() + "_");
    t.setCharacterSize(40);
    t.setFillColor(Color(120, 220, 255));
    FloatRect b = t.getLocalBounds();
    t.setPosition(490.0f - b.width / 2.0f, 342.0f);
    ventana.draw(t);
}

void GestorUI::dibujarRanking(RenderWindow& ventana, const Juego& juego) const {
    string indicador = "Insercion";
    if (juego.getAlgoritmoRanking() == 1) {
        indicador = "Quicksort";
    }
    dibujarTexto(ventana, indicador, 450, 100, 22, Color(255, 220, 60));

    int y = 200;
    int n = juego.getCantidadRanking();
    if (n == 0) {
        dibujarTexto(ventana, "(sin registros)", 420, y, 22, Color(150, 150, 150));
        return;
    }

    for (int i = 0; i < n && i < 10; i++) {
        Registro r = juego.getRegistroRanking(i);
        string linea = to_string(i + 1) + ". " + r.nombre + "  " + to_string(r.puntos);
        dibujarTexto(ventana, linea, 420, y, 22, Color(220, 220, 220));
        y = y + 30;
    }
}

bool GestorUI::clicEnBotonPausa(int x, int y) const {
    return x >= BTN_PAUSA_X && x < BTN_PAUSA_X + BTN_PAUSA_ANCHO && y >= BTN_PAUSA_Y && y < BTN_PAUSA_Y + BTN_PAUSA_ALTO;
}

int GestorUI::opcionEn(int x, int y) const {
    int resultado = -1;
    for (int i = 0; i < 3; i++) {
        int px = MENU_BTN_X;
        int py = MENU_BTN_Y + i * MENU_BTN_SEPARACION;
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

void GestorUI::ejecutarOpcion(Juego& juego, int opcion) {
    if (opcion == 0) {
        juego.iniciarNuevoJuego();
    } else if (opcion == 1) {
        juego.irARanking();
    } else {
        ventana.close();
    }
}
