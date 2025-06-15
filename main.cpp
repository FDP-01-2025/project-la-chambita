#include "raylib.h"
#include "juegoUNO.h"
#include "minijuegos.h"
using namespace std;


int main(){

//esto crea la instancia del juego uno
Juego_UNO juego;

//inicia el estado de partida antes de comenzar
juego.estadoDeJuego = esperando_jugadores;

// inicia el indice de los turnos 
juego.turno_actual = 0;

// inicia la direccion del juego (1 porque asi empieza, hasta que se tire un comodin de direccion)
juego.direccion = 1;

// aqui se guarda la cantidad de jugadores
int cantidad = 0;

// registro de nombres para cada jugador
ingresarNombres(juego);

// crea y llena el mazo principal de donde se agarraran cartas
inicializarMazo(juego);

// lo barajea para que el orden sea random
barajarMazo(juego);

// le damos 7 cartas a cada jugador
repartirCartas(juego);



//aqui va lo de raylib

InitWindow (1920, 1080, " UNO con minijuegos");
SetTargetFPS (144);

while(!WindowShouldClose()){
    BeginDrawing();
    
    ClearBackground (RAYWHITE);

    dibujarCartasJugador(juego.jugadores[0], 100, 100, true);

if (cantidad == 2)
    dibujarCartasJugador(juego.jugadores[1], 100, 300, false);

    DrawRectangle(800, 400, 80, 120, DARKGRAY); // mazo
DrawText("MAZO", 810, 460, 20, WHITE);


    EndDrawing();

}

CloseWindow();
    return 0;
}