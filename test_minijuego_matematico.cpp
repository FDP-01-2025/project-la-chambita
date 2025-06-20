#include "minijuego_matematico.h"
#include <cstdio>

int main() {
    bool resultado = jugarMinijuegoMatematico();

    if (resultado) {
        printf("Ganaste el minijuego matematico.\n");
    } else {
        printf("Perdiste el minijuego matematico.\n");
    }

    return 0;
}
