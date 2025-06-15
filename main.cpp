#include "raylib.h"
#include "juegoUNO.h"
using namespace std;

int main()
{

    // aqui va lo de raylib

    InitWindow(1920, 1080, " UNO con minijuegos");
    SetTargetFPS(60);

    Juego_UNO juego;
    juego.estadoDeJuego = esperando_jugadores;
    juego.turno_actual = 0;
    juego.direccion = 1;

    bool cantidadSeleccionada = false;
    int jugadorActual = 0;
    string entradaActual = "";
    bool nombresCompletos = false;

    inicializarMazo(juego);
    barajarMazo(juego);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (!cantidadSeleccionada)
        {
            seleccionarCatidadJugadores(juego, cantidadSeleccionada);
        }

        else if (!nombresCompletos)
        {
            capturarNombresEnLaVentana(juego, jugadorActual, entradaActual, nombresCompletos);
        }

        else
        {

            if (juego.estadoDeJuego == esperando_jugadores)
            {
                repartirCartas(juego);
                juego.estadoDeJuego = turno_normal;
            }

            for (int i = 0; i < juego.cantidadJugadores; i++)
            {
                int y = 100 + i * 150;
                dibujarCartasJugador(juego.jugadores[i], 100, y, true);
            }

            DrawRectangle(1000, 300, 80, 120, DARKGRAY); // esto es el mazo
            DrawText("MAZO", 1010, 360, 20, WHITE);
        }


        EndDrawing();
    }

    CloseWindow();
    return 0;
}