#include "raylib.h"
#include "juegoUNO.h"
#include "minijuego_Palabra.h"
using namespace std;

int main()
{

    // aqui va lo de raylib

    InitWindow(1366, 768, " UNO con minijuegos");
    SetTargetFPS(30);

    Juego_UNO juego = crearJuegoUNO();

    bool cantidadSeleccionada;
    int jugadorActual;
    string entradaActual;
    bool nombresCompletos;

    iniciarVariablesEstado(cantidadSeleccionada, jugadorActual, entradaActual, nombresCompletos);
    inicializarMazo(juego);
    barajarMazo(juego);

    //  Aquí se ejecuta todo el ciclo del juego
    ejecutarJuego(juego, cantidadSeleccionada, jugadorActual, entradaActual, nombresCompletos);

    CloseWindow();

    EstadisticasJugador stats;

    // Simulación al terminar una partida
    bool jugadorGano = true;
    int minijuegosEstaPartida = 2;

    return 0;
}