#ifndef MINIJUEGO_INTRUSO_H
#define MINIJUEGO_INTRUSO_H

#include "raylib.h"
#include "juegoUNO.h"

// Estructura para elementos del juego
struct Element {
    std::string value;
    std::string type;  // "number", "fruit", "color", "animal"
};

// Inicia el minijuego (genera elementos, mezcla, reinicia estado)
void iniciarMinijuegoIntruso();

// Actualiza lógica y dibuja todo el minijuego (se llama en cada frame)
void actualizarMinijuegoIntruso(Jugador &jugador);

// Devuelve true si el minijuego ya terminó (gane o pierda)
bool minijuegoIntrusoTerminado();

// Devuelve true si el jugador ganó (encontró el intruso)
bool minijuegoIntrusoGano();

#endif 