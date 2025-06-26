// En este archivo .h solo se DECLARAN o NOMBRAN variables, estructuras y funciones, no se implementan.

// Evita que el archivo se incluya más de una vez en el mismo proyecto.
#ifndef JUEGO_UNO_H
#define JUEGO_UNO_H

#include <string>
#include <iostream>
#include "raylib.h"

using namespace std;

// Constantes para los límites y dimensiones del juego
const int MAX_JUGADORES = 4;           // Número máximo de jugadores permitidos
const int MAX_CARTAS_POR_JUGADOR = 30; // Máximo de cartas que puede tener un jugador en la mano
const int MAX_MAZO = 108;              // Número total de cartas en el mazo
const int CARTA_ANCHO = 80;            // Ancho de cada carta (en píxeles)
const int CARTA_ALTO = 120;            // Alto de cada carta (en píxeles)
const int ESPACIO_X = 100;             // Espaciado horizontal entre cartas
const int ESPACIO_Y = 100;             // Espaciado vertical entre cartas
const int INICIO_X = 100;              // Posición inicial X para dibujar cartas
const int INICIO_Y = 100;              // Posición inicial Y para dibujar cartas

// Tipos de cartas posibles en el juego UNO
enum tipo_de_Carta
{
    Numero,           // Carta con valor numérico
    Carta_Mas_dos,    // Carta +2: el siguiente jugador roba dos cartas
    Carta_Mas_cuatro, // Carta +4: el siguiente jugador roba cuatro cartas y cambia color
    Cambio_color,     // Cambia el color actual del juego
    Cambio_direccion, // Invierte el sentido de juego
    Carta_Bloqueo     // Salta el turno del siguiente jugador
};

// Estados posibles de la partida
enum estado_de_juego
{
    esperando_jugadores, // Esperando a que se unan los jugadores
    repartiendo_cartas,  // Repartiendo cartas a los jugadores
    turno_normal,        // Turno normal de juego
    minijuego_activo,    // Hay un minijuego en curso
    juego_terminado      // El juego ha finalizado
};

// Estructura que representa una carta del juego UNO
struct Carta
{
    string color;           // Color de la carta (rojo, azul, verde, amarillo, etc.)
    tipo_de_Carta tipo;     // Tipo de carta (numérica, +2, +4, etc.)
    int valor;              // Valor numérico de la carta (si aplica)
    string nombreMinijuego; // Nombre del minijuego asociado (si aplica)
    string descripcion;     // Descripción de la carta o minijuego
    bool visible;           // Indica si la carta está visible o volteada
};

// Estructura que representa a un jugador
struct Jugador
{
    string nombre;                      // Nombre del jugador
    Carta mano[MAX_CARTAS_POR_JUGADOR]; // Cartas que tiene el jugador en la mano
    int minijuegos_ganados;             // Cantidad de minijuegos ganados por el jugador
    int partidas_ganadas;               // Cantidad de partidas ganadas
    int partidas_perdidas;              // Cantidad de partidas perdidas
    bool esTurno;                       // Indica si es el turno de este jugador
    bool esNuevo;                       // Indica si el jugador es nuevo en la partida
};

// Estructura principal que representa el estado completo del juego UNO
struct Juego_UNO
{
    Carta mazo[MAX_MAZO]; // Mazo de cartas disponibles para robar
    int cartasEnMazo;     // Cantidad de cartas restantes en el mazo

    Carta descarte[MAX_MAZO]; // Pila de cartas descartadas
    int cartasEnDescarte;     // Cantidad de cartas en el descarte

    Jugador jugadores[MAX_JUGADORES]; // Lista de jugadores en la partida
    int cantidadJugadores;            // Cantidad real de jugadores en la partida

    int turno_actual;              // Índice del jugador que tiene el turno actual
    int direccion;                 // Dirección del turno: 1 (normal) o -1 (invertido)
    estado_de_juego estadoDeJuego; // Estado actual de la partida

    Carta cartaEnJuego; // Carta que está actualmente en juego (en la pila de descarte)

    string colorForzado; // Color forzado por una carta especial (ej: +4 o cambio de color)
};

// Estructura para definir las zonas visuales donde se dibujan las cartas en la interfaz gráfica
struct ZonaVisual
{
    Rectangle zonaMazo; // Área donde se dibuja el mazo de cartas
    int xDescarte;      // Coordenada X para dibujar la pila de descarte
    int yDescarte;      // Coordenada Y para dibujar la pila de descarte
};

// Declaraciones de funciones (solo se indican, no se implementan aquí)

// Inicialización y preparación del juego
Juego_UNO crearJuegoUNO();                                                                                                  // Crea e inicializa una nueva partida de UNO
void iniciarVariablesEstado(bool &cantidadSeleccionada, int &jugadorActual, string &entradaActual, bool &nombresCompletos); // Inicializa variables de estado
void inicializarMazo(Juego_UNO &juego);                                                                                     // Llena el mazo con todas las cartas del juego
void barajarMazo(Juego_UNO &juego);                                                                                         // Mezcla aleatoriamente las cartas del mazo

// Interacción inicial con el jugador
void seleccionarCantidadJugadores(Juego_UNO &juego, bool &cantidadSeleccionada);                                      // Permite seleccionar la cantidad de jugadores
void capturarNombresEnLaVentana(Juego_UNO &juego, int &jugadorActual, string &entradaActual, bool &nombresCompletos); // Captura los nombres de los jugadores

// Comienzo del juego
void repartirCartas(Juego_UNO &juego);              // Reparte cartas a cada jugador al inicio
Carta robarCartaValida(Juego_UNO &juego);           // Roba una carta válida del mazo
void actualizarVisibilidadCartas(Juego_UNO &juego); // Actualiza la visibilidad de las cartas según el turno

// Lógica y turnos de jugabilidad
void ejecutarJuego(Juego_UNO &juego, bool &cantidadSeleccionada, int &jugadorActual, string &entradaActual, bool &nombresCompletos); // Controla el flujo principal del juego
void dibujarCartasJugador(const Jugador &jugador, int xInicial, int yInicial, bool mostrarTodas);                                    // Dibuja las cartas de un jugador en pantalla
void dibujarZonaDescarte(const Carta &carta, int x, int y);                                                                          // Dibuja la carta en la pila de descarte
bool jugadorRobaSiClick(const Rectangle &zonaMazo, Juego_UNO &juego, int jugador);                                                   // Permite que un jugador robe una carta si hace clic en el mazo
bool sePuedeJugar(Carta actual, Carta elegida);                                                                                      // Verifica si una carta elegida se puede jugar sobre la actual
bool tieneCartaJugable(const Jugador &jugador, Carta cartaEnjuego);                                                                  // Verifica si el jugador tiene alguna carta jugable
void avanzarTurno(int &jugadorActual, int direccion, int totalJugadores, Juego_UNO &juego);                                          // Avanza el turno al siguiente jugador según la dirección
bool cartaTuvoDobleClick(const Rectangle &rect);                                                                                     // Detecta si una carta fue seleccionada con doble clic
void intentarRobarYCambiarTurno(Juego_UNO &juego);                                                                                   // Permite robar una carta y pasar el turno si es necesario
Carta cartaInicial(Juego_UNO &juego);                                                                                                // Selecciona la carta inicial para comenzar la partida
void guardarEstadisticas(const Juego_UNO &juego, const string &EstadisticaArchivo);                                                  // Guarda las estadísticas de la partida en un archivo
bool ejecutarMinijuegoReflejos(Juego_UNO &juego);                                                                                    // Ejecuta un minijuego de reflejos
void aplicarMasDos(Juego_UNO &juego, int objetivo);                                                                                  // Aplica el efecto de la carta +2 al jugador objetivo
void aplicarMasCuatro(Juego_UNO &juego);                                                                                             // Aplica el efecto de la carta +4
void aplicarCambioColor(Juego_UNO &juego);                                                                                           // Permite cambiar el color actual del juego
void aplicarBloqueo(Juego_UNO &juego);                                                                                               // Aplica el efecto de bloqueo (salta turno)
void aplicarCambioDireccion(Juego_UNO &juego);                                                                                       // Invierte la dirección del turno
void aplicarMasDosConMinijuego(Juego_UNO &juego, int jugadorPenalizado, int jugadorComodin);                                         // Aplica +2 con minijuego especial

// Funciones de utilidad gráfica
ZonaVisual obtenerZonaVisual(); // Obtiene las zonas visuales para dibujar el mazo y el descarte

// Estructura para almacenar estadísticas individuales de cada jugador
struct EstadisticasJugador
{
    int partidasJugadas = 0;   // Total de partidas jugadas
    int partidasGanadas = 0;   // Total de partidas ganadas
    int partidasPerdidas = 0;  // Total de partidas perdidas
    int minijuegosJugados = 0; // Total de minijuegos jugados
};

#endif
