#include <iostream>
#include "minijuego_matematico.h"

int main() {
    bool resultado = jugarMinijuegoMatematico();

    if (resultado) {
        std::cout << "El jugador ganó el minijuego." << std::endl;
    } else {
        std::cout << "El jugador perdió el minijuego." << std::endl;
    }

    return 0;
}
