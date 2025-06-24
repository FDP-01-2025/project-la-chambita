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
     //  Aquí se ejecuta todo el ciclo del juego
    ejecutarJuego(juego, cantidadSeleccionada, jugadorActual, entradaActual, nombresCompletos);

     // ✅ Una vez que el juego termina, se actualizan estadísticas
    if (juego.estadoDeJuego == juego_terminado)
    {
        EstadisticasJugador stats;
        bool jugadorGano = true; // cambiar por lógica real si la tienes
        int minijuegosEstaPartida = 2; // o contador real
        actualizarEstadisticas(stats, jugadorGano, minijuegosEstaPartida);
    }
    
    CloseWindow();
    return 0;
}