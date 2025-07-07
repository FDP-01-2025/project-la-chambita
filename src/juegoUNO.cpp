// Implementación de las funciones principales del juego UNO
// Este archivo contiene la lógica del juego, manejo de cartas, turnos, minijuegos y visualización with raylib.

#include "../include/juegoUNO.h"
#include <algorithm>
#include <random>
#include <string>
#include <fstream>
#include <cstring>
#include "raylib.h"
#include "../include/minijuego_Palabra.h"
#include "../include/minijuego_Intruso.h"
#include "../include/minijuego_Velocidad.h"
#include "../include/minijuego_matematico.h"
using namespace std;

void assignRandomColorPlusTwoPlusFour(UNO_Game &game);

// Crea e inicializa una estructura UNO_Game with values by default.
UNO_Game createUNOGame()
{
    UNO_Game game;
    game.gameState = WaitingPlayers;
    game.currentTurn = 0;
    game.direction = 1;
    game.activeMinigame = None;
    game.playerCount = 0;
    game.cardsInDeck = 0;
    game.cardsInDiscard = 0;

    game.forcedColor = "";
    game.pendingCard.color = "";
    game.pendingCard.type = Number;
    game.pendingCard.value = -1;
    game.pendingCard.visible = false;

    game.cardInPlay.color = "";
    game.cardInPlay.type = Number;
    game.cardInPlay.value = -1;
    game.cardInPlay.visible = false;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        game.players[i].name = "";
        game.players[i].minigamesWon = 0;
        game.players[i].gamesWon = 0;
        game.players[i].gamesLost = 0;
        game.players[i].isTurn = false;
        game.players[i].isNew = true;

        for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
        {
            game.players[i].hand[j].color = "";
            game.players[i].hand[j].type = Number;
            game.players[i].hand[j].value = -1;
            game.players[i].hand[j].visible = false;
        }
    }

    return game;
}

// Declaración global or al inicio de la función principal
Vector2 minigamePosition = {100.0f, 50.0f};

// Inicializa variables of state for the selection of players and names.
void initStateVariables(bool &amountSelected, int &currentPlayer, string &currentInput, bool &namesCompleted)
{
    amountSelected = false;
    currentPlayer = 0;
    currentInput = "";
    namesCompleted = false;
}

// Llena el mazo with all cards of UNO (numbers, specials and comodines).
void initializeDeck(UNO_Game &game)
{
    string colors[4] = {"rojo", "amarillo", "verde", "azul"};
    int index = 0;

    // Agrega cartas of each color
    for (int i = 0; i < 4; i++)
    {
        string color = colors[i];

        // Carta 0 (solo una por color)
        Card c0;
        c0.color = color;
        c0.type = Number;
        c0.value = 0;
        c0.visible = false;
        game.deck[index++] = c0;

        // Cartas 1-9 (dos de cada por color)
        for (int num = 1; num <= 9; num++)
        {
            for (int j = 0; j < 2; j++)
            {
                Card c;
                c.color = color;
                c.type = Number;
                c.value = num;
                c.visible = false;
                game.deck[index++] = c;
            }
        }

        // Cartas especiales: +2, skip, reverse (dos de cada por color)
        for (int j = 0; j < 2; j++)
        {
            Card plusTwoCard;
            plusTwoCard.color = color;
            plusTwoCard.type = PlusTwo;
            plusTwoCard.value = -1; // Valor -1 for special cards
            plusTwoCard.visible = false;
            game.deck[index++] = plusTwoCard;

            Card skipCard;
            skipCard.color = color;
            skipCard.type = Skip;
            skipCard.value = -1;
            skipCard.visible = false;
            game.deck[index++] = skipCard;

            Card reverseCard;
            reverseCard.color = color;
            reverseCard.type = Reverse;
            reverseCard.value = -1;
            reverseCard.visible = false;
            game.deck[index++] = reverseCard;
        }
    }
    // Comodines: +4 and change color (4 of each, color negro)
    for (int i = 0; i < 4; i++)
    {
        Card plusFourCard;
        plusFourCard.color = "negro";
        plusFourCard.type = PlusFour;
        plusFourCard.value = -1;
        plusFourCard.visible = false;
        game.deck[index++] = plusFourCard;

        Card changeColorCard;
        changeColorCard.color = "negro";
        changeColorCard.type = ChangeColor;
        changeColorCard.value = -1;
        changeColorCard.visible = false;
        game.deck[index++] = changeColorCard;
    }
    game.cardsInDeck = index;
}

// Baraja el mazo de cartas using a random generator.
void barajarMazo(UNO_Game &mazo)
{
    random_device rd;
    mt19937 generador(rd());
    shuffle(mazo.deck, mazo.deck + mazo.cardsInDeck, generador);
}

// Dibuja botones para seleccionar la cantidad of players and detecta la selección.
void seleccionarCantidadJugadores(UNO_Game &game, bool &amountSelected)
{

    DrawText("selecciona la cantidad of players: ", 100, 100, 30, DARKGRAY);

    Rectangle botones[3]{
        {100, 200, 100, 50},
        {250, 200, 100, 50},
        {400, 200, 100, 50}};

    for (int i = 0; i < 3; i++)
    {
        DrawRectangleRec(botones[i], LIGHTGRAY);
        DrawRectangleLinesEx(botones[i], 2, BLACK);                                         // Contorno of the buttons
        DrawText(TextFormat("%d", i + 2), botones[i].x + 35, botones[i].y + 10, 30, BLACK); // Texto of the button
    }

    // Detecta clic en algún botón
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        for (int i = 0; i < 3; i++)
        {
            if (CheckCollisionPointRec(mouse, botones[i]))
            {
                game.playerCount = i + 2;
                amountSelected = true;
            }
        }
    }
}

// Permite capturar los names of the players one by one from the window.
void capturarNombresEnLaVentana(UNO_Game &game, int &currentPlayer, string &currentInput, bool &namesCompleted)
{
    // Captura teclas presionadas como caracteres
    int letra = GetCharPressed();

    while (letra > 0)
    {
        // Solo acepta caracteres imprimibles and máximo 20 letras por name
        if (letra >= 32 && letra <= 125 && currentInput.length() < 20)
        {
            currentInput += static_cast<char>(letra);
        }
        letra = GetCharPressed();
    }

    // Permite borrar el último carácter with BACKSPACE
    if (IsKeyPressed(KEY_BACKSPACE) && !currentInput.empty())
    {
        currentInput.pop_back();
    }

    // Al presionar ENTER, guarda el name and pasa al siguiente jugador
    if (IsKeyPressed(KEY_ENTER) && !currentInput.empty())
    {
        game.players[currentPlayer].name = currentInput;
        game.players[currentPlayer].minigamesWon = 0;
        game.players[currentPlayer].gamesWon = 0;
        game.players[currentPlayer].isTurn = false;

        currentInput.clear();
        currentPlayer++;

        if (currentPlayer >= game.playerCount)
        {
            namesCompleted = true;
        }
    }

    // Muestra instrucciones and name actual en pantalla
    DrawText("Escribe el name of the player: ", 100, 100, 30, DARKGRAY);
    DrawText(TextFormat("player %d:", currentPlayer + 1), 100, 150, 30, BLUE);
    DrawText(currentInput.c_str(), 100, 200, 30, BLUE);
}

// Reparte 7 cartas to each player from the mazo.
void repartirCartas(UNO_Game &game)
{
    const int CARDS_PER_PLAYER = 7;
    for (int i = 0; i < game.playerCount; i++)
    {
        for (int j = 0; j < CARDS_PER_PLAYER; j++)
        {
            if (game.cardsInDeck <= 0)
            {
                cout << "Error: no hay suficientes cartas para repartir." << endl;
                return;
            }
            if (game.players[i].hand[j].color.empty()) // Solo reparte if the slot is empty
                game.players[i].hand[j] = game.deck[--game.cardsInDeck];
        }
    }
}

// Permite al player robar una carta and avanza the turno if no puede jugarla.
void intentarRobarYCambiarTurno(UNO_Game &game)
{
    Card cartaRobada = robarCartaValida(game);
    if (sePuedeJugar(game.cardInPlay, cartaRobada))
    {
        game.cardInPlay = cartaRobada;
        // La carta robada se juega inmediatamente if es válida
    }
    else
    {
        // Si no se puede jugar, se añade to the mano of the player
        Player &player = game.players[game.currentTurn];
        for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
        {
            if (player.hand[i].color.empty())
            {
                player.hand[i] = cartaRobada;
                break;
            }
        }
    }

    avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
    actualizarVisibilidadCartas(game);
}

// Verifica if the player has any playable card in their mano.
bool tieneCartaJugable(const Player &player, Card cardInPlay)
{
    for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
    {
        const Card &c = player.hand[i];
        if (!c.color.empty() && sePuedeJugar(cardInPlay, c))
        {
            return true;
        }
    }
    return false;
}

// Roba una carta del mazo and la devuelve. Si es jugable, la pone en juego.
Card robarCartaValida(UNO_Game &game)
{
    if (game.cardsInDeck <= 0)
        return Card{}; // No hay cartas en el mazo

    Card cartaRobada = game.deck[--game.cardsInDeck];
    cartaRobada.visible = true;

    // Si la carta robada es jugable, se pone en juego
    if (sePuedeJugar(game.cardInPlay, cartaRobada))
    {
        game.cardInPlay = cartaRobada;
    }
    else
    {
        // Si no es jugable, se añade to the mano of the actual player
        Player &player = game.players[game.currentTurn];

        for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
        {
            if (player.hand[i].color.empty())
            {
                player.hand[i] = cartaRobada;
                break;
            }
        }

        // Avanza the turno because no pudo jugar
        avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
    }

    return cartaRobada; // Retorna la carta robada
}

// Función simple para robar una carta without changing the turno (for penalties)
Card robarCartaSimple(UNO_Game &game)
{
    if (game.cardsInDeck <= 0)
        return Card{}; // No hay cartas en el mazo

    Card cartaRobada = game.deck[--game.cardsInDeck];
    cartaRobada.visible = true;
    return cartaRobada;
}

// Actualiza la visibilidad of the cards: only the player in turn sees their cards.
void actualizarVisibilidadCartas(UNO_Game &game)
{
    for (int i = 0; i < game.playerCount; i++)
    {
        for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
        {
            game.players[i].hand[j].visible = (i == game.currentTurn);
        }
    }
}

// Selecciona the initial card of the game (cannot be comodín).
Card cartaInicial(UNO_Game &game)
{
    for (int i = 0; i < MAX_DECK; i++)
    {
        Card &carta = game.deck[i];

        // Solo acepta cartas numéricas (no comodines ni especiales)
        if (!carta.color.empty() && carta.type == Number)
        {
            Card cartaInicial = carta;
            carta = Card{}; // Eliminar del mazo
            return cartaInicial;
        }
    }

    // Si no encuentra carta válida (poco probable), devuelve vacía
    return Card{};
}

// menu principal
MenuResult MenuPrincipal()
{
    Rectangle botonJugar = {583, 300, 200, 60};
    Rectangle botonCerrar = {583, 400, 200, 60};

    Vector2 mouse = GetMousePosition();

    DrawText("Bienvenidos a UNO", 550, 200, 30, DARKGRAY);

    DrawRectangleRec(botonJugar, CheckCollisionPointRec(mouse, botonJugar) ? LIGHTGRAY : GRAY);
    DrawText("JUGAR", botonJugar.x + 60, botonJugar.y + 15, 30, BLACK);

    DrawRectangleRec(botonCerrar, CheckCollisionPointRec(mouse, botonCerrar) ? LIGHTGRAY : GRAY);
    DrawText("CERRAR", botonCerrar.x + 55, botonCerrar.y + 15, 30, BLACK);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mouse, botonJugar))
        {
            return MENU_PLAY;
        }
        else if (CheckCollisionPointRec(mouse, botonCerrar))
        {
            return MENU_EXIT;
        }
    }

    return MENU_NONE; // si no se ha hecho clic en ningún botón
}

// Bucle principal del juego: gestiona the flujo of screens and turns.
void ejecutarJuego(UNO_Game &game, bool &amountSelected, int &currentPlayer, string &currentInput, bool &namesCompleted)
{
    VisualZone zone = getVisualZone();
    bool enMenu = true;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (enMenu)
        {
            MenuResult res = MENU_NONE;

            // Bucle interno para el menú: se repite until the player elija JUGAR or CERRAR
            while (enMenu && !WindowShouldClose())
            {
                BeginDrawing();
                ClearBackground(RAYWHITE);

                res = MenuPrincipal();

                if (res == MENU_PLAY)
                {
                    enMenu = false;
                }
                else if (res == MENU_EXIT)
                {
                    CloseWindow();
                    return;
                }

                EndDrawing();
            }

            continue; // vuelve al inicio del bucle principal para evitar seguir with the game aún
        }

        else
        {

            // Pantalla para seleccionar cantidad of players
            if (!amountSelected)
            {
                seleccionarCantidadJugadores(game, amountSelected);
            }
            // Pantalla para ingresar names of the players
            else if (!namesCompleted)
            {
                capturarNombresEnLaVentana(game, currentPlayer, currentInput, namesCompleted);
            }
            else
            {
                // Muestra el name of the player in turn
                DrawText(TextFormat("Turno de: %s", game.players[game.currentTurn].name.c_str()), 800, 50, 30, RED);

                // Si aún no se han repartido cartas, inicializa the game
                if (game.gameState == WaitingPlayers)
                {
                    initializeDeck(game);
                    barajarMazo(game);
                    repartirCartas(game);
                    game.gameState = NormalTurn;
                    game.cardInPlay = cartaInicial(game);
                    actualizarVisibilidadCartas(game);
                }

                // Dibuja las cartas of each player in their position
                for (int i = 0; i < game.playerCount; i++)
                {
                    int y = (i < 2) ? 100 : 700;       // Players 0 and 1 arriba, 2 and 3 abajo
                    int x = (i % 2 == 0) ? 100 : 1100; // Players pares izquierda, impares derecha

                    bool mostrar = (i == game.currentTurn);
                    dibujarCartasJugador(game.players[i], x, y, mostrar);
                }

                // Dibuja el mazo and the zone of descarte
                DrawRectangleRec(zone.deckZone, DARKGRAY);
                DrawText("MAZO", zone.deckZone.x + 10, zone.deckZone.y + 60, 20, WHITE);

                dibujarZonaDescarte(game.cardInPlay, zone.xDiscard, zone.yDiscard);

                Player &player = game.players[game.currentTurn];

                int y = (game.currentTurn < 2) ? 100 : 700;
                int x = (game.currentTurn % 2 == 0) ? 100 : 1100;

                // Contar cartas visibles of the actual player
                int cartasVisibles = 0;
                for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
                {
                    if (!player.hand[i].color.empty())
                    {
                        cartasVisibles++;
                    }
                }

                // Mostrar "¡UNO!" if solo tiene una carta
                if (cartasVisibles == 1)
                {
                    DrawText("¡UNO!", x + 200, y - 40, 40, RED);
                }

                // Permite jugar una carta with double click if es válida
                for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
                {
                    Card &carta = player.hand[i];

                    if (!carta.visible || carta.color.empty())
                        continue;

                    int cartaX = x + i * SPACING_X;
                    int cartaY = y;

                    Rectangle rect = {(float)cartaX, (float)cartaY, (float)CARD_WIDTH, (float)CARD_HEIGHT};

                    // Si el player hace double click on a valid card
                    if (cartaTuvoDobleClick(rect))
                    {
                        // Validación especial for +4
                        if (carta.type == PlusFour && hasCardOfColor(player, game.cardInPlay.color))
                        {
                            // No se puede jugar +4 if tiene cartas del color actual
                            // (Opcional: podrías mostrar un mensaje with temporizador)
                            continue;
                        }

                        if (sePuedeJugar(game.cardInPlay, carta))
                        {
                            // Se juega la carta
                            game.cardInPlay = carta;

                            // Según the type of carta, activa the minijuego corresponding
                            switch (carta.type)
                            {
                            case PlusTwo:
                                game.gameState = MinigameActive;
                                game.activeMinigame = FindIntruder;
                                game.pendingCard = carta;
                                startIntruderMinigame();
                                break;

                            case PlusFour:
                                game.gameState = MinigameActive;
                                game.activeMinigame = WordOrder;
                                game.pendingCard = carta;
                                startWordOrderMinigame();
                                break;

                            case ChangeColor:
                                game.gameState = MinigameActive;
                                game.activeMinigame = WordOrder;
                                game.pendingCard = carta;
                                startWordOrderMinigame();
                                break;

                            case Skip:
                                game.gameState = MinigameActive;
                                game.activeMinigame = Speed;
                                game.pendingCard = carta;
                                startSpeedMinigame();
                                break;

                            case Reverse:
                                game.gameState = MinigameActive;
                                game.activeMinigame = Math;
                                game.pendingCard = carta;
                                minigameReverse(game);
                                break;

                            default:
                                break;
                            }

                            carta = Card{}; // Elimina la carta jugada de la mano

                            if (carta.type == Number)
                            {
                                avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
                                actualizarVisibilidadCartas(game);
                            }

                            break;
                        }
                    }
                }

                // Permite robar carta if no tiene jugable and hace clic en el mazo
                if (CheckCollisionPointRec(GetMousePosition(), zone.deckZone) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (!tieneCartaJugable(player, game.cardInPlay))
                    {
                        // Robar carta simple
                        Card cartaRobada = robarCartaSimple(game);

                        if (!cartaRobada.color.empty())
                        {
                            // Buscar espacio libre en la mano para agregar la carta robada
                            bool agregada = false;
                            for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
                            {
                                if (player.hand[i].color.empty())
                                {
                                    player.hand[i] = cartaRobada;
                                    agregada = true;
                                    break;
                                }
                            }

                            // Si la carta robada es jugable
                            if (sePuedeJugar(game.cardInPlay, cartaRobada))
                            {
                                // Jugar la carta robada (como si fuera double click)
                                game.cardInPlay = cartaRobada;

                                // Activar the minijuego corresponding
                                switch (cartaRobada.type)
                                {
                                case PlusTwo:
                                    game.gameState = MinigameActive;
                                    game.activeMinigame = FindIntruder;
                                    game.pendingCard = cartaRobada;
                                    startIntruderMinigame();
                                    break;

                                case PlusFour:
                                case ChangeColor:
                                    game.gameState = MinigameActive;
                                    game.activeMinigame = WordOrder;
                                    game.pendingCard = cartaRobada;
                                    startWordOrderMinigame(); 
                                    break;

                                case Skip:
                                    game.gameState = MinigameActive;
                                    game.activeMinigame = Speed;
                                    game.pendingCard = cartaRobada;
                                    startSpeedMinigame();
                                    break;

                                case Reverse:
                                    game.gameState = MinigameActive;
                                    game.activeMinigame = Math;
                                    game.pendingCard = cartaRobada;
                                    break;

                                default:
                                    break;
                                }

                                // Elimina la carta jugada de la mano
                                for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
                                {
                                    if (player.hand[i].color == cartaRobada.color &&
                                        player.hand[i].type == cartaRobada.type &&
                                        player.hand[i].value == cartaRobada.value)
                                    {
                                        player.hand[i] = Card{};
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                // Carta no jugable, turno termina and se actualizan cartas visibles
                                avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
                                actualizarVisibilidadCartas(game);
                            }
                        }
                    }
                }

                // Chequeo de ganador
                if (game.gameState == NormalTurn)
                {
                    for (int i = 0; i < game.playerCount; i++)
                    {
                        if (jugadorSinCartas(game.players[i]))
                        {
                            DrawText(TextFormat("¡%s ha ganado la partida!", game.players[i].name.c_str()), 500, 500, 40, GREEN);

                            game.players[i].gamesWon++;
                            for (int j = 0; j < game.playerCount; j++)
                            {
                                if (j != i)
                                    game.players[j].gamesLost++;
                            }

                            guardarEstadisticas(game, "estadisticas.txt");

                            EndDrawing();
                            continue; // Salta el resto del ciclo para no sobreescribir the screen
                        }
                    }
                }

                // BLOQUE PARA TODOS LOS MINIJUEGOS
                if (game.gameState == MinigameActive)
                {
                    bool minigameTerminado = false;
                    bool minigameGanado = false;

                    // Ejecutar the minijuego corresponding
                    switch (game.activeMinigame)
                    {
                    case WordOrder:
                        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
                        updateWordOrderMinigame(game.players[game.currentTurn]);
                        minigameTerminado = wordOrderMinigameFinished();
                        if (minigameTerminado)
                            minigameGanado = wordOrderMinigameWon();
                        break;

                    case FindIntruder:
                        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
                        updateIntruderMinigame(game.players[game.currentTurn]);
                        minigameTerminado = intruderMinigameFinished();
                        if (minigameTerminado)
                            minigameGanado = intruderMinigameWon();
                        break;

                    case Speed:
                        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
                        updateSpeedMinigame(game.players[game.currentTurn]);
                        minigameTerminado = speedMinigameFinished();
                        if (minigameTerminado)
                            minigameGanado = speedMinigameWon();
                        break;

                    case Math:
                        // El minijuego matemático se maneja de forma diferente (ventana separada)
                        if (playMathMinigame())
                        {
                            minigameGanado = true;
                            minigameTerminado = true;
                        }
                        else
                        {
                            minigameGanado = false;
                            minigameTerminado = true;
                        }
                        break;

                    default:
                        break;
                    }

                    // Manejar the result of the minijuego
                    if (minigameTerminado)
                    {
                        if (minigameGanado)
                        {
                            // Ganó the minijuego: the carta se juega normalmente
                            game.cardInPlay = game.pendingCard;

                            // Aplicar efectos específicos según the type of carta
                            switch (game.pendingCard.type)
                            {
                            case PlusTwo:
                            {
                                // Si gana +2, the following player roba 2 cartas
                                int jugadorPenalizado = (game.currentTurn + game.direction + game.playerCount) % game.playerCount;
                                for (int i = 0; i < 2; i++)
                                {
                                    Card cartaRobada = robarCartaSimple(game);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
                                        {
                                            if (game.players[jugadorPenalizado].hand[j].color.empty())
                                            {
                                                game.players[jugadorPenalizado].hand[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
                            }
                            break;

                            case PlusFour:
                            {
                                // Si gana +4, the following player roba 4 cartas
                                int jugadorPenalizado = (game.currentTurn + game.direction + game.playerCount) % game.playerCount;
                                for (int i = 0; i < 4; i++)
                                {
                                    Card cartaRobada = robarCartaSimple(game);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
                                        {
                                            if (game.players[jugadorPenalizado].hand[j].color.empty())
                                            {
                                                game.players[jugadorPenalizado].hand[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
                            }
                            break;

                            case Skip:
                                aplicarMasDosConMinijuego(game);
                                // El avance of turno ya está en aplicarMasDosConMinijuego
                                break;

                            case Reverse:
                                aplicarCambioDireccion(game);
                                avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
                                break;

                            default:
                                break;
                            }

                            // Para cartas of change color, asignar color aleatorio
                            if (game.pendingCard.type == PlusFour || game.pendingCard.type == ChangeColor)
                                assignRandomColorPlusTwoPlusFour(game);
                        }
                        else
                        {
                            // Perdió the minijuego: the carta se juega pero hay penalización
                            game.cardInPlay = game.pendingCard;

                            // Aplicar efectos específicos según the type of carta
                            switch (game.pendingCard.type)
                            {
                            case PlusTwo:
                            {
                                // Si pierde +2, the following player roba 2 cartas Y the actual player roba 2 como penalización
                                int jugadorPenalizado = (game.currentTurn + game.direction + game.playerCount) % game.playerCount;
                                // The following player roba 2 cartas
                                for (int i = 0; i < 2; i++)
                                {
                                    Card cartaRobada = robarCartaSimple(game);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
                                        {
                                            if (game.players[jugadorPenalizado].hand[j].color.empty())
                                            {
                                                game.players[jugadorPenalizado].hand[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                // The actual player roba 2 cartas como penalización
                                for (int i = 0; i < 2; i++)
                                {
                                    Card cartaRobada = robarCartaSimple(game);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
                                        {
                                            if (game.players[j].hand[j].color.empty())
                                            {
                                                game.players[j].hand[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
                            }
                            break;

                            case PlusFour:
                            {
                                // Si pierde +4, the following player roba 4 cartas Y the actual player roba 4 como penalización
                                int jugadorPenalizado = (game.currentTurn + game.direction + game.playerCount) % game.playerCount;
                                // The following player roba 4 cartas
                                for (int i = 0; i < 4; i++)
                                {
                                    Card cartaRobada = robarCartaSimple(game);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
                                        {
                                            if (game.players[jugadorPenalizado].hand[j].color.empty())
                                            {
                                                game.players[jugadorPenalizado].hand[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                // The actual player roba 4 cartas como penalización
                                for (int i = 0; i < 4; i++)
                                {
                                    Card cartaRobada = robarCartaSimple(game);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
                                        {
                                            if (game.players[j].hand[j].color.empty())
                                            {
                                                game.players[j].hand[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
                            }
                            break;

                            case Skip:
                                aplicarMasDosConMinijuego(game);
                                // El avance of turno ya está en aplicarMasDosConMinijuego
                                break;

                            case Reverse:
                                aplicarCambioDireccion(game);
                                avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
                                break;

                            default:
                                break;
                            }

                            // Para cartas of change color, asignar color aleatorio
                            if (game.pendingCard.type == PlusFour || game.pendingCard.type == ChangeColor)
                                assignRandomColorPlusTwoPlusFour(game);
                        }

                        int jugadorQueJugo = game.currentTurn;

                        // Limpiar la carta jugada de the mano of the actual player
                        for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
                        {
                            if (game.players[game.currentTurn].hand[i].color == game.pendingCard.color &&
                                game.players[game.currentTurn].hand[i].type == game.pendingCard.type &&
                                game.players[game.currentTurn].hand[i].value == game.pendingCard.value)
                            {
                                game.players[jugadorQueJugo].hand[i] = Card{};
                                break;
                            }
                        }

                        // Volver al juego normal and avanzar turno if no es bloqueo (ya lo hace aplicarMasDosConMinijuego)
                        game.gameState = NormalTurn;
                        game.activeMinigame = None;

                        // Solo avanzar turno if no es una carta que ya lo hizo dentro of the case
                        if (game.pendingCard.type != PlusTwo &&
                            game.pendingCard.type != PlusFour &&
                            game.pendingCard.type != Skip)
                        {
                            avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
                        }

                        actualizarVisibilidadCartas(game);
                    }

                    EndDrawing();
                    continue; // Salta el resto del ciclo para no sobreescribir the screen
                }
            }

            EndDrawing();
        }
    }
}

// Dibuja todas las cartas of the mano of a player in the screen. //a ver si funciona
void dibujarCartasJugador(const Player &player, int xInicial, int yInicial, bool mostrarTodas)
{
    int espacioX = 100;    // espacio between cards
    int maxCartasFila = 7; // máximo cartas por fila

    Vector2 mouse = GetMousePosition();

    for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
    {
        if (!mostrarTodas && !player.hand[i].visible)
            continue;

        const Card &carta = player.hand[i];
        if (carta.color.empty())
            continue;

        // Calcula la fila and la columna dentro of the fila
        int fila = i / maxCartasFila; // 0 for fila 1, 1 for fila 2
        int columna = i % maxCartasFila;

        int x = xInicial + columna * espacioX;
        int y = yInicial + fila * (CARD_HEIGHT + 10); // 10 pixeles of separation between rows

        Rectangle rectCarta = {(float)x, (float)y, (float)CARD_WIDTH, (float)CARD_HEIGHT};

        // Detectar hover
        bool mouseSobreCarta = CheckCollisionPointRec(mouse, rectCarta);
        Color colorFondo = mouseSobreCarta ? LIGHTGRAY : GRAY;

        DrawRectangleRec(rectCarta, colorFondo);
        DrawRectangleLinesEx(rectCarta, 2, BLACK);

        // Color del texto según el color of the carta
        Color colorTexto = BLACK;
        if (carta.color == "rojo")
            colorTexto = RED;
        else if (carta.color == "amarillo")
            colorTexto = YELLOW;
        else if (carta.color == "verde")
            colorTexto = GREEN;
        else if (carta.color == "azul")
            colorTexto = BLUE;
        else if (carta.color == "negro")
            colorTexto = DARKGRAY;

        // Texto of the carta según tipo
        string textoCarta;
        switch (carta.type)
        {
        case Number:
            textoCarta = to_string(carta.value);
            break;
        case PlusTwo:
            textoCarta = "+2";
            break;
        case PlusFour:
            textoCarta = "+4️";
            break;
        case ChangeColor:
            textoCarta = "Color";
            break;
        case Reverse:
            textoCarta = "Reverse";
            break;
        case Skip:
            textoCarta = "Skip";
            break;
        default:
            textoCarta = "?";
            break;
        }

        int anchoTexto = MeasureText(textoCarta.c_str(), 20);
        int textoX = x + (CARD_WIDTH / 2) - (anchoTexto / 2);
        int textoY = y + (CARD_HEIGHT / 2) - 10;

        DrawText(textoCarta.c_str(), textoX, textoY, 20, colorTexto);
    }
}

// Imprime the mazo in console for debugging.
void imprimirMazo(const UNO_Game &game)
{
    for (int i = 0; i < game.cardsInDeck; i++)
    {
        cout << "[" << game.deck[i].color << ", ";
        if (game.deck[i].type == Number)
            cout << game.deck[i].value;
        else
            cout << "Especial";
        cout << "] ";
    }
    cout << endl;
}

// Verifica if a carta se puede jugar over the actual card in game according to the original rules of UNO.
bool sePuedeJugar(Card actual, Card elegida)
{
    // Comodines siempre se pueden jugar
    if (elegida.type == ChangeColor || elegida.type == PlusFour)
        return true;

    // Coincide color
    if (elegida.color == actual.color)
        return true;

    // Coincide valor numérico
    if (elegida.type == Number && actual.type == Number && elegida.value == actual.value)
        return true;

    // Coincide tipo especial Y color (ej. Bloqueo azul sobre Bloqueo azul)
    if (elegida.type == actual.type && elegida.color == actual.color)
        return true;

    return false;
}

// Detecta if hubo doble clic sobre una carta (para jugarla).
bool cartaTuvoDobleClick(const Rectangle &rect)
{
    static float tiempoUltimoClick = 0;
    static int clicks = 0;

    if (CheckCollisionPointRec(GetMousePosition(), rect))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            float tiempoActual = GetTime();
            if (tiempoActual - tiempoUltimoClick < 0.3f)
            { // Si el segundo clic es rápido, cuenta como doble clic
                clicks++;
                if (clicks == 2)
                {
                    clicks = 0;
                    return true;
                }
            }
            else
            {
                clicks = 1;
            }
            tiempoUltimoClick = tiempoActual;
        }
    }
    return false;
}

// Dibuja the carta in the zone of descarte.
void dibujarZonaDescarte(const Card &carta, int x, int y)
{
    if (carta.color.empty())
        return;

    DrawRectangle(x, y, 80, 120, LIGHTGRAY);
    DrawRectangleLines(x, y, 80, 120, BLACK);

    // Selecciona color del texto según la carta
    Color colorTexto = BLACK;
    if (carta.color == "rojo")
        colorTexto = RED;
    else if (carta.color == "amarillo")
        colorTexto = YELLOW;
    else if (carta.color == "verde")
        colorTexto = GREEN;
    else if (carta.color == "azul")
        colorTexto = BLUE;
    else if (carta.color == "negro")
        colorTexto = DARKGRAY;

    // Traduce el tipo of carta a texto
    string textoCarta;
    switch (carta.type)
    {
    case Number:
        textoCarta = to_string(carta.value);
        break;
    case PlusTwo:
        textoCarta = "+2";
        break;
    case PlusFour:
        textoCarta = "+4";
        break;
    case Reverse:
        textoCarta = "Rev";
        break;
    case Skip:
        textoCarta = "Skip";
        break;
    case ChangeColor:
        textoCarta = "Color?";
        break;
    default:
        textoCarta = "?";
        break;
    }

    // Dibuja el texto centrado en la carta of descarte
    DrawText(textoCarta.c_str(), x + 30, y + 50, 30, colorTexto);
}

// Avanza the turno to the following player, respecting the direction of the game.
void avanzarTurno(int &currentTurn, int direction, int totalPlayers, UNO_Game &game)
{
    currentTurn = (currentTurn + direction + totalPlayers) % totalPlayers;
    game.currentTurn = currentTurn;
    actualizarVisibilidadCartas(game);
}

// Devuelve the structure with the positions of the zone visual of the mazo and descarte.
VisualZone getVisualZone()
{
    VisualZone zone;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Posición X del mazo: centro menos media carta menos la mitad del espacio
    zone.deckZone.width = CARD_WIDTH;
    zone.deckZone.height = CARD_HEIGHT;
    zone.deckZone.x = screenWidth / 2 - CARD_WIDTH - SPACING_BETWEEN_DECK_DISCARD / 2;
    zone.deckZone.y = screenHeight / 2 - CARD_HEIGHT / 2;

    // Posición X del descarte: centro más medio espacio
    zone.xDiscard = screenWidth / 2 + SPACING_BETWEEN_DECK_DISCARD / 2;
    zone.yDiscard = screenHeight / 2 - CARD_HEIGHT / 2;

    return zone;
}


// Guarda o actualiza las estadísticas acumuladas of TODOS the players that have played UNO.
// El archivo mantiene un registro único por player, sumando partidas jugadas, ganadas and minijuegos ganados.
void guardarEstadisticas(const UNO_Game &game, const string &EstadisticaArchivo)
{
    struct EstadisticasJugador {
        string nombre;
        int partidas_jugadas = 0;
        int partidas_ganadas = 0;
        int minijuegos_ganados = 0;
    };

    EstadisticasJugador estadisticas[200];
    int totalPlayers = 0;

    // Leer estadísticas existentes using fstream
    ifstream fin(EstadisticaArchivo);
    string linea;
    EstadisticasJugador temp;
    int estado = 0;
    while (getline(fin, linea)) {
        if (linea.rfind("Player: ", 0) == 0) {
            temp.nombre = linea.substr(9);
            estado = 1;
        }
        else if (linea.rfind("  Partidas jugadas: ", 0) == 0 && estado == 1) {
            temp.partidas_jugadas = stoi(linea.substr(20));
            estado = 2;
        }
        else if (linea.rfind("  Partidas ganadas: ", 0) == 0 && estado == 2) {
            temp.partidas_ganadas = stoi(linea.substr(20));
            estado = 3;
        }
        else if (linea.rfind("  Minijuegos ganados: ", 0) == 0 && estado == 3) {
            temp.minijuegos_ganados = stoi(linea.substr(23));
            estado = 4;
        }
        else if (linea.rfind("-----------------------------------", 0) == 0 && estado == 4) {
            if (!temp.nombre.empty() && totalPlayers < 200) {
                estadisticas[totalPlayers++] = temp;
            }
            temp = EstadisticasJugador();
            estado = 0;
        }
    }
    fin.close();

    // Actualizar estadísticas con los datos of the actual game
    for (int i = 0; i < game.playerCount; ++i) {
        const Player &jugador = game.players[i];
        bool encontrado = false;
        for (int j = 0; j < totalPlayers; ++j) {
            if (estadisticas[j].nombre == jugador.name) {
                estadisticas[j].partidas_jugadas += 1;
                estadisticas[j].partidas_ganadas += jugador.gamesWon;
                estadisticas[j].minijuegos_ganados += jugador.minigamesWon;
                encontrado = true;
                break;
            }
        }
        if (!encontrado && totalPlayers < 200) {
            estadisticas[totalPlayers].nombre = jugador.name;
            estadisticas[totalPlayers].partidas_jugadas = 1;
            estadisticas[totalPlayers].partidas_ganadas = jugador.gamesWon;
            estadisticas[totalPlayers].minijuegos_ganados = jugador.minigamesWon;
            totalPlayers++;
        }
    }

    // Escribir todas las estadísticas using fstream
    ofstream fout(EstadisticaArchivo, ios::trunc);
    if (!fout.is_open()) {
        cout << "Error al abrir el archivo para guardar estadísticas." << endl;
        return;
    }
    fout << "----- Estadísticas acumuladas of UNO -----\n";
    for (int i = 0; i < totalPlayers; ++i) {
        fout << "Player: " << estadisticas[i].nombre << "\n";
        fout << "  Partidas jugadas: " << estadisticas[i].partidas_jugadas << "\n";
        fout << "  Partidas ganadas: " << estadisticas[i].partidas_ganadas << "\n";
        fout << "  Minijuegos ganados: " << estadisticas[i].minijuegos_ganados << "\n";
        fout << "-----------------------------------\n";
    }
    fout.close();
}

// MINIJUEGO DE REFLEJOS PARA CARTA +2

// Ejecuta un minijuego de reflejos: the player must press a random key quickly.
// Si gana, el rival roba 2 cartas. Si pierde, el player that lanzó el +2 roba 2 cartas.
bool minigameReverse(UNO_Game &game) {
    int number = GetRandomValue(1, 10);
    bool answer = false; // true = even, false = odd
    bool selectionMade = false;
    bool result = false;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Rectangle evenButton = { (float)(screenWidth/2 - 160), (float)(screenHeight/2), 120, 60 };
    Rectangle oddButton = { (float)(screenWidth/2 + 40), (float)(screenHeight/2), 120, 60 };

    while (!selectionMade && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("MINIGAME: Is the hidden number EVEN or ODD?", screenWidth/2 - 320, screenHeight/2 - 100, 30, DARKGRAY);
        DrawRectangleRec(evenButton, LIGHTGRAY);
        DrawRectangleRec(oddButton, LIGHTGRAY);
        DrawRectangleLinesEx(evenButton, 3, BLUE);
        DrawRectangleLinesEx(oddButton, 3, RED);
        DrawText("EVEN", evenButton.x + 30, evenButton.y + 15, 30, BLUE);
        DrawText("ODD", oddButton.x + 20, oddButton.y + 15, 30, RED);

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, evenButton)) {
                answer = true;
                selectionMade = true;
            } else if (CheckCollisionPointRec(mouse, oddButton)) {
                answer = false;
                selectionMade = true;
            }
        }
    }

    bool isEven = (number % 2 == 0);
    result = (answer == isEven);

    // Mostrar resultado
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (result) {
        DrawText(TextFormat("Correct! It was %d. Direction changes.", number), screenWidth/2 - 250, screenHeight/2, 30, GREEN);
    } else {
        DrawText(TextFormat("Incorrect! It was %d. Draw 2 cards.", number), screenWidth/2 - 250, screenHeight/2, 30, RED);
    }
    EndDrawing();
    WaitTime(1.5);

    return result;
}

// Aplica the effect of the carta +2 with the minijuego of reflejos.
// Si the player gana the minijuego, the following player roba 2 cartas.
// Si pierde, the actual player (that threw the +2) roba 2 cartas as punishment.
// Then avanza the turno respetando the direction.
void aplicarMasDosConMinijuego(UNO_Game &game)
{
    int currentPlayer = game.currentTurn;
    int siguienteJugador = (currentPlayer + game.direction + game.playerCount) % game.playerCount;

    bool ganoMinijuego = minigameReverse(game);

    if (ganoMinijuego)
    {
        // The following player roba 2 cartas
        for (int i = 0; i < 2; i++)
        {
            Card cartaRobada = robarCartaValida(game);
            if (!cartaRobada.color.empty())
            {
                for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
                {
                    if (game.players[siguienteJugador].hand[j].color.empty())
                    {
                        game.players[siguienteJugador].hand[j] = cartaRobada;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // The actual player roba 2 cartas as punishment
        for (int i = 0; i < 2; i++)
        {
            Card cartaRobada = robarCartaValida(game);
            if (!cartaRobada.color.empty())
            {
                for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
                {
                    if (game.players[currentPlayer].hand[j].color.empty())
                    {
                        game.players[currentPlayer].hand[j] = cartaRobada;
                        break;
                    }
                }
            }
        }
    }
}

// Aplica the effect of the carta +4
void aplicarMasCuatro(UNO_Game &game)
{
    int jugadorPenalizado = (game.currentTurn + game.direction + game.playerCount) % game.playerCount;

    for (int i = 0; i < 4; i++)
    {
        Card cartaRobada = robarCartaValida(game);
        if (!cartaRobada.color.empty())
        {
            for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++)
            {
                if (game.players[jugadorPenalizado].hand[j].color.empty())
                {
                    game.players[jugadorPenalizado].hand[j] = cartaRobada;
                    break;
                }
            }
        }
    }
}

// Aplica the effect of change color
void aplicarCambioColor(UNO_Game &game)
{
    // The color se selecciona aleatoriamente después of the minijuego
    // Esta función se llama después of that the minijuego determine the color
}

// Aplica the effect of the carta bloqueo (salta turno)
void aplicarBloqueo(UNO_Game &game)
{
    // The bloqueo salta the turno of the following player respetando the direction
    // Avanza una vez for the following player, and otra vez for saltarlo
    avanzarTurno(game.currentTurn, game.direction, game.playerCount, game);
}

// Aplica the effect of change direction
void aplicarCambioDireccion(UNO_Game &game)
{
    game.direction *= -1; // Invierte the direction of the game
}

// Función para verificar if a player no tiene cartas
bool jugadorSinCartas(const Player &jugador)
{
    for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
    {
        if (!jugador.hand[i].color.empty())
        {
            return false; // Tiene al menos una carta real
        }
    }
    return true; // No tiene cartas
}

// Permite al player seleccionar el color tras jugar Cambio de color or +4
void assignRandomColorPlusTwoPlusFour(UNO_Game &game)
{
    // Solo aplica for +4 and change color
    if (game.pendingCard.type != PlusFour && game.pendingCard.type != ChangeColor)
        return;

    bool colorSeleccionado = false;
    string colores[] = {"rojo", "amarillo", "verde", "azul"};
    Color coloresRaylib[] = {RED, YELLOW, GREEN, BLUE};

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Rectangle botones[4];
    int anchoBoton = 120, altoBoton = 120;
    int espacio = 40;
    int totalAncho = 4 * anchoBoton + 3 * espacio;
    int xInicial = (screenWidth - totalAncho) / 2;
    int yBoton = screenHeight / 2 - altoBoton / 2;

    for (int i = 0; i < 4; i++)
    {
        botones[i] = { (float)(xInicial + i * (anchoBoton + espacio)), (float)yBoton, (float)anchoBoton, (float)altoBoton };
    }

    while (!colorSeleccionado && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Selecciona un color:", screenWidth / 2 - 180, yBoton - 80, 40, DARKGRAY);

        for (int i = 0; i < 4; i++)
        {
            DrawRectangleRec(botones[i], coloresRaylib[i]);
            DrawRectangleLinesEx(botones[i], 4, BLACK);
            DrawText(colores[i].c_str(), botones[i].x + 25, botones[i].y + 40, 30, BLACK);
        }

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 4; i++)
            {
                if (CheckCollisionPointRec(mouse, botones[i]))
                {
                    game.cardInPlay.color = colores[i];
                    colorSeleccionado = true;
                    break;
                }
            }
        }
    }
}

// Verifica if the player has at least one card of the specified color in their mano.
// Devuelve true if finds a card of the objective color, false if no.
bool hasCardOfColor(Player &player, const string &targetColor)
{
    for (int i = 0; i < MAX_CARDS_PER_PLAYER; i++)
    {
        if (!player.hand[i].color.empty() && player.hand[i].color == targetColor)
            return true;
    }
    return false;
}

void runReverseMinigameWithPenalty(UNO_Game &game) {
    bool won = minigameReverse(game);
    if (won) {
        applyReverse(game);
        advanceTurn(game.currentTurn, game.direction, game.playerCount, game);
    } else {
        // Penalty: draw 2 cards and DO NOT change direction
        for (int i = 0; i < 2; i++) {
            Card drawnCard = robarCartaSimple(game);
            if (!drawnCard.color.empty()) {
                for (int j = 0; j < MAX_CARDS_PER_PLAYER; j++) {
                    if (game.players[game.currentTurn].hand[j].color.empty()) {
                        game.players[game.currentTurn].hand[j] = drawnCard;
                        break;
                    }
                }
            }
        }
        advanceTurn(game.currentTurn, game.direction, game.playerCount, game);
    }
}