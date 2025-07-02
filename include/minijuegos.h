// Find the intruder game
// This game is a simple game that allows the user to find the intruder in an array of elements
// The user can play up to 2 times
// The user can play again if they want to
// The user can exit the game if they want to
// The user can see the answer if they want to
// The user can see the type of the intruder if they want to
// The user can see the value of the intruder if they want to
// DONE BY JOHI
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Estructura para elementos del juego
struct Element {
    string value;
    string type;  // "number", "fruit", "color", "animal"
};

// Función para mostrar el array
void displayArray(Element arr[], int size) {
    cout << "\nElements in the array: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i].value << " ";
    }
    cout << endl;
}

// Función para verificar si la respuesta es correcta
bool verifyAnswer(string answer, Element intruder) {
    return answer == intruder.value;
}

// Función para generar los elementos del juego
void generateRandomArray(Element arr[], int& size, Element& intruder) {
    string numbers[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    string fruits[] = {"apple", "pear", "grape", "orange", "banana", "strawberry", "kiwi", "mango"};
    string colors[] = {"red", "blue", "green", "yellow", "purple", "pink", "orange", "black"};
    string animals[] = {"dog", "cat", "elephant", "giraffe", "tiger", "lion", "monkey", "bird"};

    string types[] = {"number", "fruit", "color", "animal"};
    string selectedType = types[rand() % 4];

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

// Función para jugar una ronda
void playRound() {
    const int MAX_SIZE = 4;
    Element elements[MAX_SIZE];
    Element intruder;
    int size;

    generateRandomArray(elements, size, intruder);
    displayArray(elements, size);

    string answer;
    cout << "\nEnter the intruder element: ";
    cin >> answer;

    if (verifyAnswer(answer, intruder)) {
        cout << "Correct! You found the intruder." << endl;
        cout << "The intruder was a " << intruder.type << ": " << intruder.value << endl;
    } else {
        cout << "Incorrect. The intruder was a " << intruder.type << ": " << intruder.value << endl;
        cout << "Game Over!" << endl;
        exit(0); // Terminar el juego
    }
}

int main() {
    srand(time(0));  // Inicializar el generador de números aleatorios
    
    int attempts = 0;
    const int MAX_ATTEMPTS = 2;

    cout << "\n=== WELCOME TO THE FIND THE INTRUDER GAME ===" << endl;
    cout << "In this game, there is an array with elements of the same type" << endl;
    cout << "and one intruder element of a different type." << endl;
    cout << "Your goal is to find the intruder by typing its value." << endl;
    cout << "You can play up to " << MAX_ATTEMPTS << " times!" << endl;

    while (attempts < MAX_ATTEMPTS) {
        cout << "\nAttempt " << (attempts + 1) << " of " << MAX_ATTEMPTS << endl;
        playRound();
        attempts++;

        if (attempts < MAX_ATTEMPTS) {
            cout << "\nDo you want to play again? (y/n): ";
            char playAgain;
            cin >> playAgain;
            if (playAgain != 'y' && playAgain != 'Y') {
                break;
            }
        }
    }

    cout << "\nThank you for playing! You completed " << attempts << " rounds." << endl;
    return 0;
}

