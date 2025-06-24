#include "juegoUNO.h"

//esta es la funcion para el turno del jugador en el juego UNO

#include <iostream>
#include <string>
using namespace std;

const int MAX_CARTAS = 108;
const int MAX_JUGADORES = 4;

struct Carta {
    string color;  // rojo, azul, verde, amarillo, comodin
    string valor;  // 0–9, +2, salta, reversa, comodin, comodin+4
};

struct Juego_UNO {
    Carta mazo[MAX_CARTAS];
    int topeMazo;

    Carta pilaDescarte[MAX_CARTAS];
    int topeDescarte;

    Carta manos[MAX_JUGADORES][MAX_CARTAS];
    int cantidadCartas[MAX_JUGADORES];

    int jugadorActual;
    int totalJugadores;

    int sentido = 1;  // 1 = horario, -1 = antihorario
};

// Función para comprobar si una carta se puede jugar
bool sePuedeJugar(Carta actual, Carta elegida) {
    return (elegida.color == actual.color || 
            elegida.valor == actual.valor || 
            elegida.color == "comodin");
}

// Avanza el turno respetando el sentido del juego
void avanzarTurno(Juego_UNO &juego, int pasos = 1) {
    juego.jugadorActual = (juego.jugadorActual + pasos * juego.sentido + juego.totalJugadores) % juego.totalJugadores;
}

void turnoJugador(Juego_UNO &juego) {
    int jugador = juego.jugadorActual;
    int &cantidad = juego.cantidadCartas[jugador];
    Carta* mano = juego.manos[jugador];
    Carta cartaActual = juego.pilaDescarte[juego.topeDescarte - 1];

    cout << "\n--- Turno del Jugador " << jugador + 1 << " ---\n";
    cout << "Carta en el descarte: [" << cartaActual.color << " " << cartaActual.valor << "]\n";

    cout << "\nTu mano:\n";
    for (int i = 0; i < cantidad; ++i) {
        cout << i + 1 << ". [" << mano[i].color << " " << mano[i].valor << "]\n";
    }

    // Verifica si puede jugar
    bool puedeJugar = false;
    for (int i = 0; i < cantidad; ++i) {
        if (sePuedeJugar(cartaActual, mano[i])) {
            puedeJugar = true;
            break;
        }
    }

    if (puedeJugar) {
        int opcion;
        do {
            cout << "\nElige el número de carta a jugar (0 para robar): ";
            cin >> opcion;

            if (opcion == 0) {
                if (juego.topeMazo > 0) {
                    Carta robada = juego.mazo[--juego.topeMazo];
                    mano[cantidad++] = robada;
                    cout << "Robaste: [" << robada.color << " " << robada.valor << "]\n";
                } else {
                    cout << "El mazo está vacío. No puedes robar.\n";
                }
                break;
            } else if (opcion > 0 && opcion <= cantidad) {
                Carta elegida = mano[opcion - 1];
                if (sePuedeJugar(cartaActual, elegida)) {
                    juego.pilaDescarte[juego.topeDescarte++] = elegida;

                    // Eliminar carta jugada
                    for (int j = opcion - 1; j < cantidad - 1; ++j) {
                        mano[j] = mano[j + 1];
                    }
                    --cantidad;

                    cout << "Jugaste: [" << elegida.color << " " << elegida.valor << "]\n";

                    // Efectos especiales
                    if (elegida.valor == "+2") {
                        avanzarTurno(juego);
                        int siguiente = juego.jugadorActual;
                        for (int i = 0; i < 2 && juego.topeMazo > 0; ++i) {
                            juego.manos[siguiente][juego.cantidadCartas[siguiente]++] = juego.mazo[--juego.topeMazo];
                        }
                        cout << "Jugador " << siguiente + 1 << " robó 2 cartas.\n";
                    }
                    else if (elegida.valor == "salta") {
                        cout << "Jugador siguiente pierde su turno.\n";
                        avanzarTurno(juego);
                    }
                    else if (elegida.valor == "reversa") {
                        juego.sentido *= -1;
                        cout << "¡Dirección cambiada!\n";
                    }
                    else if (elegida.valor == "comodin" || elegida.valor == "comodin+4") {
                        string nuevoColor;
                        cout << "Elige nuevo color (rojo, verde, azul, amarillo): ";
                        cin >> nuevoColor;
                        juego.pilaDescarte[juego.topeDescarte - 1].color = nuevoColor;

                        if (elegida.valor == "comodin+4") {
                            avanzarTurno(juego);
                            int siguiente = juego.jugadorActual;
                            for (int i = 0; i < 4 && juego.topeMazo > 0; ++i) {
                                juego.manos[siguiente][juego.cantidadCartas[siguiente]++] = juego.mazo[--juego.topeMazo];
                            }
                            cout << "Jugador " << siguiente + 1 << " robó 4 cartas.\n";
                        }
                    }

                    break;  // Terminó el turno
                } else {
                    cout << "Esa carta no se puede jugar.\n";
                }
            } else {
                cout << "Opción inválida.\n";
            }
        } while (true);
    } else {
        cout << "No tienes cartas válidas. Robando una...\n";
        if (juego.topeMazo > 0) {
            Carta robada = juego.mazo[--juego.topeMazo];
            mano[cantidad++] = robada;
            cout << "Robaste: [" << robada.color << " " << robada.valor << "]\n";
        } else {
            cout << "El mazo está vacío.\n";
        }
    }

    // Cambia turno al final
    avanzarTurno(juego);
}

// Guardar partida

    void guardarPartida(const Jugador &jugador, const Carta &cartaActual, int turno, const std::string &guardarPartida);
    {
        ofstream archivo("guardar.Partida.txt");

        if (!archivo.is_open())
        {
            cout << "Error al abrir el archivo para guardar la partida." << endl;
            return;
        }

        archivo << "Jugador: " << jugador.nombre << endl; // Guarda nombre de jugadores
        archivo << "Cartas en mano: " << juego.mazo[indice++] << endl;

        archivo << "Mano ";
        for (const auto &carta : jugador.mano)
        {
            archivo << carta.color << " " << carta.valor << "\n";

        }

        archivo << "Carta actual: " << cartaActual.color << " " << cartaActual.valor << "\n";
        archivo << "Turno actual: " << juego.turno_actual << "\n";

        archivo.close();
        cout << "Partida guardada en: " << guardarPartida << endl;
    }

   // Verificar ganador

    bool verificarGanador(const Jugador &jugador)
    {
        if (juego.mazo[indice++])
        {
            cout << jugador.nombre << "Has ganado la partida" << endl;

            ofstream archivo("ganadores.txt", std::ios::app);
            if (archivo.is_open())
            {
                archivo << "Ganador: " << jugador.nombre << "\n";
                archivo.close();
            }
            else
            {
                cout << "No se pudo guardar el resultado en el archivo." << endl;
            }

            return true;
        }

        return false;
    }

