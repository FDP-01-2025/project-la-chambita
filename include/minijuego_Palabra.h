#ifndef WORD_MINIGAME_H
#define WORD_MINIGAME_H

#include "raylib.h"
#include "juegoUNO.h"

// Inicia el minijuego (elige palabra, mezcla, reinicia estado)
void startWordOrderMinigame();
// Actualiza lógica y dibuja todo el minijuego (se llama en cada frame)
void updateWordOrderMinigame(Player &player);

// Devuelve true si el minijuego ya terminó (gane o pierda)
bool wordOrderMinigameFinished();

// Devuelve true si el jugador ganó (ordenó correctamente la palabra)
bool wordOrderMinigameWon();

#endif
