#ifndef SPEED_MINIGAME_H
#define SPEED_MINIGAME_H

#include "raylib.h"
#include "UNO_game.h"

// Inicia el minijuego de velocidad (reinicia estado)
void startSpeedMinigame();

// Actualiza lógica y dibuja todo el minijuego (se llama en cada frame)
void updateSpeedMinigame(Player &player);

// Devuelve true si el minijuego ya terminó (gane o pierda)
bool speedMinigameFinished();

// Devuelve true si el jugador ganó (presionó la tecla a tiempo)
bool speedMinigameWon();

#endif 