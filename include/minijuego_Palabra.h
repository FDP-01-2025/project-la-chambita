#ifndef MINIJUEGO_PALABRA_H
#define MINIJUEGO_PALABRA_H

#include "raylib.h"
#include "juegoUNO.h"

// Inicia el minijuego (elige palabra, mezcla, reinicia estado)
void iniciarOrdenaPalabra();
// Actualiza lógica y dibuja todo el minijuego (se llama en cada frame)
void actualizarMinijuegoOrdenaPalabra(Jugador &jugador);

// Devuelve true si el minijuego ya terminó (gane o pierda)
bool minijuegoOrdenaTerminado();

// Devuelve true si el jugador ganó (ordenó correctamente la palabra)
bool minijuegoOrdenaGano();

#endif
