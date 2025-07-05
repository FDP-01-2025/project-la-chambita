#include "raylib.h"
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>
#include "../include/juegoUNO.h"
#include "../include/minijuego_Intruso.h"

// Estado interno del minijuego
static bool iniciado = false;
static bool terminado = false;
static bool gano = false;
static int intentos = 0;
static const int maxIntentos = 2;
static char entradaUsuario[20] = {0};
static int longitudEntrada = 0;
static Element elementos[4];
static Element intruso;
static int tamano = 4;
static int framesDesdeInicio = 0;

// Guardar puntaje
void guardarPuntajeIntruso(const std::string &intrusoEncontrado, int intentos)
{
    std::ofstream archivo("archivos/minijuego_intruso.txt", std::ios::app);
    if (archivo.is_open())
    {
        archivo << "Intruso encontrado: " << intrusoEncontrado << "\n";
        archivo << "Intentos: " << intentos << "\n";
        archivo << "Resultado: GANASTE\n";
        archivo << "----------------------\n";
        archivo.close();
    }
}

// Generar elementos aleatorios
void generarElementosAleatorios(Element arr[], int& size, Element& intruso) {
    std::string numbers[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    std::string fruits[] = {"apple", "pear", "grape", "orange", "banana", "strawberry", "kiwi", "mango"};
    std::string colors[] = {"red", "blue", "green", "yellow", "purple", "pink", "orange", "black"};
    std::string animals[] = {"dog", "cat", "elephant", "giraffe", "tiger", "lion", "monkey", "bird"};

    std::string types[] = {"number", "fruit", "color", "animal"};
    std::string selectedType = types[rand() % 4];

    size = 4; // Tamaño fijo

    // Llenar el arreglo con elementos del mismo tipo
    for (int i = 0; i < size - 1; i++) {
        if (selectedType == "number") {
            arr[i].value = numbers[rand() % 10];
            arr[i].type = "number";
        } else if (selectedType == "fruit") {
            arr[i].value = fruits[rand() % 8];
            arr[i].type = "fruit";
        } else if (selectedType == "color") {
            arr[i].value = colors[rand() % 8];
            arr[i].type = "color";
        } else {
            arr[i].value = animals[rand() % 8];
            arr[i].type = "animal";
        }
    }

    // Elegir un tipo diferente para el intruso
    std::string intruderType;
    do {
        intruderType = types[rand() % 4];
    } while (intruderType == selectedType);

    // Asignar el intruso
    if (intruderType == "number") {
        intruso.value = numbers[rand() % 10];
        intruso.type = "number";
    } else if (intruderType == "fruit") {
        intruso.value = fruits[rand() % 8];
        intruso.type = "fruit";
    } else if (intruderType == "color") {
        intruso.value = colors[rand() % 8];
        intruso.type = "color";
    } else {
        intruso.value = animals[rand() % 8];
        intruso.type = "animal";
    }

    // Colocar el intruso en una posición aleatoria
    int intruderPosition = rand() % size;
    arr[intruderPosition] = intruso;
}

// Inicializar minijuego
void iniciarMinijuegoIntruso()
{
    generarElementosAleatorios(elementos, tamano, intruso);
    
    longitudEntrada = 0;
    entradaUsuario[0] = '\0';
    intentos = 0;
    terminado = false;
    gano = false;
    iniciado = true;
    framesDesdeInicio = 0;
}

// Actualizar lógica y dibujar minijuego
void actualizarMinijuegoIntruso(Jugador &jugador)
{
    framesDesdeInicio++;
    if (!iniciado)
        return;

    ClearBackground(RAYWHITE);

    // Entrada de texto
    int key = GetCharPressed();
    if (key > 0 && longitudEntrada < 19)
    {
        entradaUsuario[longitudEntrada++] = (char)key;
        entradaUsuario[longitudEntrada] = '\0';
    }
    
    if (IsKeyPressed(KEY_BACKSPACE) && longitudEntrada > 0)
    {
        longitudEntrada--;
        entradaUsuario[longitudEntrada] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER) && longitudEntrada > 0)
    {
        intentos++;
        bool correcto = (std::string(entradaUsuario) == intruso.value);

        if (correcto)
        {
            gano = true;
            terminado = true;
            guardarPuntajeIntruso(intruso.value, intentos);
        }
        else if (intentos >= maxIntentos)
        {
            terminado = true;
        }
        else
        {
            longitudEntrada = 0;
            entradaUsuario[0] = '\0';
        }
    }

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Título
    const char *titulo = "Encuentra el intruso:";
    int anchoTitulo = MeasureText(titulo, 30);
    DrawText(titulo, (screenWidth - anchoTitulo) / 2, 50, 30, DARKGRAY);

    // Instrucciones
    const char *instrucciones = "Hay 4 elementos, uno es diferente. Encuentra el intruso:";
    int anchoInstrucciones = MeasureText(instrucciones, 20);
    DrawText(instrucciones, (screenWidth - anchoInstrucciones) / 2, 100, 20, DARKGRAY);

    // Mostrar elementos en una cuadrícula
    int elementoSize = 120;
    int espacio = 20;
    int inicioX = (screenWidth - (4 * elementoSize + 3 * espacio)) / 2;
    int inicioY = 150;

    for (int i = 0; i < tamano; i++)
    {
        int x = inicioX + i * (elementoSize + espacio);
        int y = inicioY;
        
        // Fondo del elemento
        Color colorFondo = LIGHTGRAY;
        if (elementos[i].type == "number") colorFondo = SKYBLUE;
        else if (elementos[i].type == "fruit") colorFondo = GREEN;
        else if (elementos[i].type == "color") colorFondo = LIGHTGRAY;
        else if (elementos[i].type == "animal") colorFondo = YELLOW;
        
        DrawRectangle(x, y, elementoSize, elementoSize, colorFondo);
        DrawRectangleLines(x, y, elementoSize, elementoSize, BLACK);

        // Texto del elemento
        int anchoTexto = MeasureText(elementos[i].value.c_str(), 20);
        int textoX = x + (elementoSize / 2) - (anchoTexto / 2);
        int textoY = y + (elementoSize / 2) - 10;
        DrawText(elementos[i].value.c_str(), textoX, textoY, 20, BLACK);
    }

    // Entrada del usuario
    const char *entradaLabel = "Tu respuesta:";
    int anchoLabel = MeasureText(entradaLabel, 20);
    DrawText(entradaLabel, (screenWidth - anchoLabel) / 2, 300, 20, DARKGRAY);
    
    int anchoEntrada = MeasureText(entradaUsuario, 30);
    DrawText(entradaUsuario, (screenWidth - anchoEntrada) / 2, 330, 30, DARKGREEN);

    // Intentos
    char textoIntentos[50];
    sprintf(textoIntentos, "Intentos: %d / %d", intentos, maxIntentos);
    int anchoIntentos = MeasureText(textoIntentos, 20);
    DrawText(textoIntentos, (screenWidth - anchoIntentos) / 2, 380, 20, DARKGRAY);

    // Mensaje de resultado
    if (terminado && framesDesdeInicio > 1)
    {
        const char *mensaje = gano ? "¡Correcto! Encontraste el intruso." : "Fallaste. El intruso era:";
        int anchoMensaje = MeasureText(mensaje, 25);
        DrawText(mensaje, (screenWidth - anchoMensaje) / 2, 420, 25, gano ? DARKGREEN : RED);
        
        if (!gano)
        {
            int anchoIntruso = MeasureText(intruso.value.c_str(), 30);
            DrawText(intruso.value.c_str(), (screenWidth - anchoIntruso) / 2, 450, 30, DARKGRAY);
        }
        
        const char *continuar = "Presiona ENTER para continuar";
        int anchoContinuar = MeasureText(continuar, 20);
        DrawText(continuar, (screenWidth - anchoContinuar) / 2, 490, 20, BLACK);

        if (IsKeyPressed(KEY_ENTER))
        {
            iniciado = false;
        }
    }
}

// Verificar si ya terminó
bool minijuegoIntrusoTerminado()
{
    return terminado;
}

// Verificar si ganó
bool minijuegoIntrusoGano()
{
    return gano;
} 