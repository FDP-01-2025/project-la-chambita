// minijuego_Palabra.cpp refactorizado
#include "raylib.h"
#include <string>
#include <ctime>
#include <fstream>
#include "juegoUNO.h"
#include <cstring>

// Estado interno del minijuego
static bool iniciado = false;
static bool terminado = false;
static bool gano = false;
static int intentoLen = 0;
static int intentos = 0;
static const int maxIntentos = 3;
static char intentoUsuario[20] = {0};
static char palabraOriginal[20];
static char palabraMezclada[20];
static int longitud = 0;
static int framesDesdeInicio = 0;

// Guardar puntaje
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

// Mezclar palabra (Fisher-Yates)
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

// Inicializar minijuego
void iniciarOrdenaPalabra()
{
    const char *palabras[] = {"computadora", "programacion", "algoritmo", "variable", "funcion"};
    int indice = rand() % 5;

    strcpy(palabraOriginal, palabras[indice]);
    longitud = strlen(palabraOriginal);
    strcpy(palabraMezclada, palabraOriginal);
    mezclarPalabra(palabraMezclada, longitud);

    intentoLen = 0;
    intentoUsuario[0] = '\0';
    intentos = 0;
    terminado = false;
    gano = false;
    iniciado = true;
    framesDesdeInicio = 0;
}

// Actualizar lógica y dibujar minijuego
void actualizarMinijuegoOrdenaPalabra(Jugador &jugador)
{
    // Posición centrada para el texto y la palabra
    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;

    framesDesdeInicio++;
    if (!iniciado)
        return;
    ClearBackground(RAYWHITE);
    // Entrada de texto
    int key = GetCharPressed();
    if (key > 0 && intentoLen < longitud)
    {
        intentoUsuario[intentoLen++] = (char)key;
        intentoUsuario[intentoLen] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE) && intentoLen > 0)
    {
        intentoLen--;
        intentoUsuario[intentoLen] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        intentos++;
        bool iguales = strcmp(intentoUsuario, palabraOriginal) == 0;

        if (iguales)
        {
            gano = true;
            terminado = true;
            guardarPuntaje(palabraOriginal, intentos);
        }
        else if (intentos >= maxIntentos)
        {
            terminado = true;
        }
        else
        {
            intentoLen = 0;
            intentoUsuario[0] = '\0';
        }
    }

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    const char *titulo = "Ordena la palabra:";
    int anchoTitulo = MeasureText(titulo, 30);
    DrawText(titulo, (screenWidth - anchoTitulo) / 2, 100, 30, DARKGRAY);
    int anchoPalabra = MeasureText(palabraMezclada, 40);
    DrawText(palabraMezclada, (screenWidth - anchoPalabra) / 2, 160, 40, BLUE);
    const char *intentoLabel = "Tu intento:";
    int anchoLabel = MeasureText(intentoLabel, 20);
    DrawText(intentoLabel, (screenWidth - anchoLabel) / 2, 230, 20, DARKGRAY);
    int anchoIntento = MeasureText(intentoUsuario, 40);
    DrawText(intentoUsuario, (screenWidth - anchoIntento) / 2, 260, 40, DARKGREEN);

    char textoIntentos[50];
    sprintf(textoIntentos, "Intentos: %d / %d", intentos, maxIntentos);
    int anchoIntentos = MeasureText(textoIntentos, 20);
    DrawText(textoIntentos, (screenWidth - anchoIntentos) / 2, 320, 20, DARKGRAY);

    if (terminado && framesDesdeInicio > 1)
    {
        const char *mensaje = gano ? "¡Correcto!" : "Fallaste";
        int anchoMensaje = MeasureText(mensaje, 30);
        DrawText(mensaje, (screenWidth - anchoMensaje) / 2, 370, 30, gano ? DARKGREEN : RED);
        const char *continuar = "Presiona ENTER para continuar";
        int anchoContinuar = MeasureText(continuar, 20);
        DrawText(continuar, (screenWidth - anchoContinuar) / 2, 420, 20, BLACK);

        if (IsKeyPressed(KEY_ENTER))
        {
            iniciado = false;
        }
    }
}

// Verificar si ya terminó
bool minijuegoOrdenaTerminado()
{
    return terminado;
}

// Verificar si ganó
bool minijuegoOrdenaGano()
{
    return gano;
}
