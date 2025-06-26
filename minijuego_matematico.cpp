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

    Texture2D fondo = LoadTexture("fondo2.jpg");

    // Lista de preguntas disponibles
    Pregunta preguntas[] = {
        {"6 ÷6 x 2 (1 + 2)", {"a) 1", "b) 9", "c) 6", "d) 0"}, 'c'},
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
            ClearBackground(RAYWHITE);
            DrawTexture(fondo, 0, 0, WHITE);

            // Título centrado, más abajo
            const char* titulo = "Resuelve el ejercicio matematico:";
            int tituloFontSize = 30;
            int tituloY = 40;
            int tituloX = (ancho - MeasureText(titulo, tituloFontSize)) / 2;
            DrawText(titulo, tituloX, tituloY, tituloFontSize, DARKBLUE);

            // Pregunta centrada
            int preguntaFontSize = 40;
            int preguntaY = tituloY + tituloFontSize + 20;
            int preguntaX = (ancho - MeasureText(preguntaActual.texto, preguntaFontSize)) / 2;
            DrawText(preguntaActual.texto, preguntaX, preguntaY, preguntaFontSize, BLACK);

            // Opciones en dos columnas con buen espacio
            int opcionFontSize = 22;
            int espacioEntreFilas = 70;
            int margenIzquierdoCol1 = 150;
            int margenIzquierdoCol2 = 450;
            int primerFilaY = preguntaY + preguntaFontSize + 40;

            for (int i = 0; i < 4; i++) {
                int colX = (i % 2 == 0) ? margenIzquierdoCol1 : margenIzquierdoCol2;
                int filaY = primerFilaY + (i / 2) * espacioEntreFilas;

                int rectWidth = 180;
                int rectHeight = 55;
                Color colorFondo = (seleccionActual == ('a' + i)) ? YELLOW : LIGHTGRAY;
                DrawRectangle(colX, filaY, rectWidth, rectHeight, colorFondo);

                int textoX = colX + 15;
                int textoY = filaY + (rectHeight / 2) - (opcionFontSize / 2);
                DrawText(preguntaActual.opciones[i], textoX, textoY, opcionFontSize, BLACK);
            }

            // Intentos arriba derecha
            DrawText(TextFormat("Intentos: %d / %d", intentos, maxIntentos), ancho - 160, 20, 20, MAROON);

        EndDrawing();
    }

    // Mostrar mensaje final por 5 segundos
   // Mostrar mensaje final por 5 segundos
double tInicio = GetTime();
while (!WindowShouldClose() && (GetTime() - tInicio < 5.0)) {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(fondo, 0, 0, WHITE);

        if (gano) {
            const char* mensajeGanar = "¡Correcto! Has ganado.";
            int fontSize = 30;
            int x = (ancho - MeasureText(mensajeGanar, fontSize)) / 2;
            DrawText(mensajeGanar, x, 200, fontSize, DARKGREEN);
        } else {
            const char* mensajePerder = "Perdiste. La respuesta correcta era:";
            int fontSize = 25;
            int yMensaje = 180;
            int xMensaje = (ancho - MeasureText(mensajePerder, fontSize)) / 2;
            DrawText(mensajePerder, xMensaje, yMensaje, fontSize, RED);

            // Mostrar la letra correcta centrada justo debajo
            char letraCorrecta[2] = { preguntaActual.respuestaCorrecta, '\0' };
            int letraFontSize = 40;
            int xLetra = (ancho - MeasureText(letraCorrecta, letraFontSize)) / 2;
            int yLetra = yMensaje + 40;
            DrawText(letraCorrecta, xLetra, yLetra, letraFontSize, DARKGRAY);
        }

    EndDrawing();
}

    UnloadTexture(fondo);
    CloseWindow();
    return gano;
}
