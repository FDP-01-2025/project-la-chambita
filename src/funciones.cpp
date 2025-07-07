#include "../include/UNO_game.h"

//esta es la funcion para el turno del jugador en el juego UNO

#include <iostream>
#include <string>
using namespace std;

const int MAX_CARDS = 108;
const int MAX_PLAYERS = 4;

struct Card {
    string color;  // rojo, azul, verde, amarillo, comodin
    string value;  // 0–9, +2, skip, reverse, wild, wild+4
};

struct UNO_Game {
    Card deck[MAX_CARDS];
    int deckTop;

    Card discardPile[MAX_CARDS];
    int discardTop;

    Card hands[MAX_PLAYERS][MAX_CARDS];
    int cardCount[MAX_PLAYERS];

    int currentPlayer;
    int totalPlayers;

    int direction = 1;  // 1 = clockwise, -1 = counterclockwise
};

// Función para comprobar si una carta se puede jugar
bool canPlay(Card current, Card chosen) {
    return (chosen.color == current.color || 
            chosen.value == current.value || 
            chosen.color == "wild");
}

// Avanza el turno respetando el sentido del juego
void advanceTurn(UNO_Game &game, int steps = 1) {
    game.currentPlayer = (game.currentPlayer + steps * game.direction + game.totalPlayers) % game.totalPlayers;
}

void playerTurn(UNO_Game &game) {
    int player = game.currentPlayer;
    int &count = game.cardCount[player];
    Card* hand = game.hands[player];
    Card currentCard = game.discardPile[game.discardTop - 1];

    std::cout << "\n--- Player Turn " << player + 1 << " ---\n";
    std::cout << "Card on discard: [" << currentCard.color << " " << currentCard.value << "]\n";

    std::cout << "\nYour hand:\n";
    for (int i = 0; i < count; ++i) {
        std::cout << i + 1 << ". [" << hand[i].color << " " << hand[i].value << "]\n";
    }

    // Verifica si puede jugar
    bool canPlayCard = false;
    for (int i = 0; i < count; ++i) {
        if (canPlay(currentCard, hand[i])) {
            canPlayCard = true;
            break;
        }
    }

    if (canPlayCard) {
        int option;
        do {
            std::cout << "\nChoose the card number to play (0 to draw): ";
            std::cin >> option;

            if (option == 0) {
                if (game.deckTop > 0) {
                    Card drawn = game.deck[--game.deckTop];
                    hand[count++] = drawn;
                    std::cout << "You drew: [" << drawn.color << " " << drawn.value << "]\n";
                } else {
                    std::cout << "The deck is empty. You can't draw.\n";
                }
                break;
            } else if (option > 0 && option <= count) {
                Card chosen = hand[option - 1];
                if (canPlay(currentCard, chosen)) {
                    game.discardPile[game.discardTop++] = chosen;

                    // Eliminar carta jugada
                    for (int j = option - 1; j < count - 1; ++j) {
                        hand[j] = hand[j + 1];
                    }
                    --count;

                    std::cout << "You played: [" << chosen.color << " " << chosen.value << "]\n";

                    // Efectos especiales
                    if (chosen.value == "+2") {
                        advanceTurn(game);
                        int next = game.currentPlayer;
                        for (int i = 0; i < 2 && game.deckTop > 0; ++i) {
                            game.hands[next][game.cardCount[next]++] = game.deck[--game.deckTop];
                        }
                        std::cout << "Player " << next + 1 << " drew 2 cards.\n";
                    }
                    else if (chosen.value == "skip") {
                        std::cout << "Next player loses their turn.\n";
                        advanceTurn(game);
                    }
                    else if (chosen.value == "reverse") {
                        game.direction *= -1;
                        std::cout << "Direction changed!\n";
                    }
                    else if (chosen.value == "wild" || chosen.value == "wild+4") {
                        string newColor;
                        std::cout << "Choose new color (red, green, blue, yellow): ";
                        std::cin >> newColor;
                        game.discardPile[game.discardTop - 1].color = newColor;

                        if (chosen.value == "wild+4") {
                            advanceTurn(game);
                            int next = game.currentPlayer;
                            for (int i = 0; i < 4 && game.deckTop > 0; ++i) {
                                game.hands[next][game.cardCount[next]++] = game.deck[--game.deckTop];
                            }
                            std::cout << "Player " << next + 1 << " drew 4 cards.\n";
                        }
                    }

                    break;  // Terminó el turno
                } else {
                    std::cout << "That card can't be played.\n";
                }
            } else {
                std::cout << "Invalid option.\n";
            }
        } while (true);
    } else {
        std::cout << "You have no valid cards. Drawing one...\n";
        if (game.deckTop > 0) {
            Card drawn = game.deck[--game.deckTop];
            hand[count++] = drawn;
            std::cout << "You drew: [" << drawn.color << " " << drawn.value << "]\n";
        } else {
            std::cout << "The deck is empty.\n";
        }
    }

    // Cambia turno al final
    advanceTurn(game);
}

// Guardar partida
/*
void saveGame(const Player &player, const Card &currentCard, int turn, const std::string &saveFile)
{
    ofstream file("save_game.txt");

    if (!file.is_open())
    {
        std::cout << "Error opening file to save the game." << std::endl;
        return;
    }

    file << "Player: " << player.name << std::endl; // Guarda nombre de jugadores
    file << "Cards in hand: " << game.deck[index++] << std::endl;

    file << "Hand ";
    for (const auto &card : player.hand)
    {
        file << card.color << " " << card.value << "\n";
    }

    file << "Current card: " << currentCard.color << " " << currentCard.value << "\n";
    file << "Current turn: " << game.currentTurn << "\n";

    file.close();
    std::cout << "Game saved in: " << saveFile << std::endl;
}

// Verificar ganador
bool checkWinner(const Player &player)
{
    if (game.deck[index++])
    {
        std::cout << player.name << "You have won the game" << std::endl;

        ofstream file("winners.txt", std::ios::app);
        if (file.is_open())
        {
            file << "Winner: " << player.name << "\n";
            file.close();
        }
        else
        {
            std::cout << "Could not save the result to the file." << std::endl;
        }

        return true;
    }

    return false;
}
*/

