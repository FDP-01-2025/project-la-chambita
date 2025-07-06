// Implementación de las funciones principales del juego UNO
// Este archivo contiene la lógica del juego, manejo de cartas, turnos, minijuegos y visualización con raylib.

#include "../include/juegoUNO.h"
#include <algorithm>
#include <random>
#include <string>
#include <fstream>
#include <cstring>
#include "raylib.h"
#include "../include/minijuego_Palabra.h"
#include "../include/minijuego_Intruso.h"
#include "../include/minijuego_Velocidad.h"
#include "../include/minijuego_matematico.h"

void asignarColorAleatorioMasDosMasCuatro(Juego_UNO &juego);

// Crea e inicializa una estructura Juego_UNO con valores por defecto.
Juego_UNO crearJuegoUNO()
{
    Juego_UNO juego;
    juego.estadoDeJuego = esperando_jugadores;
    juego.turno_actual = 0;
    juego.direccion = 1;
    juego.minijuegoActivo = ninguno;
    juego.cantidadJugadores = 0;
    juego.cartasEnMazo = 0;
    juego.cartasEnDescarte = 0;

    juego.colorForzado = "";
    juego.cartaPendiente.color = "";
    juego.cartaPendiente.tipo = Numero;
    juego.cartaPendiente.valor = -1;
    juego.cartaPendiente.visible = false;

    juego.cartaEnJuego.color = "";
    juego.cartaEnJuego.tipo = Numero;
    juego.cartaEnJuego.valor = -1;
    juego.cartaEnJuego.visible = false;

    for (int i = 0; i < MAX_JUGADORES; i++)
    {
        juego.jugadores[i].nombre = "";
        juego.jugadores[i].minijuegos_ganados = 0;
        juego.jugadores[i].partidas_ganadas = 0;
        juego.jugadores[i].partidas_perdidas = 0;
        juego.jugadores[i].esTurno = false;
        juego.jugadores[i].esNuevo = true;

        for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
        {
            juego.jugadores[i].mano[j].color = "";
            juego.jugadores[i].mano[j].tipo = Numero;
            juego.jugadores[i].mano[j].valor = -1;
            juego.jugadores[i].mano[j].visible = false;
        }
    }

    return juego;
}

// Declaración global o al inicio de la función principal
Vector2 posicionMinijuego = {100.0f, 50.0f};

// Inicializa variables de estado para la selección de jugadores y nombres.
void iniciarVariablesEstado(bool &cantidadSeleccionada, int &jugadorActual, string &entradaActual, bool &nombresCompletos)
{
    cantidadSeleccionada = false;
    jugadorActual = 0;
    entradaActual = "";
    nombresCompletos = false;
}

// Llena el mazo con todas las cartas de UNO (números, especiales y comodines).
void inicializarMazo(Juego_UNO &juego)
{
    string colores[4] = {"rojo", "amarillo", "verde", "azul"};
    int indice = 0;

    // Agrega cartas de cada color
    for (int i = 0; i < 4; i++)
    {
        string color = colores[i];

        // Carta 0 (solo una por color)
        Carta c0;
        c0.color = color;
        c0.tipo = Numero;
        c0.valor = 0;
        c0.visible = false;
        juego.mazo[indice++] = c0;

        // Cartas 1-9 (dos de cada por color)
        for (int num = 1; num <= 9; num++)
        {
            for (int j = 0; j < 2; j++)
            {
                Carta c;
                c.color = color;
                c.tipo = Numero;
                c.valor = num;
                c.visible = false;
                juego.mazo[indice++] = c;
            }
        }

        // Cartas especiales: +2, bloqueo, reversa (dos de cada por color)
        for (int j = 0; j < 2; j++)
        {
            Carta cartaMasDos;
            cartaMasDos.color = color;
            cartaMasDos.tipo = Carta_Mas_dos;
            cartaMasDos.valor = -1; // Valor -1 para cartas especiales
            cartaMasDos.visible = false;
            juego.mazo[indice++] = cartaMasDos;

            Carta Bloqueo;
            Bloqueo.color = color;
            Bloqueo.tipo = Carta_Bloqueo;
            Bloqueo.valor = -1;
            Bloqueo.visible = false;
            juego.mazo[indice++] = Bloqueo;

            Carta CambioDireccion;
            CambioDireccion.color = color;
            CambioDireccion.tipo = Cambio_direccion;
            CambioDireccion.valor = -1;
            CambioDireccion.visible = false;
            juego.mazo[indice++] = CambioDireccion;
        }
    }
    // Comodines: +4 y cambio de color (4 de cada, color negro)
    for (int i = 0; i < 4; i++)
    {
        Carta cartaMasCuatro;
        cartaMasCuatro.color = "negro";
        cartaMasCuatro.tipo = Carta_Mas_cuatro;
        cartaMasCuatro.valor = -1;
        cartaMasCuatro.visible = false;
        juego.mazo[indice++] = cartaMasCuatro;

        Carta cambioColor;
        cambioColor.color = "negro";
        cambioColor.tipo = Cambio_color;
        cambioColor.valor = -1;
        cambioColor.visible = false;
        juego.mazo[indice++] = cambioColor;
    }
    juego.cartasEnMazo = indice;
}

// Baraja el mazo de cartas usando un generador aleatorio.
void barajarMazo(Juego_UNO &mazo)
{
    random_device rd;
    mt19937 generador(rd());
    shuffle(mazo.mazo, mazo.mazo + mazo.cartasEnMazo, generador);
}

// Dibuja botones para seleccionar la cantidad de jugadores y detecta la selección.
void seleccionarCantidadJugadores(Juego_UNO &juego, bool &cantidadSeleccionada)
{

    DrawText("selecciona la cantidad de jugadores: ", 100, 100, 30, DARKGRAY);

    Rectangle botones[3]{
        {100, 200, 100, 50},
        {250, 200, 100, 50},
        {400, 200, 100, 50}};

    for (int i = 0; i < 3; i++)
    {
        DrawRectangleRec(botones[i], LIGHTGRAY);
        DrawRectangleLinesEx(botones[i], 2, BLACK);                                         // Contorno de los botones
        DrawText(TextFormat("%d", i + 2), botones[i].x + 35, botones[i].y + 10, 30, BLACK); // Texto del botón
    }

    // Detecta clic en algún botón
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        for (int i = 0; i < 3; i++)
        {
            if (CheckCollisionPointRec(mouse, botones[i]))
            {
                juego.cantidadJugadores = i + 2;
                cantidadSeleccionada = true;
            }
        }
    }
}

// Permite capturar los nombres de los jugadores uno por uno desde la ventana.
void capturarNombresEnLaVentana(Juego_UNO &juego, int &jugadorActual, string &entradaActual, bool &nombresCompletos)
{
    // Captura teclas presionadas como caracteres
    int letra = GetCharPressed();

    while (letra > 0)
    {
        // Solo acepta caracteres imprimibles y máximo 20 letras por nombre
        if (letra >= 32 && letra <= 125 && entradaActual.length() < 20)
        {
            entradaActual += static_cast<char>(letra);
        }
        letra = GetCharPressed();
    }

    // Permite borrar el último carácter con BACKSPACE
    if (IsKeyPressed(KEY_BACKSPACE) && !entradaActual.empty())
    {
        entradaActual.pop_back();
    }

    // Al presionar ENTER, guarda el nombre y pasa al siguiente jugador
    if (IsKeyPressed(KEY_ENTER) && !entradaActual.empty())
    {
        juego.jugadores[jugadorActual].nombre = entradaActual;
        juego.jugadores[jugadorActual].minijuegos_ganados = 0;
        juego.jugadores[jugadorActual].partidas_ganadas = 0;
        juego.jugadores[jugadorActual].esTurno = false;

        entradaActual.clear();
        jugadorActual++;

        if (jugadorActual >= juego.cantidadJugadores)
        {
            nombresCompletos = true;
        }
    }

    // Muestra instrucciones y nombre actual en pantalla
    DrawText("Escribe el nombre del jugador: ", 100, 100, 30, DARKGRAY);
    DrawText(TextFormat("jugador %d:", jugadorActual + 1), 100, 150, 30, BLUE);
    DrawText(entradaActual.c_str(), 100, 200, 30, BLUE);
}

// Reparte 7 cartas a cada jugador desde el mazo.
void repartirCartas(Juego_UNO &juego)
{
    const int CARTAS_POR_JUGADOR = 7;
    for (int i = 0; i < juego.cantidadJugadores; i++)
    {
        for (int j = 0; j < CARTAS_POR_JUGADOR; j++)
        {
            if (juego.cartasEnMazo <= 0)
            {
                cout << "Error: no hay suficientes cartas para repartir." << endl;
                return;
            }
            if (juego.jugadores[i].mano[j].color.empty()) // Solo reparte si el slot está vacío
                juego.jugadores[i].mano[j] = juego.mazo[--juego.cartasEnMazo];
        }
    }
}

// Permite al jugador robar una carta y avanza el turno si no puede jugarla.
void intentarRobarYCambiarTurno(Juego_UNO &juego)
{
    Carta cartaRobada = robarCartaValida(juego);
    if (sePuedeJugar(juego.cartaEnJuego, cartaRobada))
    {
        juego.cartaEnJuego = cartaRobada;
        // La carta robada se juega inmediatamente si es válida
    }
    else
    {
        // Si no se puede jugar, se añade a la mano del jugador
        Jugador &jugador = juego.jugadores[juego.turno_actual];
        for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
        {
            if (jugador.mano[i].color.empty())
            {
                jugador.mano[i] = cartaRobada;
                break;
            }
        }
    }

    avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
    actualizarVisibilidadCartas(juego);
}

// Verifica si el jugador tiene alguna carta jugable en su mano.
bool tieneCartaJugable(const Jugador &jugador, Carta cartaEnjuego)
{
    for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
    {
        const Carta &c = jugador.mano[i];
        if (!c.color.empty() && sePuedeJugar(cartaEnjuego, c))
        {
            return true;
        }
    }
    return false;
}

// Roba una carta del mazo y la devuelve. Si es jugable, la pone en juego.
Carta robarCartaValida(Juego_UNO &juego)
{
    if (juego.cartasEnMazo <= 0)
        return Carta{}; // No hay cartas en el mazo

    Carta cartaRobada = juego.mazo[--juego.cartasEnMazo];
    cartaRobada.visible = true;

    // Si la carta robada es jugable, se pone en juego
    if (sePuedeJugar(juego.cartaEnJuego, cartaRobada))
    {
        juego.cartaEnJuego = cartaRobada;
    }
    else
    {
        // Si no es jugable, se añade a la mano del jugador actual
        Jugador &jugador = juego.jugadores[juego.turno_actual];

        for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
        {
            if (jugador.mano[i].color.empty())
            {
                jugador.mano[i] = cartaRobada;
                break;
            }
        }

        // Avanza el turno porque no pudo jugar
        avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
    }

    return cartaRobada; // Retorna la carta robada
}

// Función simple para robar una carta sin cambiar el turno (para penalizaciones)
Carta robarCartaSimple(Juego_UNO &juego)
{
    if (juego.cartasEnMazo <= 0)
        return Carta{}; // No hay cartas en el mazo

    Carta cartaRobada = juego.mazo[--juego.cartasEnMazo];
    cartaRobada.visible = true;
    return cartaRobada;
}

// Actualiza la visibilidad de las cartas: solo el jugador en turno ve sus cartas.
void actualizarVisibilidadCartas(Juego_UNO &juego)
{
    for (int i = 0; i < juego.cantidadJugadores; i++)
    {
        for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
        {
            juego.jugadores[i].mano[j].visible = (i == juego.turno_actual);
        }
    }
}

// Selecciona la carta inicial del juego (no puede ser comodín).
Carta cartaInicial(Juego_UNO &juego)
{
    for (int i = 0; i < MAX_MAZO; i++)
    {
        Carta &carta = juego.mazo[i];

        // Solo acepta cartas numéricas (no comodines ni especiales)
        if (!carta.color.empty() && carta.tipo == Numero)
        {
            Carta cartaInicial = carta;
            carta = Carta{}; // Eliminar del mazo
            return cartaInicial;
        }
    }

    // Si no encuentra carta válida (poco probable), devuelve vacía
    return Carta{};
}

// menu principal
MenuResult MenuPrincipal()
{
    Rectangle botonJugar = {583, 300, 200, 60};
    Rectangle botonCerrar = {583, 400, 200, 60};

    Vector2 mouse = GetMousePosition();

    DrawText("Bienvenidos a UNO", 550, 200, 30, DARKGRAY);

    DrawRectangleRec(botonJugar, CheckCollisionPointRec(mouse, botonJugar) ? LIGHTGRAY : GRAY);
    DrawText("JUGAR", botonJugar.x + 60, botonJugar.y + 15, 30, BLACK);

    DrawRectangleRec(botonCerrar, CheckCollisionPointRec(mouse, botonCerrar) ? LIGHTGRAY : GRAY);
    DrawText("CERRAR", botonCerrar.x + 55, botonCerrar.y + 15, 30, BLACK);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mouse, botonJugar))
        {
            return MENU_JUGAR;
        }
        else if (CheckCollisionPointRec(mouse, botonCerrar))
        {
            return MENU_CERRAR;
        }
    }

    return MENU_NONE; // si no se ha hecho clic en ningún botón
}

// Bucle principal del juego: gestiona el flujo de pantallas y turnos.
void ejecutarJuego(Juego_UNO &juego, bool &cantidadSeleccionada, int &jugadorActual, string &entradaActual, bool &nombresCompletos)
{
    ZonaVisual zona = obtenerZonaVisual();
    bool enMenu = true;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (enMenu)
        {
            MenuResult res = MENU_NONE;

            // Bucle interno para el menú: se repite hasta que el jugador elija JUGAR o CERRAR
            while (enMenu && !WindowShouldClose())
            {
                BeginDrawing();
                ClearBackground(RAYWHITE);

                res = MenuPrincipal();

                if (res == MENU_JUGAR)
                {
                    enMenu = false;
                }
                else if (res == MENU_CERRAR)
                {
                    CloseWindow();
                    return;
                }

                EndDrawing();
            }

            continue; // vuelve al inicio del bucle principal para evitar seguir con el juego aún
        }

        else
        {

            // Pantalla para seleccionar cantidad de jugadores
            if (!cantidadSeleccionada)
            {
                seleccionarCantidadJugadores(juego, cantidadSeleccionada);
            }
            // Pantalla para ingresar nombres de jugadores
            else if (!nombresCompletos)
            {
                capturarNombresEnLaVentana(juego, jugadorActual, entradaActual, nombresCompletos);
            }
            else
            {
                // Muestra el nombre del jugador en turno
                DrawText(TextFormat("Turno de: %s", juego.jugadores[juego.turno_actual].nombre.c_str()), 800, 50, 30, RED);

                // Si aún no se han repartido cartas, inicializa el juego
                if (juego.estadoDeJuego == esperando_jugadores)
                {
                    inicializarMazo(juego);
                    barajarMazo(juego);
                    repartirCartas(juego);
                    juego.estadoDeJuego = turno_normal;
                    juego.cartaEnJuego = cartaInicial(juego);
                    actualizarVisibilidadCartas(juego);
                }

                // Dibuja las cartas de cada jugador en su posición
                for (int i = 0; i < juego.cantidadJugadores; i++)
                {
                    int y = (i < 2) ? 100 : 700;       // Jugadores 0 y 1 arriba, 2 y 3 abajo
                    int x = (i % 2 == 0) ? 100 : 1100; // Jugadores pares izquierda, impares derecha

                    bool mostrar = (i == juego.turno_actual);
                    dibujarCartasJugador(juego.jugadores[i], x, y, mostrar);
                }

                // Dibuja el mazo y la zona de descarte
                DrawRectangleRec(zona.zonaMazo, DARKGRAY);
                DrawText("MAZO", zona.zonaMazo.x + 10, zona.zonaMazo.y + 60, 20, WHITE);

                dibujarZonaDescarte(juego.cartaEnJuego, zona.xDescarte, zona.yDescarte);

                Jugador &jugador = juego.jugadores[juego.turno_actual];

                int y = (juego.turno_actual < 2) ? 100 : 700;
                int x = (juego.turno_actual % 2 == 0) ? 100 : 1100;

                // Contar cartas visibles del jugador actual
                int cartasVisibles = 0;
                for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
                {
                    if (!jugador.mano[i].color.empty())
                    {
                        cartasVisibles++;
                    }
                }

                // Mostrar "¡UNO!" si solo tiene una carta
                if (cartasVisibles == 1)
                {
                    DrawText("¡UNO!", x + 200, y - 40, 40, RED);
                }

                // Permite jugar una carta con doble clic si es válida
                for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
                {
                    Carta &carta = jugador.mano[i];

                    if (!carta.visible || carta.color.empty())
                        continue;

                    int cartaX = x + i * ESPACIO_X;
                    int cartaY = y;

                    Rectangle rect = {(float)cartaX, (float)cartaY, (float)CARTA_ANCHO, (float)CARTA_ALTO};

                    // Si el jugador hace doble clic en una carta válida
                    if (cartaTuvoDobleClick(rect))
                    {
                        // Validación especial para +4
                        if (carta.tipo == Carta_Mas_cuatro && tieneCartaDelColor(jugador, juego.cartaEnJuego.color))
                        {
                            // No se puede jugar +4 si tiene cartas del color actual
                            // (Opcional: podrías mostrar un mensaje con temporizador)
                            continue;
                        }

                        if (sePuedeJugar(juego.cartaEnJuego, carta))
                        {
                            // Se juega la carta
                            juego.cartaEnJuego = carta;

                            // Según el tipo de carta, activa el minijuego correspondiente
                            switch (carta.tipo)
                            {
                            case Carta_Mas_dos:
                                juego.estadoDeJuego = minijuego_activo;
                                juego.minijuegoActivo = encontrar_intruso;
                                juego.cartaPendiente = carta;
                                iniciarMinijuegoIntruso();
                                break;

                            case Carta_Mas_cuatro:
                                juego.estadoDeJuego = minijuego_activo;
                                juego.minijuegoActivo = ordenar_palabra;
                                juego.cartaPendiente = carta;
                                iniciarOrdenaPalabra();
                                break;

                            case Cambio_color:
                                juego.estadoDeJuego = minijuego_activo;
                                juego.minijuegoActivo = ordenar_palabra;
                                juego.cartaPendiente = carta;
                                iniciarOrdenaPalabra();
                                break;

                            case Carta_Bloqueo:
                                juego.estadoDeJuego = minijuego_activo;
                                juego.minijuegoActivo = velocidad;
                                juego.cartaPendiente = carta;
                                iniciarMinijuegoVelocidad();
                                break;

                            case Cambio_direccion:
                                juego.estadoDeJuego = minijuego_activo;
                                juego.minijuegoActivo = matematico;
                                juego.cartaPendiente = carta;
                                minijuegoCambioDireccion(juego);
                                break;

                            default:
                                break;
                            }

                            carta = Carta{}; // Elimina la carta jugada de la mano

                            if (carta.tipo == Numero)
                            {
                                avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                                actualizarVisibilidadCartas(juego);
                            }

                            break;
                        }
                    }
                }

                // Permite robar carta si no tiene jugable y hace clic en el mazo
                if (CheckCollisionPointRec(GetMousePosition(), zona.zonaMazo) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (!tieneCartaJugable(jugador, juego.cartaEnJuego))
                    {
                        // Robar carta simple
                        Carta cartaRobada = robarCartaSimple(juego);

                        if (!cartaRobada.color.empty())
                        {
                            // Buscar espacio libre en la mano para agregar la carta robada
                            bool agregada = false;
                            for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
                            {
                                if (jugador.mano[i].color.empty())
                                {
                                    jugador.mano[i] = cartaRobada;
                                    agregada = true;
                                    break;
                                }
                            }

                            // Si la carta robada es jugable
                            if (sePuedeJugar(juego.cartaEnJuego, cartaRobada))
                            {
                                // Jugar la carta robada (como si fuera doble clic)
                                juego.cartaEnJuego = cartaRobada;

                                // Activar el minijuego correspondiente
                                switch (cartaRobada.tipo)
                                {
                                case Carta_Mas_dos:
                                    juego.estadoDeJuego = minijuego_activo;
                                    juego.minijuegoActivo = encontrar_intruso;
                                    juego.cartaPendiente = cartaRobada;
                                    iniciarMinijuegoIntruso();
                                    break;

                                case Carta_Mas_cuatro:
                                case Cambio_color:
                                    juego.estadoDeJuego = minijuego_activo;
                                    juego.minijuegoActivo = ordenar_palabra;
                                    juego.cartaPendiente = cartaRobada;
                                    iniciarOrdenaPalabra();
                                    break;

                                case Carta_Bloqueo:
                                    juego.estadoDeJuego = minijuego_activo;
                                    juego.minijuegoActivo = velocidad;
                                    juego.cartaPendiente = cartaRobada;
                                    iniciarMinijuegoVelocidad();
                                    break;

                                case Cambio_direccion:
                                    juego.estadoDeJuego = minijuego_activo;
                                    juego.minijuegoActivo = matematico;
                                    juego.cartaPendiente = cartaRobada;
                                    break;

                                default:
                                    break;
                                }

                                // Elimina la carta jugada de la mano
                                for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
                                {
                                    if (jugador.mano[i].color == cartaRobada.color &&
                                        jugador.mano[i].tipo == cartaRobada.tipo &&
                                        jugador.mano[i].valor == cartaRobada.valor)
                                    {
                                        jugador.mano[i] = Carta{};
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                // Carta no jugable, turno termina y se actualizan cartas visibles
                                avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                                actualizarVisibilidadCartas(juego);
                            }
                        }
                    }
                }

                // Chequeo de ganador
                if (juego.estadoDeJuego == turno_normal)
                {
                    for (int i = 0; i < juego.cantidadJugadores; i++)
                    {
                        if (jugadorSinCartas(juego.jugadores[i]))
                        {
                            DrawText(TextFormat("¡%s ha ganado la partida!", juego.jugadores[i].nombre.c_str()), 500, 500, 40, GREEN);

                            juego.jugadores[i].partidas_ganadas++;
                            for (int j = 0; j < juego.cantidadJugadores; j++)
                            {
                                if (j != i)
                                    juego.jugadores[j].partidas_perdidas++;
                            }

                            guardarEstadisticas(juego, "estadisticas.txt");

                            EndDrawing();
                            continue; // Salta el resto del ciclo para no sobreescribir la pantalla
                        }
                    }
                }

                // BLOQUE PARA TODOS LOS MINIJUEGOS
                if (juego.estadoDeJuego == minijuego_activo)
                {
                    bool minijuegoTerminado = false;
                    bool minijuegoGanado = false;

                    // Ejecutar el minijuego correspondiente
                    switch (juego.minijuegoActivo)
                    {
                    case ordenar_palabra:
                        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
                        actualizarMinijuegoOrdenaPalabra(juego.jugadores[juego.turno_actual]);
                        minijuegoTerminado = minijuegoOrdenaTerminado();
                        if (minijuegoTerminado)
                            minijuegoGanado = minijuegoOrdenaGano();
                        break;

                    case encontrar_intruso:
                        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
                        actualizarMinijuegoIntruso(juego.jugadores[juego.turno_actual]);
                        minijuegoTerminado = minijuegoIntrusoTerminado();
                        if (minijuegoTerminado)
                            minijuegoGanado = minijuegoIntrusoGano();
                        break;

                    case velocidad:
                        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
                        actualizarMinijuegoVelocidad(juego.jugadores[juego.turno_actual]);
                        minijuegoTerminado = minijuegoVelocidadTerminado();
                        if (minijuegoTerminado)
                            minijuegoGanado = minijuegoVelocidadGano();
                        break;

                    case matematico:
                        // El minijuego matemático se maneja de forma diferente (ventana separada)
                        if (jugarMinijuegoMatematico())
                        {
                            minijuegoGanado = true;
                            minijuegoTerminado = true;
                        }
                        else
                        {
                            minijuegoGanado = false;
                            minijuegoTerminado = true;
                        }
                        break;

                    default:
                        break;
                    }

                    // Manejar el resultado del minijuego
                    if (minijuegoTerminado)
                    {
                        if (minijuegoGanado)
                        {
                            // Ganó el minijuego: la carta se juega normalmente
                            juego.cartaEnJuego = juego.cartaPendiente;

                            // Aplicar efectos específicos según el tipo de carta
                            switch (juego.cartaPendiente.tipo)
                            {
                            case Carta_Mas_dos:
                            {
                                // Si gana +2, el siguiente jugador roba 2 cartas
                                int jugadorPenalizado = (juego.turno_actual + juego.direccion + juego.cantidadJugadores) % juego.cantidadJugadores;
                                for (int i = 0; i < 2; i++)
                                {
                                    Carta cartaRobada = robarCartaSimple(juego);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                                        {
                                            if (juego.jugadores[jugadorPenalizado].mano[j].color.empty())
                                            {
                                                juego.jugadores[jugadorPenalizado].mano[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                            }
                            break;

                            case Carta_Mas_cuatro:
                            {
                                // Si gana +4, el siguiente jugador roba 4 cartas
                                int jugadorPenalizado = (juego.turno_actual + juego.direccion + juego.cantidadJugadores) % juego.cantidadJugadores;
                                for (int i = 0; i < 4; i++)
                                {
                                    Carta cartaRobada = robarCartaSimple(juego);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                                        {
                                            if (juego.jugadores[jugadorPenalizado].mano[j].color.empty())
                                            {
                                                juego.jugadores[jugadorPenalizado].mano[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                            }
                            break;

                            case Carta_Bloqueo:
                                aplicarBloqueo(juego);
                                // El avance de turno ya está en aplicarBloqueo
                                break;

                            case Cambio_direccion:
                                aplicarCambioDireccion(juego);
                                avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                                break;

                            default:
                                break;
                            }

                            // Para cartas de cambio de color, asignar color aleatorio
                            if (juego.cartaPendiente.tipo == Carta_Mas_cuatro || juego.cartaPendiente.tipo == Cambio_color)
                                asignarColorAleatorioMasDosMasCuatro(juego);
                        }
                        else
                        {
                            // Perdió el minijuego: la carta se juega pero hay penalización
                            juego.cartaEnJuego = juego.cartaPendiente;

                            // Aplicar efectos específicos según el tipo de carta
                            switch (juego.cartaPendiente.tipo)
                            {
                            case Carta_Mas_dos:
                            {
                                // Si pierde +2, el siguiente jugador roba 2 cartas Y el jugador actual roba 2 como penalización
                                int jugadorPenalizado = (juego.turno_actual + juego.direccion + juego.cantidadJugadores) % juego.cantidadJugadores;
                                // El siguiente jugador roba 2 cartas
                                for (int i = 0; i < 2; i++)
                                {
                                    Carta cartaRobada = robarCartaSimple(juego);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                                        {
                                            if (juego.jugadores[jugadorPenalizado].mano[j].color.empty())
                                            {
                                                juego.jugadores[jugadorPenalizado].mano[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                // El jugador actual roba 2 cartas como penalización
                                for (int i = 0; i < 2; i++)
                                {
                                    Carta cartaRobada = robarCartaSimple(juego);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                                        {
                                            if (juego.jugadores[juego.turno_actual].mano[j].color.empty())
                                            {
                                                juego.jugadores[juego.turno_actual].mano[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                            }
                            break;

                            case Carta_Mas_cuatro:
                            {
                                // Si pierde +4, el siguiente jugador roba 4 cartas Y el jugador actual roba 4 como penalización
                                int jugadorPenalizado = (juego.turno_actual + juego.direccion + juego.cantidadJugadores) % juego.cantidadJugadores;
                                // El siguiente jugador roba 4 cartas
                                for (int i = 0; i < 4; i++)
                                {
                                    Carta cartaRobada = robarCartaSimple(juego);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                                        {
                                            if (juego.jugadores[jugadorPenalizado].mano[j].color.empty())
                                            {
                                                juego.jugadores[jugadorPenalizado].mano[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                // El jugador actual roba 4 cartas como penalización
                                for (int i = 0; i < 4; i++)
                                {
                                    Carta cartaRobada = robarCartaSimple(juego);
                                    if (!cartaRobada.color.empty())
                                    {
                                        for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                                        {
                                            if (juego.jugadores[juego.turno_actual].mano[j].color.empty())
                                            {
                                                juego.jugadores[juego.turno_actual].mano[j] = cartaRobada;
                                                break;
                                            }
                                        }
                                    }
                                }
                                avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                            }
                            break;

                            case Carta_Bloqueo:
                                aplicarBloqueo(juego);
                                // El avance de turno ya está en aplicarBloqueo
                                break;

                            case Cambio_direccion:
                                aplicarCambioDireccion(juego);
                                avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                                break;

                            default:
                                break;
                            }

                            // Para cartas de cambio de color, asignar color aleatorio
                            if (juego.cartaPendiente.tipo == Carta_Mas_cuatro || juego.cartaPendiente.tipo == Cambio_color)
                                asignarColorAleatorioMasDosMasCuatro(juego);
                        }

                        int jugadorQueJugo = juego.turno_actual;

                        // Limpiar la carta jugada de la mano del jugador actual
                        for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
                        {
                            if (juego.jugadores[juego.turno_actual].mano[i].color == juego.cartaPendiente.color &&
                                juego.jugadores[juego.turno_actual].mano[i].tipo == juego.cartaPendiente.tipo &&
                                juego.jugadores[juego.turno_actual].mano[i].valor == juego.cartaPendiente.valor)
                            {
                                juego.jugadores[jugadorQueJugo].mano[i] = Carta{};
                                break;
                            }
                        }

                        // Volver al juego normal y avanzar turno si no es bloqueo (ya lo hace aplicarBloqueo)
                        juego.estadoDeJuego = turno_normal;
                        juego.minijuegoActivo = ninguno;

                        // Solo avanzar turno si no es una carta que ya lo hizo dentro del case
                        if (juego.cartaPendiente.tipo != Carta_Bloqueo &&
                            juego.cartaPendiente.tipo != Carta_Mas_dos &&
                            juego.cartaPendiente.tipo != Carta_Mas_cuatro)
                        {
                            avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                        }

                        actualizarVisibilidadCartas(juego);
                    }

                    EndDrawing();
                    continue; // Salta el resto del ciclo para no sobreescribir la pantalla
                }
            }

            EndDrawing();
        }
    }
}

// Dibuja todas las cartas de la mano de un jugador en pantalla. //a ver si funciona
void dibujarCartasJugador(const Jugador &jugador, int xInicial, int yInicial, bool mostrarTodas)
{
    int espacioX = 100;    // espacio entre cartas
    int maxCartasFila = 7; // máximo cartas por fila

    Vector2 mouse = GetMousePosition();

    for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
    {
        if (!mostrarTodas && !jugador.mano[i].visible)
            continue;

        const Carta &carta = jugador.mano[i];
        if (carta.color.empty())
            continue;

        // Calcula la fila y la columna dentro de la fila
        int fila = i / maxCartasFila; // 0 para fila 1, 1 para fila 2
        int columna = i % maxCartasFila;

        int x = xInicial + columna * espacioX;
        int y = yInicial + fila * (CARTA_ALTO + 10); // 10 pixeles de separación entre filas

        Rectangle rectCarta = {(float)x, (float)y, (float)CARTA_ANCHO, (float)CARTA_ALTO};

        // Detectar hover
        bool mouseSobreCarta = CheckCollisionPointRec(mouse, rectCarta);
        Color colorFondo = mouseSobreCarta ? LIGHTGRAY : GRAY;

        DrawRectangleRec(rectCarta, colorFondo);
        DrawRectangleLinesEx(rectCarta, 2, BLACK);

        // Color del texto según el color de la carta
        Color colorTexto = BLACK;
        if (carta.color == "rojo")
            colorTexto = RED;
        else if (carta.color == "amarillo")
            colorTexto = YELLOW;
        else if (carta.color == "verde")
            colorTexto = GREEN;
        else if (carta.color == "azul")
            colorTexto = BLUE;
        else if (carta.color == "negro")
            colorTexto = DARKGRAY;

        // Texto de la carta según tipo
        string textoCarta;
        switch (carta.tipo)
        {
        case Numero:
            textoCarta = to_string(carta.valor);
            break;
        case Carta_Mas_dos:
            textoCarta = "+2";
            break;
        case Carta_Mas_cuatro:
            textoCarta = "+4️";
            break;
        case Cambio_color:
            textoCarta = "Color";
            break;
        case Cambio_direccion:
            textoCarta = "Reversa";
            break;
        case Carta_Bloqueo:
            textoCarta = "Bloqueo";
            break;
        default:
            textoCarta = "?";
            break;
        }

        int anchoTexto = MeasureText(textoCarta.c_str(), 20);
        int textoX = x + (CARTA_ANCHO / 2) - (anchoTexto / 2);
        int textoY = y + (CARTA_ALTO / 2) - 10;

        DrawText(textoCarta.c_str(), textoX, textoY, 20, colorTexto);
    }
}

// Imprime el mazo en consola para depuración.
void imprimirMazo(const Juego_UNO &juego)
{
    for (int i = 0; i < juego.cartasEnMazo; i++)
    {
        cout << "[" << juego.mazo[i].color << ", ";
        if (juego.mazo[i].tipo == Numero)
            cout << juego.mazo[i].valor;
        else
            cout << "Especial";
        cout << "] ";
    }
    cout << endl;
}

// Verifica si una carta se puede jugar sobre la carta actual en juego según las reglas originales de UNO.
bool sePuedeJugar(Carta actual, Carta elegida)
{
    // Comodines siempre se pueden jugar
    if (elegida.tipo == Cambio_color || elegida.tipo == Carta_Mas_cuatro)
        return true;

    // Coincide color
    if (elegida.color == actual.color)
        return true;

    // Coincide valor numérico
    if (elegida.tipo == Numero && actual.tipo == Numero && elegida.valor == actual.valor)
        return true;

    // Coincide tipo especial Y color (ej. Bloqueo azul sobre Bloqueo azul)
    if (elegida.tipo == actual.tipo && elegida.color == actual.color)
        return true;

    return false;
}

// Detecta si hubo doble clic sobre una carta (para jugarla).
bool cartaTuvoDobleClick(const Rectangle &rect)
{
    static float tiempoUltimoClick = 0;
    static int clicks = 0;

    if (CheckCollisionPointRec(GetMousePosition(), rect))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            float tiempoActual = GetTime();
            if (tiempoActual - tiempoUltimoClick < 0.3f)
            { // Si el segundo clic es rápido, cuenta como doble clic
                clicks++;
                if (clicks == 2)
                {
                    clicks = 0;
                    return true;
                }
            }
            else
            {
                clicks = 1;
            }
            tiempoUltimoClick = tiempoActual;
        }
    }
    return false;
}

// Dibuja la carta en la zona de descarte.
void dibujarZonaDescarte(const Carta &carta, int x, int y)
{
    if (carta.color.empty())
        return;

    DrawRectangle(x, y, 80, 120, LIGHTGRAY);
    DrawRectangleLines(x, y, 80, 120, BLACK);

    // Selecciona color del texto según la carta
    Color colorTexto = BLACK;
    if (carta.color == "rojo")
        colorTexto = RED;
    else if (carta.color == "amarillo")
        colorTexto = YELLOW;
    else if (carta.color == "verde")
        colorTexto = GREEN;
    else if (carta.color == "azul")
        colorTexto = BLUE;
    else if (carta.color == "negro")
        colorTexto = DARKGRAY;

    // Traduce el tipo de carta a texto
    string textoCarta;
    switch (carta.tipo)
    {
    case Numero:
        textoCarta = to_string(carta.valor);
        break;
    case Carta_Mas_dos:
        textoCarta = "+2";
        break;
    case Carta_Mas_cuatro:
        textoCarta = "+4";
        break;
    case Cambio_direccion:
        textoCarta = "Rev";
        break;
    case Carta_Bloqueo:
        textoCarta = "bloqueo";
        break;
    case Cambio_color:
        textoCarta = "Color?";
        break;
    default:
        textoCarta = "?";
        break;
    }

    // Dibuja el texto centrado en la carta de descarte
    DrawText(textoCarta.c_str(), x + 30, y + 50, 30, colorTexto);
}

// Avanza el turno al siguiente jugador, respetando la dirección del juego.
void avanzarTurno(int &jugadorActual, int direccion, int totalJugadores, Juego_UNO &juego)
{
    jugadorActual = (jugadorActual + direccion + totalJugadores) % totalJugadores;
    juego.turno_actual = jugadorActual;
    actualizarVisibilidadCartas(juego);
}

// Devuelve la estructura con las posiciones de la zona visual del mazo y descarte.
ZonaVisual obtenerZonaVisual()
{
    ZonaVisual zona;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Posición X del mazo: centro menos media carta menos la mitad del espacio
    zona.zonaMazo.width = CARTA_ANCHO;
    zona.zonaMazo.height = CARTA_ALTO;
    zona.zonaMazo.x = screenWidth / 2 - CARTA_ANCHO - ESPACIO_ENTRE_MAZO_DESCARTE / 2;
    zona.zonaMazo.y = screenHeight / 2 - CARTA_ALTO / 2;

    // Posición X del descarte: centro más medio espacio
    zona.xDescarte = screenWidth / 2 + ESPACIO_ENTRE_MAZO_DESCARTE / 2;
    zona.yDescarte = screenHeight / 2 - CARTA_ALTO / 2;

    return zona;
}


// Guarda o actualiza las estadísticas acumuladas de TODOS los jugadores que han jugado a UNO.
// El archivo mantiene un registro único por jugador, sumando partidas jugadas, ganadas y minijuegos ganados.
void guardarEstadisticas(const Juego_UNO &juego, const string &EstadisticaArchivo)
{
    struct EstadisticasJugador {
        string nombre;
        int partidas_jugadas = 0;
        int partidas_ganadas = 0;
        int minijuegos_ganados = 0;
    };

    EstadisticasJugador estadisticas[200];
    int totalJugadores = 0;

    // Leer estadísticas existentes usando fstream
    ifstream fin(EstadisticaArchivo);
    string linea;
    EstadisticasJugador temp;
    int estado = 0;
    while (getline(fin, linea)) {
        if (linea.rfind("Jugador: ", 0) == 0) {
            temp.nombre = linea.substr(9);
            estado = 1;
        }
        else if (linea.rfind("  Partidas jugadas: ", 0) == 0 && estado == 1) {
            temp.partidas_jugadas = stoi(linea.substr(20));
            estado = 2;
        }
        else if (linea.rfind("  Partidas ganadas: ", 0) == 0 && estado == 2) {
            temp.partidas_ganadas = stoi(linea.substr(20));
            estado = 3;
        }
        else if (linea.rfind("  Minijuegos ganados: ", 0) == 0 && estado == 3) {
            temp.minijuegos_ganados = stoi(linea.substr(23));
            estado = 4;
        }
        else if (linea.rfind("-----------------------------------", 0) == 0 && estado == 4) {
            if (!temp.nombre.empty() && totalJugadores < 200) {
                estadisticas[totalJugadores++] = temp;
            }
            temp = EstadisticasJugador();
            estado = 0;
        }
    }
    fin.close();

    // Actualizar estadísticas con los datos de la partida actual
    for (int i = 0; i < juego.cantidadJugadores; ++i) {
        const Jugador &jugador = juego.jugadores[i];
        bool encontrado = false;
        for (int j = 0; j < totalJugadores; ++j) {
            if (estadisticas[j].nombre == jugador.nombre) {
                estadisticas[j].partidas_jugadas += 1;
                estadisticas[j].partidas_ganadas += jugador.partidas_ganadas;
                estadisticas[j].minijuegos_ganados += jugador.minijuegos_ganados;
                encontrado = true;
                break;
            }
        }
        if (!encontrado && totalJugadores < 200) {
            estadisticas[totalJugadores].nombre = jugador.nombre;
            estadisticas[totalJugadores].partidas_jugadas = 1;
            estadisticas[totalJugadores].partidas_ganadas = jugador.partidas_ganadas;
            estadisticas[totalJugadores].minijuegos_ganados = jugador.minijuegos_ganados;
            totalJugadores++;
        }
    }

    // Escribir todas las estadísticas usando fstream
    ofstream fout(EstadisticaArchivo, ios::trunc);
    if (!fout.is_open()) {
        cout << "Error al abrir el archivo para guardar estadísticas." << endl;
        return;
    }
    fout << "----- Estadísticas acumuladas de UNO -----\n";
    for (int i = 0; i < totalJugadores; ++i) {
        fout << "Jugador: " << estadisticas[i].nombre << "\n";
        fout << "  Partidas jugadas: " << estadisticas[i].partidas_jugadas << "\n";
        fout << "  Partidas ganadas: " << estadisticas[i].partidas_ganadas << "\n";
        fout << "  Minijuegos ganados: " << estadisticas[i].minijuegos_ganados << "\n";
        fout << "-----------------------------------\n";
    }
    fout.close();
}

// MINIJUEGO DE REFLEJOS PARA CARTA +2

// Ejecuta un minijuego de reflejos: el jugador debe presionar una tecla aleatoria rápidamente.
// Si gana, el rival roba 2 cartas. Si pierde, el jugador que lanzó el +2 roba 2 cartas.
bool ejecutarMinijuegoReflejos(Juego_UNO &juego)
{
    float tiempoLimite = 3.0f; // 3 segundos para reaccionar
    float tiempoTranscurrido = 0.0f;
    char teclaCorrecta = 'A' + GetRandomValue(0, 25); // Tecla aleatoria entre A-Z

    while (!WindowShouldClose() && tiempoTranscurrido < tiempoLimite)
    {
        float deltaTime = GetFrameTime();
        tiempoTranscurrido += deltaTime;

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("¡MINIJUEGO DE REFLEJOS!", 600, 200, 40, YELLOW);
        DrawText(TextFormat("Presiona la tecla: %c", teclaCorrecta), 600, 300, 30, LIGHTGRAY);
        DrawText(TextFormat("Tiempo restante: %.2f", tiempoLimite - tiempoTranscurrido), 600, 400, 25, RED);

        EndDrawing();

        if (IsKeyPressed((int)teclaCorrecta))
        {
            // Ganó: el rival recibe +2 cartas
            int rival = (juego.turno_actual + juego.direccion + juego.cantidadJugadores) % juego.cantidadJugadores;

            for (int i = 0; i < 2; i++)
            {
                Carta nueva = robarCartaValida(juego);
                for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                {
                    if (juego.jugadores[rival].mano[j].color.empty())
                    {
                        juego.jugadores[rival].mano[j] = nueva;
                        break;
                    }
                }
            }

            return true;
        }
    }

    // Perdió: el jugador que lanzó el +2 recibe +2 cartas
    int castigado = juego.turno_actual;
    for (int i = 0; i < 2; i++)
    {
        Carta nueva = robarCartaValida(juego);
        for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
        {
            if (juego.jugadores[castigado].mano[j].color.empty())
            {
                juego.jugadores[castigado].mano[j] = nueva;
                break;
            }
        }
    }

    return false;
}

// Aplica el efecto de la carta +2 con el minijuego de reflejos.
// Si el jugador gana el minijuego, el siguiente jugador roba 2 cartas.
// Si pierde, el jugador actual (que tiró el +2) roba 2 cartas como castigo.
// Luego avanza el turno respetando la dirección.
void aplicarMasDosConMinijuego(Juego_UNO &juego)
{
    int jugadorActual = juego.turno_actual;
    int siguienteJugador = (jugadorActual + juego.direccion + juego.cantidadJugadores) % juego.cantidadJugadores;

    bool ganoMinijuego = ejecutarMinijuegoReflejos(juego);

    if (ganoMinijuego)
    {
        // El siguiente jugador roba 2 cartas
        for (int i = 0; i < 2; i++)
        {
            Carta cartaRobada = robarCartaValida(juego);
            if (!cartaRobada.color.empty())
            {
                for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                {
                    if (juego.jugadores[siguienteJugador].mano[j].color.empty())
                    {
                        juego.jugadores[siguienteJugador].mano[j] = cartaRobada;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // El jugador actual roba 2 cartas como castigo
        for (int i = 0; i < 2; i++)
        {
            Carta cartaRobada = robarCartaValida(juego);
            if (!cartaRobada.color.empty())
            {
                for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                {
                    if (juego.jugadores[jugadorActual].mano[j].color.empty())
                    {
                        juego.jugadores[jugadorActual].mano[j] = cartaRobada;
                        break;
                    }
                }
            }
        }
    }
}

// Aplica el efecto de la carta +4
void aplicarMasCuatro(Juego_UNO &juego)
{
    int jugadorPenalizado = (juego.turno_actual + juego.direccion + juego.cantidadJugadores) % juego.cantidadJugadores;

    for (int i = 0; i < 4; i++)
    {
        Carta cartaRobada = robarCartaValida(juego);
        if (!cartaRobada.color.empty())
        {
            for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
            {
                if (juego.jugadores[jugadorPenalizado].mano[j].color.empty())
                {
                    juego.jugadores[jugadorPenalizado].mano[j] = cartaRobada;
                    break;
                }
            }
        }
    }
}

// Aplica el efecto de cambio de color
void aplicarCambioColor(Juego_UNO &juego)
{
    // El color se selecciona aleatoriamente después del minijuego
    // Esta función se llama después de que el minijuego determine el color
}

// Aplica el efecto de la carta bloqueo (salta turno)
void aplicarBloqueo(Juego_UNO &juego)
{
    // El bloqueo salta el turno del siguiente jugador respetando la dirección
    // Avanza una vez para el siguiente jugador, y otra vez para saltarlo
    avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
}

// Aplica el efecto de cambio de dirección
void aplicarCambioDireccion(Juego_UNO &juego)
{
    juego.direccion *= -1; // Invierte la dirección del juego
}

// Función para verificar si un jugador no tiene cartas
bool jugadorSinCartas(const Jugador &jugador)
{
    for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
    {
        if (!jugador.mano[i].color.empty())
        {
            return false; // Tiene al menos una carta real
        }
    }
    return true; // No tiene cartas
}

// Permite al jugador seleccionar el color tras jugar Cambio de color o +4
void asignarColorAleatorioMasDosMasCuatro(Juego_UNO &juego)
{
    // Solo aplica para +4 y cambio de color
    if (juego.cartaPendiente.tipo != Carta_Mas_cuatro && juego.cartaPendiente.tipo != Cambio_color)
        return;

    bool colorSeleccionado = false;
    string colores[] = {"rojo", "amarillo", "verde", "azul"};
    Color coloresRaylib[] = {RED, YELLOW, GREEN, BLUE};

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Rectangle botones[4];
    int anchoBoton = 120, altoBoton = 120;
    int espacio = 40;
    int totalAncho = 4 * anchoBoton + 3 * espacio;
    int xInicial = (screenWidth - totalAncho) / 2;
    int yBoton = screenHeight / 2 - altoBoton / 2;

    for (int i = 0; i < 4; i++)
    {
        botones[i] = { (float)(xInicial + i * (anchoBoton + espacio)), (float)yBoton, (float)anchoBoton, (float)altoBoton };
    }

    while (!colorSeleccionado && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Selecciona un color:", screenWidth / 2 - 180, yBoton - 80, 40, DARKGRAY);

        for (int i = 0; i < 4; i++)
        {
            DrawRectangleRec(botones[i], coloresRaylib[i]);
            DrawRectangleLinesEx(botones[i], 4, BLACK);
            DrawText(colores[i].c_str(), botones[i].x + 25, botones[i].y + 40, 30, BLACK);
        }

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 4; i++)
            {
                if (CheckCollisionPointRec(mouse, botones[i]))
                {
                    juego.cartaEnJuego.color = colores[i];
                    colorSeleccionado = true;
                    break;
                }
            }
        }
    }
}

// Verifica si el jugador tiene al menos una carta del color especificado en su mano.
// Devuelve true si encuentra una carta del color objetivo, false si no.
bool tieneCartaDelColor(Jugador &jugador, const string &colorObjetivo)
{
    for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
    {
        if (!jugador.mano[i].color.empty() && jugador.mano[i].color == colorObjetivo)
            return true;
    }
    return false;
}

/*
Minijuego para carta "Cambio de dirección":
- El jugador debe adivinar si el número aleatorio (1-10) es par o impar.
- Si acierta, se ejecuta el cambio de dirección.
- Si falla, el jugador roba 2 cartas y el turno avanza normalmente (no cambia dirección).
*/

bool minijuegoCambioDireccion(Juego_UNO &juego) {
    int numero = GetRandomValue(1, 10);
    bool respuesta = false; // true = par, false = impar
    bool seleccionHecha = false;
    bool resultado = false;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Rectangle botonPar = { (float)(screenWidth/2 - 160), (float)(screenHeight/2), 120, 60 };
    Rectangle botonImpar = { (float)(screenWidth/2 + 40), (float)(screenHeight/2), 120, 60 };

    while (!seleccionHecha && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("MINIJUEGO: ¿El número oculto es PAR o IMPAR?", screenWidth/2 - 320, screenHeight/2 - 100, 30, DARKGRAY);
        DrawRectangleRec(botonPar, LIGHTGRAY);
        DrawRectangleRec(botonImpar, LIGHTGRAY);
        DrawRectangleLinesEx(botonPar, 3, BLUE);
        DrawRectangleLinesEx(botonImpar, 3, RED);
        DrawText("PAR", botonPar.x + 30, botonPar.y + 15, 30, BLUE);
        DrawText("IMPAR", botonImpar.x + 20, botonImpar.y + 15, 30, RED);

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, botonPar)) {
                respuesta = true;
                seleccionHecha = true;
            } else if (CheckCollisionPointRec(mouse, botonImpar)) {
                respuesta = false;
                seleccionHecha = true;
            }
        }
    }

    bool esPar = (numero % 2 == 0);
    resultado = (respuesta == esPar);

    // Mostrar resultado
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (resultado) {
        DrawText(TextFormat("¡Correcto! Era %d. Cambia la dirección.", numero), screenWidth/2 - 250, screenHeight/2, 30, GREEN);
    } else {
        DrawText(TextFormat("¡Incorrecto! Era %d. Robas 2 cartas.", numero), screenWidth/2 - 250, screenHeight/2, 30, RED);
    }
    EndDrawing();
    WaitTime(1.5);

    return resultado;
}

// Lógica para ejecutar el minijuego de cambio de dirección y aplicar penalización si pierde
void ejecutarCambioDireccionConMinijuego(Juego_UNO &juego) {
    bool gano = minijuegoCambioDireccion(juego);
    if (gano) {
        aplicarCambioDireccion(juego);
        avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
    } else {
        // Penalización: roba 2 cartas y NO cambia dirección
        for (int i = 0; i < 2; i++) {
            Carta cartaRobada = robarCartaSimple(juego);
            if (!cartaRobada.color.empty()) {
                for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++) {
                    if (juego.jugadores[juego.turno_actual].mano[j].color.empty()) {
                        juego.jugadores[juego.turno_actual].mano[j] = cartaRobada;
                        break;
                    }
                }
            }
        }
        avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
    }
}