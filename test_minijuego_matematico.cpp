#include <cstdio>
#include "minijuego_matematico.h"

int main() {
    bool resultado = jugarMinijuegoMatematico();

    if (resultado)
        printf("Ganaste el minijuego matemático.\n");
    else
        printf("Perdiste el minijuego matemático.\n");

    return 0;
}
