#ifndef INTRUDER_MINIGAME_H
#define INTRUDER_MINIGAME_H

#include "raylib.h"
#include "UNO_game.h"

// Estructura para elementos del juego
struct Element {
    std::string value;
    std::string type;  // "number", "fruit", "color", "animal"
};

// Inicia el minijuego (genera elementos, mezcla, reinicia estado)
void startIntruderMinigame();

// Actualiza lógica y dibuja todo el minijuego (se llama en cada frame)
void updateIntruderMinigame(Player &player);

// Devuelve true si el minijuego ya terminó (gane o pierda)
bool intruderMinigameFinished();

// Devuelve true si el jugador ganó (encontró el intruso)
bool intruderMinigameWon();

#endif 