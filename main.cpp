#include "raylib.h"
#include "include/juegoUNO.h"
#include "include/minijuego_Palabra.h"
#include "include/minijuego_Intruso.h"
#include "include/minijuego_Velocidad.h"
#include "include/minijuego_matematico.h"
using namespace std;

int main()
{
    // aqui va lo de raylib
    //dimesiones para joel 1366x768
    //dimesiones para aaron pantalla 1920x1080
    InitWindow(1920, 1080, " UNO with minijuegos");
    SetTargetFPS(30);

    UNO_Game game = createUNOGame();

    bool amountSelected;
    int currentPlayer;
    string currentInput;
    bool namesCompleted;

    initStateVariables(amountSelected, currentPlayer, currentInput, namesCompleted);
    initializeDeck(game);
    shuffleDeck(game);

    //  Aquí se ejecuta todo el ciclo del juego
    runGame(game, amountSelected, currentPlayer, currentInput, namesCompleted);

    CloseWindow();

    PlayerStatistics stats;

    // Simulación al terminar una partida
    bool playerWon = true;
    int minigamesThisGame = 2;

    return 0;
}