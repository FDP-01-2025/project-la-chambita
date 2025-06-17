// en el .h solo se DECLARAN O NOMBRAN variables

#ifndef JUEGO_UNO_H // para evitar que se incluya dos veces el mismo archivo.
#define JUEGO_UNO_H

#include <string>
#include <iostream>
#include "raylib.h"

using namespace std;

const int MAX_JUGADORES = 4;
const int MAX_CARTAS_POR_JUGADOR = 30;
const int MAX_MAZO = 108;

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
    bool visible; // si la carta esta volteada o visible
};

// caracteristicas del jugador
struct Jugador
{
    string nombre;
    Carta mano[MAX_CARTAS_POR_JUGADOR]; // las cartas que tiene en la mano
    int minijuegos_ganados;
    int partidas_ganadas;
    bool esTurno;
};

// caracteristicas del juego
struct Juego_UNO
{
    Carta mazo[MAX_MAZO];
    int cartasEnMazo;

    Carta descarte[MAX_MAZO];
    int cartasEnDescarte;

    Jugador jugadores[MAX_JUGADORES];
    int cantidadJugadores; // cant real de jugadores

    int turno_actual; // indice del jugador que va estar jugando
    int direccion;    // 1 o -1 (la direccion normal o invertida)
    estado_de_juego estadoDeJuego;

    Carta cartaEnJuego;
};

//caracteristicas de las zonas visuales donde estaran las cartas
struct ZonaVisual
{
    Rectangle zonaMazo;
    int xDescarte;
    int yDescarte;
};

// Funciones solo declaradas
void ejecutarJuego(Juego_UNO &juego);

void inicializarMazo(Juego_UNO &juego);
void barajarMazo(Juego_UNO &juego);
void repartirCartas(Juego_UNO &juego);
void dibujarCartasJugador(const Jugador &jugador, int xInicial, int yInicial, bool mostrarTodas);
void capturarNombresEnLaVentana(Juego_UNO &juego, int &jugadorActual, string &entradaActual, bool &nombresCompletos);
void procesarTurno(Juego_UNO &juego);
void seleccionarCatidadJugadores(Juego_UNO &juego, bool &cantidadSeleccionada);
bool sePuedeJugar(Carta actual, Carta elegida);
bool cartaTuvoDobleClick(const Rectangle rect);
void dibujarZonaDescarte(const Carta &carta, int x, int y);
bool jugadorRobaSiClick(const Rectangle& zonaMazo, Juego_UNO &juego, int jugador);
Carta robarCartaValida(Juego_UNO &juego);
ZonaVisual obtenerZonaVisual();
Juego_UNO crearJuegoUNO();
void iniciarVariablesEstado(bool &cantidadSeleccionada, int &jugadorActual, string &entradaActual, bool &nombresCompletos);
void ejecutarJuego(Juego_UNO &juego, bool &cantidadSeleccionada, int &jugadorActual, string &entradaActual, bool &nombresCompletos);
void actualizarVisibilidadCartas(Juego_UNO &juego);

#endif