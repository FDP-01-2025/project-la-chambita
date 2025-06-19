#include "raylib.h"
#include <string>
#include <ctime>
#include <fstream>

// Función para guardar resultados en archivo
void guardarPuntaje(const std::string &palabra, int intentos)
{
    std::ofstream archivo("puntaje_minijuego.txt", std::ios::app);
    if (archivo.is_open())
    {
        archivo << "Palabra: " << palabra << "\n";
        archivo << "Intentos: " << intentos << "\n";
        archivo << "Resultado: GANASTE\n";
        archivo << "----------------------\n";
        archivo.close();
    }
}

// Función para mezclar palabra con algoritmo simple (Fisher-Yates)
void mezclarPalabra(char palabra[], int length)
{
    for (int i = length - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        char temp = palabra[i];
        palabra[i] = palabra[j];
        palabra[j] = temp;
    }
}

bool jugarOrdenaPalabra()
{
    InitWindow(800, 450, "Minijuego: Ordenar la palabra");
    SetTargetFPS(60);

    const int TOTAL_PALABRAS = 5;
    const char palabras[TOTAL_PALABRAS][20] = {
        "computadora",
        "programacion",
        "algoritmo",
        "variable",
        "funcion"};

    srand(time(0));
    int indice = rand() % TOTAL_PALABRAS;

    char palabraOriginal[20];
    int longitud = 0;
    while (palabras[indice][longitud] != '\0' && longitud < 19)
    {
        palabraOriginal[longitud] = palabras[indice][longitud];
        longitud++;
    }
    palabraOriginal[longitud] = '\0';

    char palabraMezclada[20];
    for (int i = 0; i <= longitud; i++)
    {
        palabraMezclada[i] = palabraOriginal[i];
    }

    mezclarPalabra(palabraMezclada, longitud);

    char intentoUsuario[20] = {0};
    int intentoLen = 0;

    int intentos = 0;
    const int maxIntentos = 3;
    bool juegoTerminado = false;
    bool gano = false;

    while (!WindowShouldClose() && !juegoTerminado)
    {
        int key = GetCharPressed();
        if (key > 0)
        {
            if ((key >= 32) && (key <= 125) && intentoLen < longitud)
            {
                intentoUsuario[intentoLen] = (char)key;
                intentoLen++;
                intentoUsuario[intentoLen] = '\0';
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && intentoLen > 0)
        {
            intentoLen--;
            intentoUsuario[intentoLen] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            intentos++;

            bool iguales = true;
            for (int i = 0; i < longitud; i++)
            {
                if (intentoUsuario[i] != palabraOriginal[i])
                {
                    iguales = false;
                    break;
                }
            }

            if (iguales)
            {
                gano = true;
                juegoTerminado = true;
                guardarPuntaje(palabraOriginal, intentos);
            }
            else
            {
                if (intentos >= maxIntentos)
                {
                    juegoTerminado = true;
                }
                else
                {
                    intentoLen = 0;
                    intentoUsuario[0] = '\0';
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Ordena la palabra:", 20, 20, 20, DARKGRAY);
        DrawText(palabraMezclada, 20, 60, 40, BLUE);

        DrawText("Tu intento:", 20, 130, 20, DARKGRAY);
        DrawText(intentoUsuario, 20, 160, 40, MAROON);

        DrawText(TextFormat("Intentos: %d / %d", intentos, maxIntentos), 20, 220, 20, DARKGRAY);

        if (juegoTerminado)
        {
            // Mostrar resultado durante 5 segundos antes de cerrar
            double tiempoInicio = GetTime();
            while (!WindowShouldClose() && (GetTime() - tiempoInicio < 5.0))
            {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                if (gano)
                {
                    DrawText("¡Correcto! Has ganado.", 20, 270, 30, GREEN);
                }
                else
                {
                    DrawText("Se acabaron los intentos.", 20, 270, 30, RED);
                    DrawText(TextFormat("La palabra era: %s", palabraOriginal), 20, 310, 20, DARKGRAY);
                }
                EndDrawing();
            }
            break; // Salir del while principal después de la espera
        }
    }

    CloseWindow();
    return gano;
}
