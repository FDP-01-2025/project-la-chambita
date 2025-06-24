#ifndef JUEGO_UNO_H
#define JUEGO_UNO_H

#include <string>
#include <iostream>
#include "raylib.h"

// No usar 'using namespace std;' en headers

struct MensajeTemporal {
    std::string texto;
    float tiempoRestante;
    bool activo;

    MensajeTemporal() : texto(""), tiempoRestante(0), activo(false) {}
};

void ActivarMensaje(MensajeTemporal &mensaje, const std::string &nuevoMensaje, float duracion);

// Constantes, enums y structs usando std::string en lugar de string

const int MAX_JUGADORES = 4;
const int MAX_CARTAS_POR_JUGADOR = 30;
const int MAX_MAZO = 108;
const int CARTA_ANCHO= 80;
const int CARTA_ALTO = 120;
const int ESPACIO_X =100;
const int ESPACIO_Y = 100;
const int INICIO_X = 100;
const int INICIO_Y =100;

enum tipo_de_Carta {
    Numero,
    Carta_Mas_dos,
    Carta_Mas_cuatro,
    Cambio_color,
    Cambio_direccion,
    Carta_Bloqueo
};

enum estado_de_juego {
    esperando_jugadores,
    repartiendo_cartas,
    turno_normal,
    minijuego_activo,
    juego_terminado
};

struct Carta {
    std::string color;
    tipo_de_Carta tipo;
    int valor;
    std::string nombreMinijuego;
    std::string descripcion;
    bool visible;
};

struct Jugador {
    std::string nombre;
    Carta mano[MAX_CARTAS_POR_JUGADOR];
    int minijuegos_ganados;
    int partidas_ganadas;
    bool esTurno;
};

struct Juego_UNO {
    Carta mazo[MAX_MAZO];
    int cartasEnMazo;

    Carta descarte[MAX_MAZO];
    int cartasEnDescarte;

    Jugador jugadores[MAX_JUGADORES];
    int cantidadJugadores;

    int turno_actual;
    int direccion;
    estado_de_juego estadoDeJuego;

    Carta cartaEnJuego;
};

struct ZonaVisual {
    Rectangle zonaMazo;
    int xDescarte;
    int yDescarte;
};

Juego_UNO crearJuegoUNO();
void iniciarVariablesEstado(bool &cantidadSeleccionada, int &jugadorActual, std::string &entradaActual, bool &nombresCompletos);
void inicializarMazo(Juego_UNO &juego);
void barajarMazo(Juego_UNO &juego);

void seleccionarCantidadJugadores(Juego_UNO &juego, bool &cantidadSeleccionada);
void capturarNombresEnLaVentana(Juego_UNO &juego, int &jugadorActual, std::string &entradaActual, bool &nombresCompletos);

void repartirCartas(Juego_UNO &juego);
Carta robarCartaValida(Juego_UNO &juego);
void actualizarVisibilidadCartas(Juego_UNO &juego);

void ejecutarJuego(Juego_UNO &juego, bool &cantidadSeleccionada, int &jugadorActual, std::string &entradaActual, bool &nombresCompletos);
void dibujarCartasJugador(const Jugador &jugador, int xInicial, int yInicial, bool mostrarTodas);
void dibujarZonaDescarte(const Carta &carta, int x, int y);
bool jugadorRobaSiClick(const Rectangle& zonaMazo, Juego_UNO &juego, int jugador);
bool sePuedeJugar(Carta actual, Carta elegida);
bool tieneCartaJugable(const Jugador &jugador, Carta cartaEnjuego);
void avanzarTurno(int &jugadorActual, int direccion, int totalJugadores, Juego_UNO &juego);
bool cartaTuvoDobleClick(const Rectangle &rect);
void intentarRobarYCambiarTurno(Juego_UNO &juego);
Carta cartaInicial(Juego_UNO &juego);
void guardarPartida(const Jugador &jugador, const Carta &cartaActual, int turno, const std::string &guardarPartida);
bool verificarGanador(const Jugador &jugador);

ZonaVisual obtenerZonaVisual();

struct EstadisticasJugador {
    int partidasJugadas = 0;
    int partidasGanadas = 0;
    int partidasPerdidas = 0;
    int minijuegosJugados = 0;
};

void actualizarEstadisticas(EstadisticasJugador& stats, bool ganoPartida, int minijuegosJugadosEnPartida);

void DibujarMensaje(MensajeTemporal &mensaje, float deltaTime);
bool verificarGanador(const Jugador &jugador);


#endif // JUEGO_UNO_H
