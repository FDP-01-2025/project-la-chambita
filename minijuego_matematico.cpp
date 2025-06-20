#include "raylib.h"
#include <fstream>
#include <ctime>
#include <cstring>
#include <cctype>

// Estructura para una pregunta con opciones
struct Pregunta {
    const char* texto;
    const char* opciones[4];
    char respuestaCorrecta;
};

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

bool jugarMinijuegoMatematico() {
    const int ancho = 800, alto = 450;
    InitWindow(ancho, alto, "Minijuego: Ejercicio Matematico");
    SetTargetFPS(60);

    Texture2D fondo = LoadTexture("imagenMate.jpg");

    // Lista de preguntas disponibles
    Pregunta preguntas[] = {
        {"6 ÷6 \xF7 2 (1 + 2)", {"a) 1", "b) 9", "c) 6", "d) 0"}, 'b'},
        {"5 + 3 * 2",        {"a) 16", "b) 11", "c) 10", "d) 8"}, 'b'},
        {"(4 + 2) * 3",      {"a) 18", "b) 14", "c) 12", "d) 20"}, 'a'}
    };

    int totalPreguntas = sizeof(preguntas) / sizeof(Pregunta);
    srand(time(0));
    int indice = rand() % totalPreguntas;
    Pregunta preguntaActual = preguntas[indice];

    int intentos = 0;
    const int maxIntentos = 2;
    bool gano = false;
    bool juegoTerminado = false;
    char seleccionActual = '\0';

    while (!WindowShouldClose() && !juegoTerminado) {
        // Detectar entrada
        int key = GetKeyPressed();
        if (key != 0) {
            char tecla = (char)tolower(key);
            if (tecla >= 'a' && tecla <= 'd') {
                seleccionActual = tecla;
                intentos++;

                if (tecla == preguntaActual.respuestaCorrecta) {
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

        // Titulo y enunciado
        DrawText("Resuelve el ejercicio matematico:", 40, 70, 25, DARKBLUE);
        DrawText(preguntaActual.texto, ancho / 2 - MeasureText(preguntaActual.texto, 40) / 2, 120, 40, BLACK);

        // Mostrar opciones (2 columnas)
        for (int i = 0; i < 4; i++) {
            int x = (i % 2 == 0) ? 180 : 420;
            int y = 200 + (i / 2) * 80;
            Color colorFondo = (seleccionActual == ('a' + i)) ? YELLOW : LIGHTGRAY;
            DrawRectangle(x, y, 160, 50, colorFondo);
            DrawText(preguntaActual.opciones[i], x + 10, y + 15, 20, BLACK);
        }

        // Mostrar intentos
        DrawText(TextFormat("Intentos: %d / %d", intentos, maxIntentos), 600, 60, 20, MAROON);

        EndDrawing();
    }

    // Mostrar mensaje final por 5 segundos
    double tInicio = GetTime();
    while (!WindowShouldClose() && (GetTime() - tInicio < 5.0)) {
        BeginDrawing();
        DrawTexture(fondo, 0, 0, WHITE);
        ClearBackground(RAYWHITE);

        if (gano) {
            DrawText("\xA1Correcto! Has ganado.", ancho / 2 - MeasureText("\xA1Correcto! Has ganado.", 30) / 2, 200, 30, GREEN);
        } else {
            DrawText("Perdiste. La respuesta correcta era: ", 180, 200, 25, RED);
            DrawText(TextFormat("%c", preguntaActual.respuestaCorrecta), 530, 200, 25, DARKGRAY);
        }

        EndDrawing();
    }

    UnloadTexture(fondo);
    CloseWindow();
    return gano;
}
