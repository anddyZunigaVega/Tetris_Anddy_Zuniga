#ifndef GESTORUI_H
#define GESTORUI_H

#include <SFML/Graphics.hpp>
#include <string>

#include "Juego.h"

using namespace sf;
using namespace std;

// Version en avance: vista MVC. GestorUI concentra las imagenes, texturas,
// fondos, el procesamiento de eventos de pantalla (menus) y todo el dibujado.
// Se comunica con el modelo Juego a traves de Juego& contexto (comandos y
// getters), creando un puente bidireccional entre Vista y Modelo.
class GestorUI {
private:
    RenderWindow& ventana;
    const Font& fuente;

    Texture texturaJuego;
    Sprite fondoJuego;
    Texture texturaReplay;
    Sprite fondoReplay;
    Texture texturaMenu;
    Sprite fondoMenu;
    Texture texturaPausa;
    Sprite fondoPausa;
    Texture texturaGameOver;
    Sprite fondoGameOver;
    Texture texturaRanking;
    Sprite fondoRanking;

    int seleccionMenu;

    void dibujarTexto(RenderWindow& ventana, const string& s, int x, int y,
                      int tam, const Color& color) const;
    Color colorDeTipo(int tipo) const;
    void dibujarPiezaEn(RenderWindow& ventana, int tipo, int rot, int px,
                        int py, int tam) const;
    void dibujarTablero(RenderWindow& ventana, const Juego& juego) const;
    void dibujarPanel(RenderWindow& ventana, const Juego& juego) const;
    void dibujarBotonPausa(RenderWindow& ventana) const;
    void dibujarMenu(RenderWindow& ventana, const Juego& juego) const;
    void dibujarGameOver(RenderWindow& ventana, const Juego& juego) const;
    void dibujarReplayOverlay(RenderWindow& ventana, const Juego& juego) const;
    void dibujarRanking(RenderWindow& ventana, const Juego& juego) const;

    bool clicEnBotonPausa(int x, int y) const;
    int opcionEn(int x, int y) const;
    void ejecutarOpcion(Juego& juego, int opcion);

public:
    GestorUI(RenderWindow& ventana, const Font& fuente);
    void inicializarRecursos(const Font& fuente);

    void procesarEventos(sf::Event& evento, Juego& contexto);
    void dibujar(RenderWindow& ventana, Juego& contexto);
};

#endif