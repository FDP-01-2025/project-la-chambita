// En este archivo .h solo se DECLARAN o NOMBRAN variables, estructuras y funciones, no se implementan.

// Evita que el archivo se incluya más de una vez en el mismo proyecto.
#ifndef UNO_GAME_H
#define UNO_GAME_H

#include <string>
#include <iostream>
#include "raylib.h"

using namespace std;

// Constantes para los límites y dimensiones del juego
const int MAX_PLAYERS = 4;           // Número máximo de jugadores permitidos
const int MAX_CARDS_PER_PLAYER = 30; // Máximo de cartas que puede tener un jugador en la mano
const int MAX_DECK = 108;            // Número total de cartas en el mazo
const int CARD_WIDTH = 80;           // Ancho de cada carta (en píxeles)
const int CARD_HEIGHT = 120;         // Alto de cada carta (en píxeles)
const int SPACING_X = 100;           // Espaciado horizontal entre cartas
const int SPACING_Y = 100;           // Espaciado vertical entre cartas
const int START_X = 100;             // Posición inicial X para dibujar cartas
const int START_Y = 100;             // Posición inicial Y para dibujar cartas
const int SPACING_BETWEEN_DECK_DISCARD = 40; // espacio horizontal entre ellos

// Tipos de cartas posibles en el juego UNO
enum CardType
{
    Number,           // Carta con valor numérico
    PlusTwo,          // Carta +2: el siguiente jugador roba dos cartas
    PlusFour,         // Carta +4: el siguiente jugador roba cuatro cartas y cambia color
    ChangeColor,      // Cambia el color actual del juego
    Reverse,          // Invierte el sentido de juego
    Skip,
};

// Estados posibles de la partida
enum GameState
{
    WaitingPlayers, // Esperando a que se unan los jugadores
    DealingCards,   // Repartiendo cartas a los jugadores
    NormalTurn,     // Turno normal de juego
    MinigameActive, // Hay un minijuego en curso
    GameFinished    // El juego ha finalizado
};

// Tipos de minijuegos disponibles
enum MinigameType
{
    None,
    WordOrder,      // Para +4 y Cambio_color
    FindIntruder,   // Para +2
    Speed,          // Para Bloqueo
    Math,           // Para Cambio_direccion
    Reflexes        // Para +2 (alternativo)
};

// Estructura que representa una carta del juego UNO
struct Card
{
    string color;           // Color de la carta (rojo, azul, verde, amarillo, etc.)
    CardType type;          // Tipo de carta (numérica, +2, +4, etc.)
    int value;              // Valor numérico de la carta (si aplica)
    string minigameName;    // Nombre del minijuego asociado (si aplica)
    string description;     // Descripción de la carta o minijuego
    bool visible;           // Indica si la carta está visible o volteada
};

// Estructura que representa a un jugador
struct Player
{
    string name;                        // Nombre del jugador
    Card hand[MAX_CARDS_PER_PLAYER];    // Cartas que tiene el jugador en la mano
    int minigamesWon;                   // Cantidad de minijuegos ganados por el jugador
    int gamesWon;                       // Cantidad de partidas ganadas
    int gamesLost;                      // Cantidad de partidas perdidas
    bool isTurn;                        // Indica si es el turno de este jugador
    bool isNew;                         // Indica si el jugador es nuevo en la partida
};

// Estructura principal que representa el estado completo del juego UNO
struct UNO_Game
{
    Card deck[MAX_DECK]; // Mazo de cartas disponibles para robar
    int cardsInDeck;     // Cantidad de cartas restantes en el mazo

    Card discard[MAX_DECK]; // Pila de cartas descartadas
    int cardsInDiscard;     // Cantidad de cartas en el descarte

    Player players[MAX_PLAYERS]; // Lista de jugadores en la partida
    int playerCount;            // Cantidad real de jugadores en la partida

    int currentTurn;              // Índice del jugador que tiene el turno actual
    int direction;                // Dirección del turno: 1 (normal) o -1 (invertido)
    GameState gameState;          // Estado actual de la partida

    Card cardInPlay; // Carta que está actualmente en juego (en la pila de descarte)

    string forcedColor; // Color forzado por una carta especial (ej: +4 o cambio de color)

    Card pendingCard;           // Carta pendiente de jugar en un minijuego o acción especial
    MinigameType activeMinigame; // Tipo de minijuego actualmente activo
};

enum MenuResult
{
    MENU_NONE,
    MENU_PLAY,
    MENU_EXIT
};

// Estructura para definir las zonas visuales donde se dibujan las cartas en la interfaz gráfica
struct VisualZone
{
    Rectangle deckZone; // Área donde se dibuja el mazo de cartas
    int xDiscard;       // Coordenada X para dibujar la pila de descarte
    int yDiscard;       // Coordenada Y para dibujar la pila de descarte
};

// Declaraciones de funciones (solo se indican, no se implementan aquí)

// Inicialización y preparación del juego
UNO_Game createUNOGame();                                                                                                  // Crea e inicializa una nueva partida de UNO
void initStateVariables(bool &amountSelected, int &currentPlayer, string &currentInput, bool &namesCompleted);             // Inicializa variables de estado
void initializeDeck(UNO_Game &game);                                                                                       // Llena el mazo con todas las cartas del juego
void shuffleDeck(UNO_Game &game);                                                                                          // Mezcla aleatoriamente las cartas del mazo

// Interacción inicial con el jugador
void selectPlayerCount(UNO_Game &game, bool &amountSelected);                                                              // Permite seleccionar la cantidad de jugadores
void captureNamesInWindow(UNO_Game &game, int &currentPlayer, string &currentInput, bool &namesCompleted);                 // Captura los nombres de los jugadores

// comienzo del juego
void dealCards(UNO_Game &game);
Card drawValidCard(UNO_Game &game);
void updateCardVisibility(UNO_Game &game);

// lógica y turnos de jugabilidad
MenuResult MainMenu(); // menu principal
void runGame(UNO_Game &game, bool &amountSelected, int &currentPlayer, string &currentInput, bool &namesCompleted);
void drawPlayerCards(const Player &player, int xStart, int yStart, bool showAll);
void drawDiscardZone(const Card &card, int x, int y);
bool playerDrawsIfClick(const Rectangle &deckZone, UNO_Game &game, int player);
bool canPlay(Card current, Card chosen);
bool hasPlayableCard(const Player &player, Card cardInPlay);
void advanceTurn(int &currentPlayer, int direction, int totalPlayers, UNO_Game &game);
bool cardHadDoubleClick(const Rectangle &rect);
void tryDrawAndChangeTurn(UNO_Game &game);
Card initialCard(UNO_Game &game);
void saveStatistics(const UNO_Game &game, const string &statisticsFile);
// chequeo ganador
bool playerHasNoCards(const Player &player);
bool runReflexesMinigame(UNO_Game &game);
void applyPlusTwo(UNO_Game &game, int target);
void applyPlusFour(UNO_Game &game);
void applyChangeColor(UNO_Game &game);
void applySkip(UNO_Game &game);
void applyReverse(UNO_Game &game);
void applyPlusTwoWithMinigame(UNO_Game &game, int penalizedPlayer, int wildcardPlayer);
void applyPlusTwo(UNO_Game &game, int target);
void applyPlusFour(UNO_Game &game);
void applyChangeColor(UNO_Game &game);
void applySkip(UNO_Game &game);
void applyReverse(UNO_Game &game);
bool hasCardOfColor(Player &player, const string &targetColor);
bool minigameReverse(UNO_Game &game);
// funciones de utilidad gráfica
VisualZone getVisualZone();

// estructuras de estadísticas del jugador
struct PlayerStatistics
{
    int gamesPlayed = 0;      // Total de partidas jugadas
    int gamesWon = 0;         // Total de partidas ganadas
    int gamesLost = 0;        // Total de partidas perdidas
    int minigamesPlayed = 0;  // Total de minijuegos jugados
};

#endif
