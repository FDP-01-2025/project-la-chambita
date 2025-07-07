#include "raylib.h"
#include <fstream>
#include <ctime>
#include <cstring>
#include <cctype>

// Estructura para una pregunta con opciones
struct Question {
    const char* text;
    const char* options[4];
    int correctAnswer; // 0,1,2,3
};

// Guardar resultado en archivo
void saveResult(int option, bool won) {
    std::ofstream file("puntaje_minijuego_mate.txt", std::ios::app);
    if (file.is_open()) {
        file << "Respuesta dada: " << (option + 1) << "\n";
        file << "Resultado: " << (won ? "GANASTE" : "PERDISTE") << "\n";
        file << "----------------------\n";
        file.close();
    }
}

// Llama esta función dentro de tu bucle principal, pasando el área donde quieres dibujar el minijuego
// Retorna true si el minijuego terminó, y pone true/false en *won si ganó o perdió
bool playMathMinigame(Rectangle area, bool* won);

// Definición vacía para la versión sin parámetros, para evitar error de linker
bool playMathMinigame() {
    // Puedes mostrar un mensaje de error o dejarlo vacío
    return false;
}

bool playMathMinigame(Rectangle area, bool* won) {
    static bool started = false;
    static Question currentQuestion;
    static int attempts = 0;
    static const int maxAttempts = 2;
    static bool finished = false;
    static int currentSelection = -1;

    // Preguntas sencillas para niños
    static Question questions[] = {
        {"2 + 3 =", {"4", "5", "6", "7"}, 1},
        {"7 - 4 =", {"2", "3", "4", "5"}, 1},
        {"5 + 2 =", {"6", "7", "8", "9"}, 1},
        {"9 - 5 =", {"3", "4", "5", "6"}, 1},
        {"3 + 1 =", {"3", "4", "5", "6"}, 1},
        {"6 - 2 =", {"2", "3", "4", "5"}, 2},
        {"4 + 2 =", {"5", "6", "7", "8"}, 1},
        {"8 - 3 =", {"4", "5", "6", "7"}, 1}
    };
    static int totalQuestions = sizeof(questions) / sizeof(Question);

    if (!started) {
        srand((unsigned int)time(0));
        int index = rand() % totalQuestions;
        currentQuestion = questions[index];
        attempts = 0;
        finished = false;
        currentSelection = -1;
        *won = false;
        started = true;
    }

    // Dibujo
    DrawRectangleRec(area, RAYWHITE);
    int width = (int)area.width;
    int height = (int)area.height;

    // Título
    const char* title = "Resuelve el ejercicio:";
    int titleFontSize = 28;
    int titleY = (int)area.y + 20;
    int titleX = (int)area.x + (width - MeasureText(title, titleFontSize)) / 2;
    DrawText(title, titleX, titleY, titleFontSize, DARKBLUE);

    // Pregunta
    int questionFontSize = 38;
    int questionY = titleY + titleFontSize + 15;
    int questionX = (int)area.x + (width - MeasureText(currentQuestion.text, questionFontSize)) / 2;
    DrawText(currentQuestion.text, questionX, questionY, questionFontSize, BLACK);

    // Botones de opciones (4 botones)
    int buttonWidth = 180;
    int buttonHeight = 60;
    int spacingY = 25;
    int firstButtonY = questionY + questionFontSize + 30;
    int marginX = (width - buttonWidth) / 2 + (int)area.x;

    for (int i = 0; i < 4; i++) {
        Rectangle button = {
            (float)marginX,
            (float)(firstButtonY + i * (buttonHeight + spacingY)),
            (float)buttonWidth,
            (float)buttonHeight
        };

        Color bgColor = LIGHTGRAY;
        if (currentSelection == i) bgColor = YELLOW;

        // Detectar click
        if (!finished && CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentSelection = i;
            attempts++;
            if (i == currentQuestion.correctAnswer) {
                *won = true;
                finished = true;
                saveResult(i, true);
            } else if (attempts >= maxAttempts) {
                *won = false;
                finished = true;
                saveResult(i, false);
            }
        }

        DrawRectangleRec(button, bgColor);
        DrawRectangleLinesEx(button, 2, GRAY);

        // Número del botón
        char num[8];
        snprintf(num, sizeof(num), "%d", i + 1);
        DrawText(num, (int)button.x + 10, (int)button.y + 10, 22, DARKGRAY);

        // Texto de la opción
        DrawText(currentQuestion.options[i], (int)button.x + 50, (int)button.y + 15, 28, BLACK);
    }

    // Intentos
    DrawText(TextFormat("Intentos: %d / %d", attempts, maxAttempts), (int)area.x + width - 170, (int)area.y + 10, 20, MAROON);

    // Mensaje final
    if (finished) {
        Color color = *won ? DARKGREEN : RED;
        const char* message = *won ? "¡Correcto! Has ganado." : "Perdiste. La respuesta era:";
        int fontSize = 28;
        int x = (int)area.x + (width - MeasureText(message, fontSize)) / 2;
        int y = firstButtonY + 4 * (buttonHeight + spacingY) + 10;
        DrawText(message, x, y, fontSize, color);

        if (!*won) {
            // Mostrar la respuesta correcta
            const char* correct = currentQuestion.options[currentQuestion.correctAnswer];
            int fontSize2 = 32;
            int x2 = (int)area.x + (width - MeasureText(correct, fontSize2)) / 2;
            DrawText(correct, x2, y + 35, fontSize2, DARKGRAY);
        }

        // Botón para cerrar minijuego
        Rectangle btnExit = { area.x + width/2 - 60, (float)(y + 80), 120, 40 };
        DrawRectangleRec(btnExit, SKYBLUE);
        DrawRectangleLinesEx(btnExit, 2, BLUE);
        const char* txtExit = "Cerrar";
        int xExit = (int)(btnExit.x + (btnExit.width - MeasureText(txtExit, 22)) / 2);
        int yExit = (int)(btnExit.y + 8);
        DrawText(txtExit, xExit, yExit, 22, DARKBLUE);

        if (CheckCollisionPointRec(GetMousePosition(), btnExit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            started = false; // Para reiniciar la próxima vez
            return true; // Minijuego terminado
        }
    }

    return false; // Minijuego sigue activo
}