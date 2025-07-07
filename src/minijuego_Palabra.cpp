// word_minigame.cpp refactorizado
#include "raylib.h"
#include <string>
#include <ctime>
#include <fstream>
#include "../include/juegoUNO.h"
#include <cstring>

using namespace std;

// Estado interno del minijuego
static bool started = false;
static bool finished = false;
static bool won = false;
static int attemptLen = 0;
static int attempts = 0;
static const int maxAttempts = 3;
static char userAttempt[21] = {0}; // +1 para null terminator
static char originalWord[21];
static char mixedWord[21];
static int length = 0;
static int framesSinceStart = 0;

// Guardar puntaje
void saveScore(const string &word, int attempts)
{
    ofstream file("archivos/minijuego_palabra.txt", ios::app);
    if (file.is_open())
    {
        file << "Palabra: " << word << "\n";
        file << "Intentos: " << attempts << "\n";
        file << "Resultado: GANASTE\n";
        file << "----------------------\n";
        file.close();
    }
}

// Mezclar palabra (Fisher-Yates), asegurando que no sea igual a la original
void mixWord(char word[], int length)
{
    if (length <= 1)
        return;
    char original[21];
    strcpy(original, word);
    do
    {
        for (int i = length - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            char temp = word[i];
            word[i] = word[j];
            word[j] = temp;
        }
    } while (strcmp(original, word) == 0 && length > 1);
}

// Inicializar minijuego
void startWordOrderMinigame()
{
    const char *words[] = {"computadora", "programacion", "algoritmo", "variable", "funcion"};
    int index = rand() % 5;

    strcpy(originalWord, words[index]);
    length = strlen(originalWord);
    strcpy(mixedWord, originalWord);
    mixWord(mixedWord, length);

    attemptLen = 0;
    userAttempt[0] = '\0';
    attempts = 0;
    finished = false;
    won = false;
    started = true;
    framesSinceStart = 0;
}

// Actualizar lógica y dibujar minijuego
void updateWordOrderMinigame(Player &player)
{
    if (!started)
        return;

    // Solo cuenta frames cuando está iniciado
    framesSinceStart++;

    // Visual
    ClearBackground(RAYWHITE);

    // Entrada de texto solo si no ha terminado
    if (!finished)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if (attemptLen < length && attemptLen < 20 && key >= 32 && key <= 125)
            {
                userAttempt[attemptLen++] = (char)key;
                userAttempt[attemptLen] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && attemptLen > 0)
        {
            attemptLen--;
            userAttempt[attemptLen] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            attempts++;
            bool equal = strcmp(userAttempt, originalWord) == 0;

            if (equal)
            {
                won = true;
                finished = true;
                saveScore(originalWord, attempts);
            }
            else if (attempts >= maxAttempts)
            {
                finished = true;
            }
            else
            {
                attemptLen = 0;
                userAttempt[0] = '\0';
            }
        }
    }

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    const char *title = "Ordena la palabra:";
    int titleWidth = MeasureText(title, 30);
    DrawText(title, (screenWidth - titleWidth) / 2, 100, 30, DARKGRAY);
    int wordWidth = MeasureText(mixedWord, 40);
    DrawText(mixedWord, (screenWidth - wordWidth) / 2, 160, 40, BLUE);
    const char *attemptLabel = "Tu intento:";
    int labelWidth = MeasureText(attemptLabel, 20);
    DrawText(attemptLabel, (screenWidth - labelWidth) / 2, 230, 20, DARKGRAY);
    int attemptWidth = MeasureText(userAttempt, 40);
    DrawText(userAttempt, (screenWidth - attemptWidth) / 2, 260, 40, DARKGREEN);

    char attemptsText[50];
    sprintf(attemptsText, "Intentos: %d / %d", attempts, maxAttempts);
    int attemptsWidth = MeasureText(attemptsText, 20);
    DrawText(attemptsText, (screenWidth - attemptsWidth) / 2, 320, 20, DARKGRAY);

    if (finished && framesSinceStart > 1)
    {
        const char *message = won ? "¡Correcto!" : "Fallaste";
        int messageWidth = MeasureText(message, 30);
        DrawText(message, (screenWidth - messageWidth) / 2, 370, 30, won ? DARKGREEN : RED);
        const char *continueText = "Presiona ENTER para continuar";
        int continueWidth = MeasureText(continueText, 20);
        DrawText(continueText, (screenWidth - continueWidth) / 2, 420, 20, BLACK);

        // Solo permite salir después de mostrar el mensaje al menos un frame
        if (IsKeyPressed(KEY_ENTER) && framesSinceStart > 10)
        {
            started = false;
        }
    }
}

// Verificar si ya terminó
bool wordOrderMinigameFinished()
{
    return finished;
}

// Verificar si ganó
bool wordOrderMinigameWon()
{
    return won;
}
