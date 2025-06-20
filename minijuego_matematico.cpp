#include "raylib.h"
#include <fstream>
#include <ctime>
#include <cstring>

// Guardar resultado en archivo
void guardarResultado(char opcion, bool gano) {
    std::ofstream archivo("puntaje_minijuego_mate.txt", std::ios::app);
    if (archivo.is_open()) {
        archivo << "Respuesta dada: " << opcion << "\n";
        archivo << "Resultado: " << (gano ? "GANASTE" : "PERDISTE") << "\n";
        archivo << "----------------------\n";
        archivo.close();
    }
}

// Devuelve true si gana, false si pierde
bool jugarMinijuegoMatematico() {
    const int ancho = 800, alto = 450;
    InitWindow(ancho, alto, "Minijuego: Ejercicio Matematico");
    SetTargetFPS(60);

    // Cargar fondo
    Texture2D fondo = LoadTexture("imagenMate.jpg");

    const char *pregunta = "6 ÷ 2 (1 + 2)";
    const char *opciones[4] = {"a) 1", "b) 9", "c) 6", "d) 0"};
    char respuestaCorrecta = 'b';
    int intentos = 0;
    const int maxIntentos = 2;
    bool gano = false;
    bool juegoTerminado = false;

    while (!WindowShouldClose() && !juegoTerminado) {
        // Detectar entrada de A–D
        int key = GetKeyPressed();
        if (key != 0) {
            char tecla = (char)tolower(key);
            if (tecla >= 'a' && tecla <= 'd') {
                intentos++;
                if (tecla == respuestaCorrecta) {
                    gano = true;
                    guardarResultado(tecla, true);
                    juegoTerminado = true;
                } else if (intentos >= maxIntentos) {
                    guardarResultado(tecla, false);
                    juegoTerminado = true;
                }
            }
        }

        BeginDrawing();
        DrawTexture(fondo, 0, 0, WHITE);
        ClearBackground(RAYWHITE);

        DrawText("Resuelve el ejercicio matematico:", 40, 30, 25, DARKBLUE);
        DrawText(pregunta, ancho / 2 - MeasureText(pregunta, 40) / 2, 80, 40, BLACK);

        for (int i = 0; i < 4; i++) {
            DrawRectangle(80, 150 + i * 60, 200, 40, LIGHTGRAY);
            DrawText(opciones[i], 90, 160 + i * 60, 20, BLACK);
        }

        DrawText(TextFormat("Intentos: %d / %d", intentos, maxIntentos), 600, 20, 20, MAROON);

        EndDrawing();
    }

    // Mostrar mensaje final por 5 segundos
    double tInicio = GetTime();
    while (!WindowShouldClose() && (GetTime() - tInicio < 5.0)) {
        BeginDrawing();
        DrawTexture(fondo, 0, 0, WHITE);
        ClearBackground(RAYWHITE);
        if (gano) {
            DrawText("¡Correcto! Has ganado.", ancho / 2 - MeasureText("¡Correcto! Has ganado.", 30) / 2, 200, 30, GREEN);
        } else {
            DrawText("Perdiste. La respuesta correcta era: b", 150, 200, 25, RED);
        }
        EndDrawing();
    }

    UnloadTexture(fondo);
    CloseWindow();
    return gano;
}
