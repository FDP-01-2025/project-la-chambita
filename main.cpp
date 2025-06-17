#include "raylib.h"
#include "juegoUNO.h"

using namespace std;

int main()
{

    // aqui va lo de raylib

    InitWindow(1080, 720, " UNO con minijuegos");
    SetTargetFPS(30);

    Juego_UNO juego = crearJuegoUNO();

    bool cantidadSeleccionada;
    int jugadorActual;
    string entradaActual;
    bool nombresCompletos;

    iniciarVariablesEstado(cantidadSeleccionada, jugadorActual, entradaActual, nombresCompletos);

    inicializarMazo(juego);
    barajarMazo(juego);

    ejecutarJuego(juego, cantidadSeleccionada, jugadorActual, entradaActual, nombresCompletos);
    return 0;
}