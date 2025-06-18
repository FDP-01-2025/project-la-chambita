// definir o decirle al programa como funciona cada cosa

#include "juegoUNO.h"
#include <algorithm>
#include <random>
#include <string>
#include "raylib.h"

// FUNCIONES:

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

// Esta función dibuja en pantalla todas las cartas que un jugador tiene en su mano
void dibujarCartasJugador(const Jugador &jugador, int xInicial, int yInicial, bool mostrarTodas)
{

    // esto defne el espcio horizontal entre una carta y otra para que no se encimen.
    int espacioX = 100;

    // Recorre cada carta de la mano del jugador.
    for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
    {
        if (!mostrarTodas && !jugador.mano[i].visible)
            continue; // continue: omitir la iteración actual del bucle y pasar a la siguiente iteración.

        const Carta &carta = jugador.mano[i];

        if (carta.color.empty())
            continue;

        /*Calcula la posición de cada carta en el eje X (horizontal) sumando el espacio entre cartas.
        El eje Y permanece fijo para que todas las cartas estén alineadas en fila*/
        int x = xInicial + i * ESPACIO_X;
        int y = yInicial;

        DrawRectangle(x, y, CARTA_ANCHO, CARTA_ALTO, LIGHTGRAY);
        DrawRectangleLines(x, y, CARTA_ANCHO, CARTA_ALTO, BLACK);

        // Se decide el color con el que se dibujará el texto dentro de la carta, según el color de la carta UNO.
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

        // Aquí se traduce el tipo de carta a un texto (o emoji) que aparecerá dentro del rectángulo. se pasa a to string
        string textoCarta;
        switch (carta.tipo)
        {
        case Numero:
            textoCarta = to_string(carta.valor); // convierte el valor de int a string para ponerlo en la carta
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

        // centra el texto horizontalmente en la carta
        int anchoTexto = MeasureText(textoCarta.c_str(), 20);
        int textoX = x + (80 / 2) - (anchoTexto / 2);
        int textoY = y + 50;

        // Muestra el texto o símbolo de la carta dentro del rectángulo.
        DrawText(textoCarta.c_str(), textoX, textoY, 20, colorTexto);
    }

    // Dibuja el nombre del jugador sobre su fila de cartas.
    int anchoNombre = MeasureText(jugador.nombre.c_str(), 20);
    int nombreX = xInicial + ((MAX_CARTAS_POR_JUGADOR * espacioX) - anchoNombre / 2);
    DrawText(jugador.nombre.c_str(), xInicial, yInicial - 30, 20, BLACK);
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

void seleccionarCatidadJugadores(Juego_UNO &juego, bool &cantidadSeleccionada)
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

// Función para comprobar si una carta se puede jugar
bool sePuedeJugar(Carta actual, Carta elegida)
{
    //
    if(elegida.tipo == Cambio_color || elegida.tipo == Carta_Mas_cuatro || elegida.tipo == Carta_Mas_dos || elegida.tipo == Carta_Bloqueo || elegida.tipo == Cambio_direccion)
    return true;

    //coincide color
    if (elegida.color == actual.color)
    return true;

    //si son del mismo numero 
    if(elegida.tipo == Numero && actual.tipo == Numero && elegida.valor == actual.valor)
    return true;

    if(elegida.tipo == actual.tipo)
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
            { // 0.3 (segundos) f(porque es valor float)
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

        // esto dibuja el texto en la carta (casi centrado xd)
        DrawText(textoCarta.c_str(), x + 30, y + 50, 30, colorTexto);
    }
}

bool jugadorRobaSiDaClick(Rectangle zonaMazo, Juego_UNO &juego, int jugador)
{

    // si se usa el mouse en la zona del mazo:
    if (CheckCollisionPointRec(GetMousePosition(), zonaMazo) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {

        // si aun hay cartas en el mazo Y el jugador NO ha excedido su limite de cartaw
        if (juego.cartasEnMazo > 0)
        {
            Jugador &j = juego.jugadores[jugador];

            // enccuentra la primra posicion libre en la baraja del jugador
            for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
            {
                if (j.mano[i].color.empty())
                {                                                 // esto es un espacio vacio
                    j.mano[i] = juego.mazo[--juego.cartasEnMazo]; // restar uno a juego.cartasEnMazo antes de usar su valor
                    j.mano[i].visible = true;                     // la muestra en pantalla
                    return true;                                  // se robo correctamennte
                }
            }
        }
        return false; // que hubo un error al robar
    }
    return false;
}

ZonaVisual obtenerZonaVisual()
{
    ZonaVisual zona;
    zona.zonaMazo = {1000, 300, 80, 120};
    zona.xDescarte = 1100;
    zona.yDescarte = 300;
    return zona;
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
            seleccionarCatidadJugadores(juego, cantidadSeleccionada);
        }
        else if (!nombresCompletos)
        {
            capturarNombresEnLaVentana(juego, jugadorActual, entradaActual, nombresCompletos);
        }
        else
        {
            if (juego.estadoDeJuego == esperando_jugadores)
            {
                repartirCartas(juego);
                juego.estadoDeJuego = turno_normal;
                juego.cartaEnJuego = robarCartaValida(juego);
                actualizarVisibilidadCartas(juego);
            }

            for (int i = 0; i < juego.cantidadJugadores; i++)
            {
                int y = 100 + i * 150;
                bool mostrar = (i == juego.turno_actual);
                dibujarCartasJugador(juego.jugadores[i], 100, y, mostrar);
            }

            DrawRectangleRec(zona.zonaMazo, DARKGRAY);
            DrawText("MAZO", zona.zonaMazo.x + 10, zona.zonaMazo.y + 60, 20, WHITE);

            dibujarZonaDescarte(juego.cartaEnJuego, zona.xDescarte, zona.yDescarte);

            Jugador &jugador = juego.jugadores[juego.turno_actual];

            for (int i = 0; i < MAX_CARTAS_POR_JUGADOR; i++)
            {
                Carta &carta = jugador.mano[i];

                if (!carta.visible || carta.color.empty())
                    continue;

                int x = INICIO_X + i * ESPACIO_X;
                int y = INICIO_Y + juego.turno_actual * 150;

                Rectangle rect = {(float)x, (float)y, (float)CARTA_ANCHO, (float)CARTA_ALTO};

                // TEMPORALLLLL
                DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, RED);

                if (cartaTuvoDobleClick(rect) && sePuedeJugar(juego.cartaEnJuego, carta))
                {
                    //solo para verificar que se tiro en cada turno
                  cout << "Carta jugada: " << carta.color << ", tipo: " << carta.tipo << ", valor: " << carta.valor << endl;

                    // reemplaza la carta dela mano
                    juego.cartaEnJuego = carta;

                    // elimina la carta de la mano
                    carta = Carta{};

                    // avanza el turno
                    avanzarTurno(juego.turno_actual, juego.direccion, juego.cantidadJugadores, juego);
                    actualizarVisibilidadCartas(juego);
                    break; // solo se puede jugar una carta por turno
                }
            }
            jugadorRobaSiDaClick(zona.zonaMazo, juego, juego.turno_actual);
        }

        EndDrawing();
    }
}

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

// Avanza el turno respetando el sentido del juego
void avanzarTurno(int &jugadorActual, int direccion, int totalJugadores, Juego_UNO &juego)
{
    jugadorActual = (jugadorActual + direccion + totalJugadores) % totalJugadores;
    juego.turno_actual = jugadorActual;
    actualizarVisibilidadCartas(juego);
}

Carta robarCartaValida(Juego_UNO &juego)
{

    if (juego.cartasEnMazo <= 0)
    {
        return Carta{};
    }

    while (juego.cartasEnMazo > 0)
    {
        Carta carta = juego.mazo[juego.cartasEnMazo - 1]; // toma la ultima carta
        juego.cartasEnMazo--;                             // disminuye el contador del mazo

        if (sePuedeJugar(carta, juego.cartaEnJuego))
        {
            return carta;
        }
        else
        {
            continue;
        }
    }
    return Carta{};
}
