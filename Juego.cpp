#include "Juego.h"

#include <string>

using namespace sf;

using namespace std;

Juego::Juego() {
    tablero.limpiar();
    colaPiezas = ColaPiezas();
    colaPiezas.rellenarSiFalta();
    estado = MENU;
    replayDesdeGameOver = false;
    tipoActual = -1;
    rotActual = 0;
    filaActual = 0;
    colActual = 0;
    puntaje = 0;
    nivel = 1;
    lineas = 0;
    tiempoMs = 0;
    velocidadMs = VELOCIDAD_INICIAL_MS;
    mensajeEvento = "";
    tiempoEventoMs = 0;
    holdUsado = false;
    nombreJugador = "";
    nFilasBorrar = 0;
    parpadeoMs = 0;
    algoritmoRanking = 0;
    colaEventos.limpiar();
    relojGravedad.restart();
    relojMarco.restart();
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
    velocidadMs = VELOCIDAD_INICIAL_MS;
    mensajeEvento = "";
    tiempoEventoMs = 0;
    holdUsado = false;
    nombreJugador = "";
    nFilasBorrar = 0;
    relojMarco.restart();
    colaEventos.limpiar();
    colaEventos.insertar(Evento(15000, EVENTO_ACELERAR));
    colaEventos.insertar(Evento(20000, EVENTO_BONUS_TIEMPO));
    colaEventos.insertar(Evento(25000, EVENTO_BONUS_PUNTOS));
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

    registrarMovimiento();
}

void Juego::registrarMovimiento() {
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
        registrarMovimiento();
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
    registrarMovimiento();
    bloquearPieza();
}

void Juego::bloquearPieza() {
    if (estado != JUGANDO) {
        return;
    }
    tablero.ponerFicha(tipoActual, rotActual, filaActual, colActual);

    int completas[20];
    int n = tablero.filasCompletas(completas);
    if (n > 0) {
        int i;
        for (i = 0; i < n; i++) {
            filasBorrar[i] = completas[i];
        }
        nFilasBorrar = n;
        parpadeoMs = 0;
        registrarMovimiento();
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

void Juego::manejarEventosCola() {
    bool hayEventos = !colaEventos.empty();
    while (hayEventos) {
        if (colaEventos.top().momento > tiempoMs) {
            hayEventos = false;
        } else {
            Evento e = colaEventos.pop();
            if (e.tipo == EVENTO_ACELERAR) {
                velocidadMs = velocidadMs * 60 / 100;
                if (velocidadMs < 60) {
                    velocidadMs = 60;
                }
                mensajeEvento = "ACELERAR: caida mas rapida";
                tiempoEventoMs = 3000;
                int proximoMomento = e.momento + 15000;
                colaEventos.insertar(Evento(proximoMomento, EVENTO_ACELERAR));
            } else if (e.tipo == EVENTO_BONUS_TIEMPO) {
                puntaje = puntaje + 50;
                mensajeEvento = "BONUS TIEMPO: +50 puntos";
                tiempoEventoMs = 3000;
                int proximoMomento = e.momento + 20000;
                colaEventos.insertar(Evento(proximoMomento, EVENTO_BONUS_TIEMPO));
            } else if (e.tipo == EVENTO_BONUS_PUNTOS) {
                puntaje = puntaje + 100;
                mensajeEvento = "BONUS PUNTOS: +100 puntos";
                tiempoEventoMs = 3000;
                int proximoMomento = e.momento + 25000;
                colaEventos.insertar(Evento(proximoMomento, EVENTO_BONUS_PUNTOS));
            }
            hayEventos = !colaEventos.empty();
        }
    }
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
        registrarMovimiento();

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

void Juego::actualizar() {
    Time dt = relojMarco.restart();
    int ms = dt.asMilliseconds();

    if (estado == JUGANDO) {
        tiempoMs = tiempoMs + ms;
        manejarEventosCola();
        if (tiempoEventoMs > 0) {
            tiempoEventoMs = tiempoEventoMs - ms;
        }

        if (nFilasBorrar > 0) {
            parpadeoMs = parpadeoMs + ms;
            if (parpadeoMs >= 400) {
                lineas = lineas + nFilasBorrar;
                puntaje = puntaje + puntajePorLineas(nFilasBorrar) * nivel;
                nivel = lineas / 10 + 1;
                velocidadMs = VELOCIDAD_INICIAL_MS - (nivel - 1) * 70;
                if (velocidadMs < 80) {
                    velocidadMs = 80;
                }
                tablero.borrarFilas(filasBorrar, nFilasBorrar);
                nFilasBorrar = 0;
                parpadeoMs = 0;
                relojGravedad.restart();
                generarPieza();
            }
            return;
        }

        int msGravedad = relojGravedad.getElapsedTime().asMilliseconds();
        if (msGravedad >= velocidadMs) {
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

// ---------- Comandos de navegacion de estados ----------
void Juego::iniciarNuevoJuego() {
    reiniciarPartida();
    estado = JUGANDO;
    generarPieza();
}

void Juego::irARanking() {
    estado = VER_RANKING;
}

void Juego::pausar() {
    relojGravedad.restart();
    relojMarco.restart();
    estado = PAUSA;
}

void Juego::reanudar() {
    relojGravedad.restart();
    relojMarco.restart();
    estado = JUGANDO;
}

void Juego::volverAlMenu() {
    estado = MENU;
}

void Juego::volverDeReplay() {
    if (replayDesdeGameOver) {
        estado = GAME_OVER;
    } else {
        estado = MENU;
    }
}

void Juego::setEstado(EstadoJuego e) {
    estado = e;
}

void Juego::elegirAlgoritmoRanking(int alg) {
    algoritmoRanking = alg;
    ranking.elegirAlgoritmo(algoritmoRanking);
    ranking.reordenar();
}

void Juego::pasoReplayAtras() {
    bool ok = historial.deshacer();
    if (ok) {
        restaurarEstado(historial.obtenerActual());
        relojReplay.restart();
    }
}

void Juego::pasoReplayAdelante() {
    bool ok = historial.avanzar();
    if (ok) {
        restaurarEstado(historial.obtenerActual());
        relojReplay.restart();
    }
}

// ---------- Comandos de juego ----------
void Juego::moverIzquierda() {
    if (piezaPuede(filaActual, colActual - 1, rotActual)) {
        colActual = colActual - 1;
        registrarMovimiento();
    }
}

void Juego::moverDerecha() {
    if (piezaPuede(filaActual, colActual + 1, rotActual)) {
        colActual = colActual + 1;
        registrarMovimiento();
    }
}

void Juego::rotar() {
    int nuevaRot = (rotActual + 1) % 4;
    if (piezaPuede(filaActual, colActual, nuevaRot)) {
        rotActual = nuevaRot;
        registrarMovimiento();
    }
}

void Juego::bajarSuave() {
    if (piezaPuede(filaActual + 1, colActual, rotActual)) {
        filaActual = filaActual + 1;
        puntaje = puntaje + 1;
        registrarMovimiento();
    } else {
        bloquearPieza();
    }
    relojGravedad.restart();
}

void Juego::agregarLetra(char c) {
    if (nombreJugador.size() < 10) {
        nombreJugador = nombreJugador + c;
    }
}

void Juego::borrarLetra() {
    if (nombreJugador.size() > 0) {
        nombreJugador.erase(nombreJugador.size() - 1);
    }
}

// ---------- Getters para la Vista ----------
EstadoJuego Juego::getEstado() const { return estado; }
int Juego::getPuntaje() const { return puntaje; }
int Juego::getNivel() const { return nivel; }
int Juego::getLineas() const { return lineas; }
int Juego::getTiempoMs() const { return tiempoMs; }
int Juego::getVelocidadMs() const { return velocidadMs; }
string Juego::getMensajeEvento() const { return mensajeEvento; }
int Juego::getTiempoEventoMs() const { return tiempoEventoMs; }
string Juego::getNombreJugador() const { return nombreJugador; }
int Juego::getTipoActual() const { return tipoActual; }
int Juego::getRotActual() const { return rotActual; }
int Juego::getFilaActual() const { return filaActual; }
int Juego::getColActual() const { return colActual; }

int Juego::getTipoHold() const {
    if (pilaHold.vacia()) {
        return -1;
    }
    return pilaHold.arriba();
}

int Juego::getCeldaTablero(int f, int c) const {
    return tablero.getCelda(f, c);
}

int Juego::getProximaPieza(int k) const {
    return colaPiezas.proximo(k);
}

int Juego::getCantidadProximas() const {
    return colaPiezas.tamano();
}

int Juego::getNFilasBorrar() const { return nFilasBorrar; }
int Juego::getFilaBorrar(int i) const { return filasBorrar[i]; }
int Juego::getParpadeoMs() const { return parpadeoMs; }
bool Juego::getReplayDesdeGameOver() const { return replayDesdeGameOver; }

int Juego::getPasoReplay() const {
    return historial.ordenActual() + 1;
}

int Juego::getTotalPasos() const {
    return historial.tamano();
}

int Juego::getAlgoritmoRanking() const { return algoritmoRanking; }

int Juego::getCantidadRanking() const {
    return ranking.getCantidad();
}

Registro Juego::getRegistroRanking(int i) const {
    return ranking.obtener(i);
}