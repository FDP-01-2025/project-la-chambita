#ifndef JUEGO_UNO_H  //para evitar que se incluya dos veces el mismo archivo.
#define JUEGO_UNO_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// caracteristicas de los tipos de cartas
enum tipo_de_Carta
{
    Numero,
    Carta_Mas_dos,
    Carta_Mas_cuatro,
    Cambio_color,
    Cambio_direccion,
    Carta_Bloqueo
};

// caracteristicas del estado de la partida
enum estado_de_juego
{
    esperando_jugadores,
    repartiendo_cartas,
    turno_normal,
    minijuego_activo,
    juego_terminado
};

// caracteristicas de las cartas
struct Carta
{
    string color;
    tipo_de_Carta tipo;
    int valor;
    string nombreMinijuego;
    string descripcion;
};

// caracteristicas del jugador
struct Jugador
{
    string nombre;
    vector<Carta> mano; // las cartas que tiene en la mano
    int minijuegos_ganados;
    int partidas_ganadas;
    bool esTurno;
};

// caracteristicas del juego
struct Juego_UNO
{
    vector<Carta> mazo;     // la baraja principal de donde se agarran cartas
    vector<Carta> descarte; // es la pila de cartas que ya se han jugado
    vector<Jugador> jugadores;
    int turno_actual; // indice del jugador que va estar jugando
    int direccion;    // 1 o -1 (la direccion normal o invertida)
    estado_de_juego estadoDeJuego;
};

// Funciones
void inicializarMazo(vector<Carta> &mazo);
void barajarMazo(vector<Carta> &mazo);
void repartirCartas(vector<Jugador> &jugadores, vector<Carta> &mazo);
void pedirJugadores(vector<Jugador> &jugadores, int &cantidad);
void dibujarCartasJugador(const Jugador &jugador, int xInicial, int yInicial);

#endif