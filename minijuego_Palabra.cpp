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

// Función para mezclar palabra con algoritmo Fisher-Yates
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

// Función principal del minijuego
bool jugarOrdenaPalabra()
{
    InitWindow(800, 450, "Minijuego: Ordenar la palabra");
    SetTargetFPS(60);

    // Cargar imagen de fondo
    Texture2D fondo = LoadTexture("wallpapper1.png");

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

    const int anchoVentana = 800;

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

        // Escalar fondo para cubrir ventana y poner transparencia
        float escalaX = 800.0f / fondo.width;
        float escalaY = 450.0f / fondo.height;
        Color colorFondo = {255, 255, 255, 100}; // transparencia 100/255
        DrawTextureEx(fondo, (Vector2){0, 0}, 0.0f, escalaX, colorFondo);

        // Centrando textos
        const char *textoPrincipal = "Ordena la palabra:";
        int anchoTextoPrincipal = MeasureText(textoPrincipal, 20);
        int xTextoPrincipal = (anchoVentana - anchoTextoPrincipal) / 2;

        int anchoPalabraMezclada = MeasureText(palabraMezclada, 40);
        int xPalabraMezclada = (anchoVentana - anchoPalabraMezclada) / 2;

        const char *textoIntento = "Tu intento:";
        int anchoTextoIntento = MeasureText(textoIntento, 20);
        int xTextoIntento = (anchoVentana - anchoTextoIntento) / 2;

        int anchoIntentoUsuario = MeasureText(intentoUsuario, 40);
        int xIntentoUsuario = (anchoVentana - anchoIntentoUsuario) / 2;

        char textoIntentos[50];
        sprintf(textoIntentos, "Intentos: %d / %d", intentos, maxIntentos);
        int anchoTextoIntentos = MeasureText(textoIntentos, 20);
        int xTextoIntentos = (anchoVentana - anchoTextoIntentos) / 2;

        DrawText(textoPrincipal, xTextoPrincipal, 20, 20, DARKGRAY);
        DrawText(palabraMezclada, xPalabraMezclada, 60, 40, BLUE);
        DrawText(textoIntento, xTextoIntento, 130, 20, DARKGRAY);
        DrawText(intentoUsuario, xIntentoUsuario, 160, 40, DARKGREEN);
        DrawText(textoIntentos, xTextoIntentos, 220, 20, DARKGRAY);

        EndDrawing();
    }

    // Mostrar resultado 5 segundos antes de cerrar ventana
    double tiempoInicio = GetTime();
    while (!WindowShouldClose() && (GetTime() - tiempoInicio < 5.0))
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Mismo fondo con transparencia
        float escalaX = 800.0f / fondo.width;
        float escalaY = 450.0f / fondo.height;
        Color colorFondo = {255, 255, 255, 100};
        DrawTextureEx(fondo, (Vector2){0, 0}, 0.0f, escalaX, colorFondo);

        if (gano)
        {
            const char *textoGano = "¡Correcto! Has ganado.";
            int anchoGano = MeasureText(textoGano, 30);
            int xGano = (anchoVentana - anchoGano) / 2;
            DrawText(textoGano, xGano, 330, 30, DARKGREEN);
        }
        else
        {
            const char *textoPerdio = "Se acabaron los intentos.";
            int anchoPerdio = MeasureText(textoPerdio, 30);
            int xPerdio = (anchoVentana - anchoPerdio) / 2;
            DrawText(textoPerdio, xPerdio, 350, 30, RED);

            char textoPalabra[50];
            sprintf(textoPalabra, "La palabra era: %s", palabraOriginal);
            int anchoPalabra = MeasureText(textoPalabra, 20);
            int xPalabra = (anchoVentana - anchoPalabra) / 2;
            DrawText(textoPalabra, xPalabra, 330, 20, DARKGRAY);
        }

        EndDrawing();
    }

    UnloadTexture(fondo);
    CloseWindow();

    return gano;
}
