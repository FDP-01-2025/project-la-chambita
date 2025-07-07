#include "raylib.h"
#include <string>
#include <ctime>
#include <fstream>
#include "../include/juegoUNO.h"
#include "../include/minijuego_Velocidad.h"

using namespace std;

// Estado interno del minijuego
static bool started = false;
static bool finished = false;
static bool won = false;
static float timeLimit = 2.0f; // 2 segundos para reaccionar
static float elapsedTime = 0.0f;
static char correctKey = 'A';
static int framesSinceStart = 0;

// Guardar puntaje
void saveSpeedScore(char key, bool won, float time)
{
    ofstream file("archivos/minijuego_velocidad.txt", ios::app);
    if (file.is_open())
    {
        file << "Tecla presionada: " << key << "\n";
        file << "Tiempo usado: " << time << "s\n";
        file << "Resultado: " << (won ? "GANASTE" : "PERDISTE") << "\n";
        file << "----------------------\n";
        file.close();
    }
}

// Inicializar minijuego
void startSpeedMinigame()
{
    correctKey = 'A' + GetRandomValue(0, 25);
    elapsedTime = 0.0f;
    finished = false;
    won = false;
    started = true;
    framesSinceStart = 0;
}

// Actualizar lógica y dibujar minijuego
void updateSpeedMinigame(Player &player)
{
    if (!started)
        return;

    framesSinceStart++;
    float deltaTime = GetFrameTime();

    if (!finished)
    {
        elapsedTime += deltaTime;

        // Verificar si se presionó la tecla correcta
        if (IsKeyPressed((int)correctKey))
        {
            won = true;
            finished = true;
            saveSpeedScore(correctKey, true, elapsedTime);
        }
        // Verificar si se acabó el tiempo
        else if (elapsedTime >= timeLimit)
        {
            finished = true;
            saveSpeedScore(correctKey, false, timeLimit);
        }
    }

    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Título
    const char *title = "¡MINIJUEGO DE VELOCIDAD!";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, (screenWidth - titleWidth) / 2, 100, 40, YELLOW);

    // Instrucciones
    const char *instructions = "Presiona la tecla que aparece en pantalla lo más rápido posible";
    int instructionsWidth = MeasureText(instructions, 25);
    DrawText(instructions, (screenWidth - instructionsWidth) / 2, 180, 25, LIGHTGRAY);

    // Mostrar la tecla a presionar
    char keyStr[2] = {correctKey, '\0'};
    int keyWidth = MeasureText(keyStr, 80);
    DrawText(keyStr, (screenWidth - keyWidth) / 2, 250, 80, WHITE);

    // Tiempo restante
    float timeLeft = timeLimit - elapsedTime;
    if (timeLeft < 0) timeLeft = 0;

    char timeStr[50];
    sprintf(timeStr, "Tiempo restante: %.1f", timeLeft);
    int timeWidth = MeasureText(timeStr, 30);
    DrawText(timeStr, (screenWidth - timeWidth) / 2, 350, 30, RED);

    // Barra de progreso del tiempo
    float progress = elapsedTime / timeLimit;
    if (progress > 1.0f) progress = 1.0f;

    int barWidth = 400;
    int barHeight = 20;
    int barX = (screenWidth - barWidth) / 2;
    int barY = 400;

    // Fondo de la barra
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    // Progreso de la barra
    DrawRectangle(barX, barY, (int)(barWidth * progress), barHeight, RED);

    // Mensaje de resultado
    if (finished)
    {
        const char *message = won ? "¡Excelente! Presionaste la tecla a tiempo." : "¡Demasiado lento! Se acabó el tiempo.";
        int messageWidth = MeasureText(message, 25);
        DrawText(message, (screenWidth - messageWidth) / 2, 450, 25, won ? DARKGREEN : RED);

        if (won)
        {
            char usedTimeStr[50];
            sprintf(usedTimeStr, "Tiempo usado: %.2f segundos", elapsedTime);
            int usedTimeWidth = MeasureText(usedTimeStr, 20);
            DrawText(usedTimeStr, (screenWidth - usedTimeWidth) / 2, 480, 20, LIGHTGRAY);
        }

        const char *continueText = "Presiona ENTER para continuar";
        int continueWidth = MeasureText(continueText, 20);
        DrawText(continueText, (screenWidth - continueWidth) / 2, 520, 20, WHITE);

        if (IsKeyPressed(KEY_ENTER))
        {
            started = false;
        }
    }
}

// Verificar si ya terminó
bool speedMinigameFinished()
{
    return finished;
}

// Verificar si ganó
bool speedMinigameWon()
{
    return won;
}
