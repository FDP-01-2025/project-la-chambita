#include "raylib.h"
#include <fstream>
#include <ctime>
#include <cstring>
#include <cctype>

// Estructura para una pregunta con opciones
struct Pregunta {
    const char* texto;
    const char* opciones[4];
    int respuestaCorrecta; // 0,1,2,3
};

// Guardar resultado en archivo
void guardarResultado(int opcion, bool gano) {
    std::ofstream archivo("puntaje_minijuego_mate.txt", std::ios::app);
    if (archivo.is_open()) {
        archivo << "Respuesta dada: " << (opcion + 1) << "\n";
        archivo << "Resultado: " << (gano ? "GANASTE" : "PERDISTE") << "\n";
        archivo << "----------------------\n";
        archivo.close();
    }
}

// Llama esta función dentro de tu bucle principal, pasando el área donde quieres dibujar el minijuego
// Retorna true si el minijuego terminó, y pone true/false en *gano si ganó o perdió
bool jugarMinijuegoMatematico(Rectangle area, bool* gano);

// Definición vacía para la versión sin parámetros, para evitar error de linker
bool jugarMinijuegoMatematico() {
    // Puedes mostrar un mensaje de error o dejarlo vacío
    return false;
}

bool jugarMinijuegoMatematico(Rectangle area, bool* gano) {
    static bool iniciado = false;
    static Pregunta preguntaActual;
    static int intentos = 0;
    static const int maxIntentos = 2;
    static bool terminado = false;
    static int seleccionActual = -1;

    // Preguntas sencillas para niños
    static Pregunta preguntas[] = {
        {"2 + 3 =", {"4", "5", "6", "7"}, 1},
        {"7 - 4 =", {"2", "3", "4", "5"}, 1},
        {"5 + 2 =", {"6", "7", "8", "9"}, 1},
        {"9 - 5 =", {"3", "4", "5", "6"}, 1},
        {"3 + 1 =", {"3", "4", "5", "6"}, 1},
        {"6 - 2 =", {"2", "3", "4", "5"}, 2},
        {"4 + 2 =", {"5", "6", "7", "8"}, 1},
        {"8 - 3 =", {"4", "5", "6", "7"}, 1}
    };
    static int totalPreguntas = sizeof(preguntas) / sizeof(Pregunta);

    if (!iniciado) {
        srand((unsigned int)time(0));
        int indice = rand() % totalPreguntas;
        preguntaActual = preguntas[indice];
        intentos = 0;
        terminado = false;
        seleccionActual = -1;
        *gano = false;
        iniciado = true;
    }

    // Dibujo
    DrawRectangleRec(area, RAYWHITE);
    int ancho = (int)area.width;
    int alto = (int)area.height;

    // Título
    const char* titulo = "Resuelve el ejercicio:";
    int tituloFontSize = 28;
    int tituloY = (int)area.y + 20;
    int tituloX = (int)area.x + (ancho - MeasureText(titulo, tituloFontSize)) / 2;
    DrawText(titulo, tituloX, tituloY, tituloFontSize, DARKBLUE);

    // Pregunta
    int preguntaFontSize = 38;
    int preguntaY = tituloY + tituloFontSize + 15;
    int preguntaX = (int)area.x + (ancho - MeasureText(preguntaActual.texto, preguntaFontSize)) / 2;
    DrawText(preguntaActual.texto, preguntaX, preguntaY, preguntaFontSize, BLACK);

    // Botones de opciones (4 botones)
    int botonWidth = 180;
    int botonHeight = 60;
    int espacioY = 25;
    int primerBotonY = preguntaY + preguntaFontSize + 30;
    int margenX = (ancho - botonWidth) / 2 + (int)area.x;

    for (int i = 0; i < 4; i++) {
        Rectangle boton = {
            (float)margenX,
            (float)(primerBotonY + i * (botonHeight + espacioY)),
            (float)botonWidth,
            (float)botonHeight
        };

        Color colorFondo = LIGHTGRAY;
        if (seleccionActual == i) colorFondo = YELLOW;

        // Detectar click
        if (!terminado && CheckCollisionPointRec(GetMousePosition(), boton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            seleccionActual = i;
            intentos++;
            if (i == preguntaActual.respuestaCorrecta) {
                *gano = true;
                terminado = true;
                guardarResultado(i, true);
            } else if (intentos >= maxIntentos) {
                *gano = false;
                terminado = true;
                guardarResultado(i, false);
            }
        }

        DrawRectangleRec(boton, colorFondo);
        DrawRectangleLinesEx(boton, 2, GRAY);

        // Número del botón
        char num[8];
        snprintf(num, sizeof(num), "%d", i + 1);
        DrawText(num, (int)boton.x + 10, (int)boton.y + 10, 22, DARKGRAY);

        // Texto de la opción
        DrawText(preguntaActual.opciones[i], (int)boton.x + 50, (int)boton.y + 15, 28, BLACK);
    }

    // Intentos
    DrawText(TextFormat("Intentos: %d / %d", intentos, maxIntentos), (int)area.x + ancho - 170, (int)area.y + 10, 20, MAROON);

    // Mensaje final
    if (terminado) {
        Color color = *gano ? DARKGREEN : RED;
        const char* mensaje = *gano ? "¡Correcto! Has ganado." : "Perdiste. La respuesta era:";
        int fontSize = 28;
        int x = (int)area.x + (ancho - MeasureText(mensaje, fontSize)) / 2;
        int y = primerBotonY + 4 * (botonHeight + espacioY) + 10;
        DrawText(mensaje, x, y, fontSize, color);

        if (!*gano) {
            // Mostrar la respuesta correcta
            const char* correcta = preguntaActual.opciones[preguntaActual.respuestaCorrecta];
            int fontSize2 = 32;
            int x2 = (int)area.x + (ancho - MeasureText(correcta, fontSize2)) / 2;
            DrawText(correcta, x2, y + 35, fontSize2, DARKGRAY);
        }

        // Botón para cerrar minijuego
        Rectangle btnSalir = { area.x + ancho/2 - 60, (float)(y + 80), 120, 40 };
        DrawRectangleRec(btnSalir, SKYBLUE);
        DrawRectangleLinesEx(btnSalir, 2, BLUE);
        const char* txtSalir = "Cerrar";
        int xSalir = (int)(btnSalir.x + (btnSalir.width - MeasureText(txtSalir, 22)) / 2);
        int ySalir = (int)(btnSalir.y + 8);
        DrawText(txtSalir, xSalir, ySalir, 22, DARKBLUE);

        if (CheckCollisionPointRec(GetMousePosition(), btnSalir) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            iniciado = false; // Para reiniciar la próxima vez
            return true; // Minijuego terminado
        }
    }

    return false; // Minijuego sigue activo
}