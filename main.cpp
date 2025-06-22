#include "raylib.h"
#include "juegoUNO.h"

using namespace std;

int main()
{

    // aqui va lo de raylib

    InitWindow(1920,1080, " UNO con minijuegos");
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

    CloseWindow();

    EstadisticasJugador stats;

// Simulación al terminar una partida
bool jugadorGano = true;
int minijuegosEstaPartida = 2;

actualizarEstadisticas(stats, jugadorGano, minijuegosEstaPartida);

MensajeTemporal mensaje; // global o local

// Al cambiar de turno
ActivarMensaje(mensaje, "¡Turno del Jugador 2!", 2.5f);

// En el bucle principal
float deltaTime = GetFrameTime();
BeginDrawing();
ClearBackground(BLACK);
DibujarMensaje(mensaje, deltaTime);
EndDrawing();


    return 0;
}