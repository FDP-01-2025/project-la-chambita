#include <cstdio>
#include "minijuego_Palabra.h"

int main() {
    bool resultado = jugarOrdenaPalabra();

    if (resultado) {
        printf("Ganaste el minijuego.\n");
    } else {
        printf("Perdiste el minijuego o cerraste la ventana.\n");
    }

    return 0;
}
