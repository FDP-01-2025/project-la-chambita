#include "raylib.h"
#include "juegoUNO.h"
#include "minijuego_Palabra.h"
using namespace std;

int main()
{
    // aqui va lo de raylib
    //dimesiones para joel 1366x768
    //dimesiones para aaron pantalla 1920x1080
    InitWindow(1920, 1080, " UNO con minijuegos");
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