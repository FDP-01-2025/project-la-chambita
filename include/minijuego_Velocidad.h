#ifndef MINIJUEGO_VELOCIDAD_H
#define MINIJUEGO_VELOCIDAD_H

#include "raylib.h"
#include "juegoUNO.h"

// Inicia el minijuego de velocidad (reinicia estado)
void iniciarMinijuegoVelocidad();

// Actualiza lógica y dibuja todo el minijuego (se llama en cada frame)
void actualizarMinijuegoVelocidad(Jugador &jugador);

// Devuelve true si el minijuego ya terminó (gane o pierda)
bool minijuegoVelocidadTerminado();

// Devuelve true si el jugador ganó (presionó la tecla a tiempo)
bool minijuegoVelocidadGano();

#endif 