#include "raylib.h"
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>
#include "../include/UNO_game.h"
#include "../include/intruder_minigame.h"

using namespace std;

// Estado interno del minijuego
static bool started = false;
static bool finished = false;
static bool won = false;
static int attempts = 0;
static const int maxAttempts = 2;
static char userInput[20] = {0};
static int inputLength = 0;
static Element elements[4];
static Element intruder;
static int size = 4;
static int framesSinceStart = 0;

// Guardar puntaje
void saveIntruderScore(const string &foundIntruder, int attempts)
{
    ofstream file("archivos/minijuego_intruso.txt", ios::app);
    if (file.is_open())
    {
        file << "Intruso encontrado: " << foundIntruder << "\n";
        file << "Intentos: " << attempts << "\n";
        file << "Resultado: GANASTE\n";
        file << "----------------------\n";
        file.close();
    }
}

// Generar elementos aleatorios
void generateRandomElements(Element arr[], int& size, Element& intruder) {
    string numbers[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    string fruits[] = {"apple", "pear", "grape", "orange", "banana", "strawberry", "kiwi", "mango"};
    string colors[] = {"red", "blue", "green", "yellow", "purple", "pink", "orange", "black"};
    string animals[] = {"dog", "cat", "elephant", "giraffe", "tiger", "lion", "monkey", "bird"};

    string types[] = {"number", "fruit", "color", "animal"};
    string selectedType = types[rand() % 4];

    size = 4; // Tamaño fijo

    // Llenar el arreglo con elementos del mismo tipo
    for (int i = 0; i < size; i++) {
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
    string intruderType;
    do {
        intruderType = types[rand() % 4];
    } while (intruderType == selectedType);

    // Asignar el intruso
    if (intruderType == "number") {
        intruder.value = numbers[rand() % 10];
        intruder.type = "number";
    } else if (intruderType == "fruit") {
        intruder.value = fruits[rand() % 8];
        intruder.type = "fruit";
    } else if (intruderType == "color") {
        intruder.value = colors[rand() % 8];
        intruder.type = "color";
    } else {
        intruder.value = animals[rand() % 8];
        intruder.type = "animal";
    }

    // Colocar el intruso en una posición aleatoria
    int intruderPosition = rand() % size;
    arr[intruderPosition] = intruder;
}

// Inicializar minijuego
void startIntruderMinigame()
{
    generateRandomElements(elements, size, intruder);

    inputLength = 0;
    userInput[0] = '\0';
    attempts = 0;
    finished = false;
    won = false;
    started = true;
    framesSinceStart = 0;
}

// Actualizar lógica y dibujar minijuego
void updateIntruderMinigame(Player &player)
{
    framesSinceStart++;
    if (!started)
        return;

    ClearBackground(RAYWHITE);

    // Entrada de texto
    int key = GetCharPressed();
    if (key > 0 && inputLength < 19)
    {
        userInput[inputLength++] = (char)key;
        userInput[inputLength] = '\0';
    }

    if (IsKeyPressed(KEY_BACKSPACE) && inputLength > 0)
    {
        inputLength--;
        userInput[inputLength] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER) && inputLength > 0)
    {
        attempts++;
        bool correct = (string(userInput) == intruder.value);

        if (correct)
        {
            won = true;
            finished = true;
            saveIntruderScore(intruder.value, attempts);
        }
        else if (attempts >= maxAttempts)
        {
            finished = true;
        }
        else
        {
            inputLength = 0;
            userInput[0] = '\0';
        }
    }

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Título
    const char *title = "Encuentra el intruso:";
    int titleWidth = MeasureText(title, 30);
    DrawText(title, (screenWidth - titleWidth) / 2, 50, 30, DARKGRAY);

    // Instrucciones
    const char *instructions = "Hay 4 elementos, uno es diferente. Encuentra el intruso:";
    int instructionsWidth = MeasureText(instructions, 20);
    DrawText(instructions, (screenWidth - instructionsWidth) / 2, 100, 20, DARKGRAY);

    // Mostrar elementos en una cuadrícula
    int elementSize = 120;
    int spacing = 20;
    int startX = (screenWidth - (4 * elementSize + 3 * spacing)) / 2;
    int startY = 150;

    for (int i = 0; i < size; i++)
    {
        int x = startX + i * (elementSize + spacing);
        int y = startY;

        // Fondo del elemento
        Color bgColor = LIGHTGRAY;
        if (elements[i].type == "number") bgColor = SKYBLUE;
        else if (elements[i].type == "fruit") bgColor = GREEN;
        else if (elements[i].type == "color") bgColor = PURPLE;
        else if (elements[i].type == "animal") bgColor = YELLOW;

        DrawRectangle(x, y, elementSize, elementSize, bgColor);
        DrawRectangleLines(x, y, elementSize, elementSize, BLACK);

        // Texto del elemento
        int textWidth = MeasureText(elements[i].value.c_str(), 20);
        int textX = x + (elementSize / 2) - (textWidth / 2);
        int textY = y + (elementSize / 2) - 10;
        DrawText(elements[i].value.c_str(), textX, textY, 20, BLACK);
    }

    // Entrada del usuario
    const char *inputLabel = "Tu respuesta:";
    int labelWidth = MeasureText(inputLabel, 20);
    DrawText(inputLabel, (screenWidth - labelWidth) / 2, 300, 20, DARKGRAY);

    int inputWidth = MeasureText(userInput, 30);
    DrawText(userInput, (screenWidth - inputWidth) / 2, 330, 30, DARKGREEN);

    // Intentos
    char attemptsText[50];
    sprintf(attemptsText, "Intentos: %d / %d", attempts, maxAttempts);
    int attemptsWidth = MeasureText(attemptsText, 20);
    DrawText(attemptsText, (screenWidth - attemptsWidth) / 2, 380, 20, DARKGRAY);

    // Mensaje de resultado
    if (finished && framesSinceStart > 1)
    {
        const char *message = won ? "¡Correcto! Encontraste el intruso." : "Fallaste. El intruso era:";
        int messageWidth = MeasureText(message, 25);
        DrawText(message, (screenWidth - messageWidth) / 2, 420, 25, won ? DARKGREEN : RED);

        if (!won)
        {
            int intruderWidth = MeasureText(intruder.value.c_str(), 30);
            DrawText(intruder.value.c_str(), (screenWidth - intruderWidth) / 2, 450, 30, DARKGRAY);
        }

        const char *continueText = "Presiona ENTER para continuar";
        int continueWidth = MeasureText(continueText, 20);
        DrawText(continueText, (screenWidth - continueWidth) / 2, 490, 20, BLACK);

        if (IsKeyPressed(KEY_ENTER))
        {
            started = false;
        }
    }
}

// Verificar si ya terminó
bool intruderMinigameFinished()
{
    return finished;
}

// Verificar si ganó
bool intruderMinigameWon()
{
    return won;
}