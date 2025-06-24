// definir o decirle al programa como funciona cada cosa

#include "juegoUNO.h"
#include <algorithm>
#include <random>
#include <string>
#include <fstream>
#include "raylib.h"

// FUNCIONES:

Juego_UNO crearJuegoUNO()
{
    Juego_UNO juego;
    juego.estadoDeJuego = esperando_jugadores;
    juego.turno_actual = 0;
    juego.direccion = 1;
    return juego;
}

void iniciarVariablesEstado(bool &cantidadSeleccionada, int &jugadorActual, string &entradaActual, bool &nombresCompletos)
{
    cantidadSeleccionada = false;
    jugadorActual = 0;
    entradaActual = "";
    nombresCompletos = false;
}

void inicializarMazo(Juego_UNO &juego)
{
    string colores[4] = {"rojo", "amarillo", "verde", "azul"};
    int indice = 0;

    for (int i = 0; i < 4; i++)
    {
        string color = colores[i];

        Carta c0;
        c0.color = color;
        c0.tipo = Numero;
        c0.valor = 0;
        c0.visible = false;
        juego.mazo[indice++] = c0;

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

        for (int j = 0; j < 2; j++)
        {
            Carta cartaMasDos; // Objeto carta
            cartaMasDos.color = color;
            cartaMasDos.tipo = Carta_Mas_dos; // Asignar el valor enum al campo tipo
            cartaMasDos.valor = -1;           // puedes usar -1 para cartas especiales sin número
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

void barajarMazo(Juego_UNO &mazo)
{
    random_device rd;
    mt19937 generador(rd());
    shuffle(mazo.mazo, mazo.mazo + mazo.cartasEnMazo, generador);
}

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
        DrawRectangleLinesEx(botones[i], 2, BLACK);                                         // esta wea es pura estetica. son contornos para rectangulos
        DrawText(TextFormat("%d", i + 2), botones[i].x + 35, botones[i].y + 10, 30, BLACK); // el %d dice que espera un valor entero
    }

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

void capturarNombresEnLaVentana(Juego_UNO &juego, int &jugadorActual, string &entradaActual, bool &nombresCompletos)
{

    // captura las teclas como caracteres
    int letra = GetCharPressed();

    while (letra > 0)
    {
        // // Solo acepta caracteres imprimibles de espacio a-z y 20 letras por nombre
        if (letra >= 32 && letra <= 125 && entradaActual.length() < 20)
        {
            entradaActual += static_cast<char>(letra); // agrega la letra al nombre
        }
        letra = GetCharPressed(); // sigue capturando siempre y cuando se esten presionando teclas
    }

    // Si se presiona BACKSPACE y hay texto, borra el último carácter
    if (IsKeyPressed(KEY_BACKSPACE) && !entradaActual.empty())
    {
        entradaActual.pop_back(); // borra 1 letra del final
    }

    // cuando presione enter, y ya se haya escrito algo
    if (IsKeyPressed(KEY_ENTER) && !entradaActual.empty())
    {

        // esto guarda el nombre actual en el jugador que corresponde
        juego.jugadores[jugadorActual].nombre = entradaActual;
        juego.jugadores[jugadorActual].minijuegos_ganados = 0;
        juego.jugadores[jugadorActual].partidas_ganadas = 0;
        juego.jugadores[jugadorActual].esTurno = false;

        entradaActual.clear(); // limpia la entrada para el sig. jugador
        jugadorActual++;       // pasa al sig. jugador

        if (jugadorActual >= juego.cantidadJugadores)
        {
            nombresCompletos = true;
        }
    }

    // esto vamostrar las instrucciones en pantalla
    DrawText("Escribe el nombre del jugador: ", 100, 100, 30, DARKGRAY);
    DrawText(TextFormat("jugador %d:", jugadorActual + 1), 100, 150, 30, BLUE);
    DrawText(entradaActual.c_str(), 100, 200, 30, BLUE);
}

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

void intentarRobarYCambiarTurno(Juego_UNO &juego)
{
    Carta cartaRobada = robarCartaValida(juego);
    if (sePuedeJugar(juego.cartaEnJuego, cartaRobada))
    {
        juego.cartaEnJuego = cartaRobada;
        // no agrego la carta ala baraja pq ya se jugo de una
    }

    else
    {
        // si no se puede jugar se agrega a la baraja
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

Carta robarCartaValida(Juego_UNO &juego)
{
    if (juego.cartasEnMazo <= 0)
        return Carta{}; // No hay cartas en el mazo

    // Roba la carta
    Carta cartaRobada = juego.mazo[--juego.cartasEnMazo];
    cartaRobada.visible = true;

    // Si se puede jugar, se convierte en la nueva carta en juego
    if (sePuedeJugar(juego.cartaEnJuego, cartaRobada))
    {
        juego.cartaEnJuego = cartaRobada;
    }
    else
    {
        // Si no se puede jugar, se le agrega al jugador actual
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

    return Carta{}; // ya no se necesita devolver nada útil
}

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
            juego.mazo[juego.cartasEnMazo] = carta; // vuelve al fondo
        }
    }

    // Si no hay ninguna válida
    return Carta{};
}

void ejecutarJuego(Juego_UNO &juego, bool &cantidadSeleccionada, int &jugadorActual, string &entradaActual, bool &nombresCompletos)
{
    ZonaVisual zona = obtenerZonaVisual();

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
            // Muestra en pantalla el nombre del jugador que tiene el turno actual
            DrawText(TextFormat("Turno de: %s", juego.jugadores[juego.turno_actual].nombre.c_str()), 800, 50, 30, RED);

            if (juego.estadoDeJuego == esperando_jugadores)
            {
                repartirCartas(juego);
                juego.estadoDeJuego = turno_normal;
                juego.cartaEnJuego = cartaInicial(juego);
                actualizarVisibilidadCartas(juego);
            }

            for (int i = 0; i < juego.cantidadJugadores; i++)
            {
                int y, x;

                if (i < 2)
                {
                    y = 100; // jugadores 0 y 1 (arriba)
                }

                else
                {
                    y = 700; // jugadores 2 y 3 (abajo)
                }

                if (i % 2 == 0)
                { // jugadores 0  y2 (izquierda)
                    x = 100;
                }

                else
                {
                    x = 1100; // jugasores 1 y 3 (derecha)
                }
                bool mostrar = (i == juego.turno_actual);
                dibujarCartasJugador(juego.jugadores[i], x, y, mostrar);
            }

            DrawRectangleRec(zona.zonaMazo, DARKGRAY);
            DrawText("MAZO", zona.zonaMazo.x + 10, zona.zonaMazo.y + 60, 20, WHITE);

            dibujarZonaDescarte(juego.cartaEnJuego, zona.xDescarte, zona.yDescarte);

            Jugador &jugador = juego.jugadores[juego.turno_actual];

            int y, x;

            if (juego.turno_actual < 2)
                y = 100;
            else
                y = 700;

            if (juego.turno_actual % 2 == 0)
                x = 100;
            else
                x = 1100;

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
                    carta = Carta{};
                    if (verificarGanador(jugador))
                    {
                        juego.estadoDeJuego = juego_terminado;
                        break;
                        avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                        actualizarVisibilidadCartas(juego);
                        break;
                    }
                }

                if (CheckCollisionPointRec(GetMousePosition(), zona.zonaMazo) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (!tieneCartaJugable(jugador, juego.cartaEnJuego))
                    {
                        intentarRobarYCambiarTurno(juego);
                    }
                }
            }

            EndDrawing();
        }
    }
}

void dibujarCartasJugador(const Jugador &jugador, int xInicial, int yInicial, bool mostrarTodas)
{
    int espacioX = 100;
    for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
    {
        if (!mostrarTodas && !jugador.mano[i].visible)
            continue;
        const Carta &carta = jugador.mano[i];
        if (carta.color.empty())
            continue;
        int x = xInicial + i * ESPACIO_X;
        int y = yInicial;
        DrawRectangle(x, y, CARTA_ANCHO, CARTA_ALTO, LIGHTGRAY);
        DrawRectangleLines(x, y, CARTA_ANCHO, CARTA_ALTO, BLACK);
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
        int textoX = x + (80 / 2) - (anchoTexto / 2);
        int textoY = y + 50;
        DrawText(textoCarta.c_str(), textoX, textoY, 20, colorTexto);
    }
}

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

bool sePuedeJugar(Carta actual, Carta elegida)
{
    if (elegida.tipo == Cambio_color || elegida.tipo == Carta_Mas_cuatro)
        return true;
    if (elegida.color == actual.color)
        return true;
    if (elegida.tipo == Numero && actual.tipo == Numero && elegida.valor == actual.valor)
        return true;
    if (elegida.tipo == actual.tipo && elegida.color == actual.color)
        return true;
    return false;
}

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
            {
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

void dibujarZonaDescarte(const Carta &carta, int x, int y)
{
    if (carta.color.empty())
        return;

    else
    {
        DrawRectangle(x, y, 80, 120, LIGHTGRAY);
        DrawRectangleLines(x, y, 80, 120, BLACK);
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
        DrawText(textoCarta.c_str(), x + 30, y + 50, 30, colorTexto);
    }
}

void avanzarTurno(int &jugadorActual, int direccion, int totalJugadores, Juego_UNO &juego)
{
    jugadorActual = (jugadorActual + direccion + totalJugadores) % totalJugadores;
    juego.turno_actual = jugadorActual;
    actualizarVisibilidadCartas(juego);
}

ZonaVisual obtenerZonaVisual()
{
    ZonaVisual zona;
    zona.zonaMazo = {1000, 300, 80, 120};
    zona.xDescarte = 1100;
    zona.yDescarte = 300;
    return zona;
}

void actualizarEstadisticas(EstadisticasJugador &stats, bool ganoPartida, int minijuegosJugadosEnPartida)
{
    stats.partidasJugadas++;
    if (ganoPartida)
    {
        stats.partidasGanadas++;
    }
    else
    {
        stats.partidasPerdidas++;
    }
    stats.minijuegosJugados += minijuegosJugadosEnPartida;
    std::ofstream archivo("estadisticas.txt");
    if (archivo.is_open())
    {
        archivo << "Partidas Jugadas: " << stats.partidasJugadas << '\n';
        archivo << "Partidas Ganadas: " << stats.partidasGanadas << '\n';
        archivo << "Partidas Perdidas: " << stats.partidasPerdidas << '\n';
        archivo << "Minijuegos Jugados: " << stats.minijuegosJugados << '\n';
        archivo.close();
    }
}

void ActivarMensaje(MensajeTemporal &mensaje, const string &texto, float duracion)
{
    mensaje.texto = texto;
    mensaje.tiempoRestante = duracion;
    mensaje.activo = true;
}

void DibujarMensaje(MensajeTemporal &mensaje, float deltaTime)
{
    if (mensaje.activo)
    {
        DrawText(mensaje.texto.c_str(), 100, 100, 30, RAYWHITE);
        mensaje.tiempoRestante -= deltaTime;
        if (mensaje.tiempoRestante <= 0)
        {
            mensaje.activo = false;
        }
    }
}
