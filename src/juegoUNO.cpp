// Implementación de las funciones principales del juego UNO
// Este archivo contiene la lógica del juego, manejo de cartas, turnos, minijuegos y visualización con raylib.

#include "../include/juegoUNO.h"
#include <algorithm>
#include <random>
#include <string>
#include <fstream>
#include "raylib.h"
#include "../include/minijuego_Palabra.h"

// ======================= FUNCIONES PRINCIPALES DEL JUEGO =======================
// Crea e inicializa una estructura Juego_UNO con valores por defecto.
Juego_UNO crearJuegoUNO()
{
    Juego_UNO juego;
    juego.estadoDeJuego = esperando_jugadores;
    juego.turno_actual = 0;
    juego.direccion = 1;
    return juego;
}

// Declaración global o al inicio de la función principal
Vector2 posicionMinijuego = {100.0f, 50.0f}; // Ajusta las coordenadas a tu gusto

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

    return Carta{}; // No se necesita devolver la carta realmente
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
    while (juego.cartasEnMazo > 0)
    {
        Carta carta = juego.mazo[--juego.cartasEnMazo];
        if (carta.tipo != Carta_Mas_cuatro && carta.tipo != Cambio_color)
        {
            carta.visible = true;
            return carta;
        }
        else
        {
            juego.mazo[juego.cartasEnMazo] = carta; // Devuelve la carta al fondo del mazo
        }
    }

    // Si no hay ninguna válida, retorna carta vacía
    return Carta{};
}

// menu principal
void mostrarMenuPrincipal()
{
    bool menuActivo = true;

    Rectangle botonJugar = {583, 300, 200, 60};
    Rectangle botonCerrar = {583, 400, 200, 60};

    while (menuActivo && !WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Bienvenidos a UNO", 550, 200, 30, DARKGRAY);

        // Botón Jugar
        DrawRectangleRec(botonJugar, CheckCollisionPointRec(mouse, botonJugar) ? LIGHTGRAY : GRAY);
        DrawText("JUGAR", botonJugar.x + 60, botonJugar.y + 15, 30, BLACK);

        // Botón Cerrar
        DrawRectangleRec(botonCerrar, CheckCollisionPointRec(mouse, botonCerrar) ? LIGHTGRAY : GRAY);
        DrawText("CERRAR", botonCerrar.x + 55, botonCerrar.y + 15, 30, BLACK);

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mouse, botonJugar))
            {
                menuActivo = false; // Sale del menú y continúa al juego
            }
            else if (CheckCollisionPointRec(mouse, botonCerrar))
            {
                CloseWindow(); // Cierra la ventana directamente
                exit(0);       // Termina el programa completamente
            }
        }
    }
}

// Bucle principal del juego: gestiona el flujo de pantallas y turnos.
void ejecutarJuego(Juego_UNO &juego, bool &cantidadSeleccionada, int &jugadorActual, string &entradaActual, bool &nombresCompletos)
{
    // temporal
    ZonaVisual zona = obtenerZonaVisual();
    mostrarMenuPrincipal();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!cantidadSeleccionada)
        {
            seleccionarCantidadJugadores(juego, cantidadSeleccionada);
        }
        else if (!nombresCompletos)
        {
            capturarNombresEnLaVentana(juego, jugadorActual, entradaActual, nombresCompletos);
        }
        else
        {
            // Muestra el nombre del jugador en turno
            DrawText(TextFormat("Turno de: %s", juego.jugadores[juego.turno_actual].nombre.c_str()), 800, 50, 30, RED);

            if (juego.estadoDeJuego == esperando_jugadores)
            {
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
                DrawText("¡UNO!", x + 200, y - 40, 40, RED); // Ajusta posición según tu diseño
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

                if (cartaTuvoDobleClick(rect) && sePuedeJugar(juego.cartaEnJuego, carta))
                {
                    cout << "Carta jugada: " << carta.color << ", tipo: " << carta.tipo << ", valor: " << carta.valor << endl;

                    juego.cartaEnJuego = carta;

                    if (carta.tipo == Carta_Mas_dos)
                    {
                        int jugadorPenalizado = (juego.turno_actual + juego.direccion + juego.cantidadJugadores) % juego.cantidadJugadores;
                        aplicarMasDosConMinijuego(juego, jugadorPenalizado, juego.turno_actual);
                    } // probar minijuego ordena la palabra
                    else if (carta.tipo == Carta_Mas_cuatro || carta.tipo == Cambio_color)
                    {
                        juego.estadoDeJuego = minijuego_activo;
                        juego.cartaPendiente = carta;
                        iniciarOrdenaPalabra();
                    }

                    carta = Carta{}; // Elimina la carta jugada de la mano
                    avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                    actualizarVisibilidadCartas(juego);
                    break;
                }
            }

            // Permite robar carta si no tiene jugable y hace clic en el mazo
            if (CheckCollisionPointRec(GetMousePosition(), zona.zonaMazo) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (!tieneCartaJugable(jugador, juego.cartaEnJuego))
                {
                    intentarRobarYCambiarTurno(juego);
                }
            }
        }

        // chequeo ganador

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

                    guardarEstadisticas(juego, "archivos/estadisticas.txt");

                    EndDrawing();
                    WaitTime(5.0);
                    return;
                }
            }
        }

        // ✅ BLOQUE INDEPENDIENTE PARA EL MINIJUEGO DE ORDENAR PALABRA
        if (juego.estadoDeJuego == minijuego_activo)
        {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
            actualizarMinijuegoOrdenaPalabra(juego.jugadores[juego.turno_actual]);

            if (minijuegoOrdenaTerminado())
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    if (minijuegoOrdenaGano())
                    {
                        juego.cartaEnJuego = juego.cartaPendiente; // ✅ Descarta la carta
                    }
                    else
                    {
                        // ❌ Penalización: el siguiente jugador roba 4 cartas
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

                    // 🎨 Color aleatorio para carta de cambio de color
                    int colorIndex = rand() % 4;
                    switch (colorIndex)
                    {
                    case 0:
                        juego.cartaEnJuego.color = "rojo";
                        break;
                    case 1:
                        juego.cartaEnJuego.color = "azul";
                        break;
                    case 2:
                        juego.cartaEnJuego.color = "verde";
                        break;
                    case 3:
                        juego.cartaEnJuego.color = "amarillo";
                        break;
                    }
                    // Salir del minijuego y volver al juego normal
                    juego.estadoDeJuego = turno_normal;

                    // Limpia la carta jugada de la mano
                    for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
                    {
                        if (juego.jugadores[juego.turno_actual].mano[i].color == juego.cartaEnJuego.color &&
                            juego.jugadores[juego.turno_actual].mano[i].tipo == juego.cartaEnJuego.tipo &&
                            juego.jugadores[juego.turno_actual].mano[i].valor == juego.cartaEnJuego.valor)
                        {
                            juego.jugadores[juego.turno_actual].mano[i] = Carta{};
                            break;
                        }
                    }

                    // 🔁 Continuar juego normalmente
                    juego.estadoDeJuego = turno_normal;
                    avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                    actualizarVisibilidadCartas(juego);
                }
            }

            EndDrawing(); // ❗ Ya se dibujó todo lo del minijuego, cerramos frame
            continue;     // ⛔ Saltamos el resto del ciclo para no sobreescribir la pantalla
        }

        EndDrawing();
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

// Verifica si una carta se puede jugar sobre la carta actual en juego.
bool sePuedeJugar(Carta actual, Carta elegida)
{
    // Comodines siempre se pueden jugar
    if (elegida.tipo == Cambio_color || elegida.tipo == Carta_Mas_cuatro)
        return true;

    // Coincide el color
    if (elegida.color == actual.color)
        return true;

    // Coincide el número (solo para cartas numéricas)
    if (elegida.tipo == Numero && actual.tipo == Numero && elegida.valor == actual.valor)
        return true;

    // Cartas especiales: solo si coinciden en tipo y color
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
    zona.zonaMazo = {1000, 300, 80, 120};
    zona.xDescarte = 1100;
    zona.yDescarte = 300;
    return zona;
}

// Guarda las estadísticas de los jugadores en un archivo de texto.
void guardarEstadisticas(const Juego_UNO &juego, const string &EstadisticaArchivo)
{
    ofstream archivo(EstadisticaArchivo, ios::app);

    if (!archivo.is_open())
    {
        cout << "Error al abrir el archivo para guardar estadísticas." << endl;
        return;
    }

    archivo << "----- Resultados de la partida -----" << endl;

    for (int i = 0; i < juego.cantidadJugadores; ++i)
    {
        const Jugador &jugador = juego.jugadores[i];
        archivo << "Jugador: " << jugador.nombre << endl;
        archivo << "  Partidas ganadas: " << jugador.partidas_ganadas << endl;
        archivo << "  Partidas perdidas: " << jugador.partidas_perdidas << endl;
        archivo << "  Minijuegos ganados: " << jugador.minijuegos_ganados << endl;
        archivo << "-----------------------------------" << endl;
    }

    archivo.close();
    cout << "Estadísticas guardadas en " << EstadisticaArchivo << endl;
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
void aplicarMasDosConMinijuego(Juego_UNO &juego, int jugadorPenalizado, int jugadorComodin)
{
    bool ganoMinijuego = ejecutarMinijuegoReflejos(juego);

    if (ganoMinijuego)
    {
        // El jugador penalizado roba 2 cartas
        for (int i = 0; i < 2; i++)
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
    else
    {
        // El jugador que tiró el +2 roba las 2 cartas (castigo)
        for (int i = 0; i < 2; i++)
        {
            Carta cartaRobada = robarCartaValida(juego);
            if (!cartaRobada.color.empty())
            {
                for (int j = 0; j < MAX_CARTAS_POR_JUGADOR; j++)
                {
                    if (juego.jugadores[jugadorComodin].mano[j].color.empty())
                    {
                        juego.jugadores[jugadorComodin].mano[j] = cartaRobada;
                        break;
                    }
                }
            }
        }
    }
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
