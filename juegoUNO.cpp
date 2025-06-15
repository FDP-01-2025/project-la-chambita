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

void capturarNombresEnLaVentana(Juego_UNO &juego, int &jugadorActual, string &entradaActual, bool &nombresCompletos){

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
            continue;

        const Carta &carta = jugador.mano[i];

        if (carta.color.empty())
            continue;

        /*Calcula la posición de cada carta en el eje X (horizontal) sumando el espacio entre cartas.
        El eje Y permanece fijo para que todas las cartas estén alineadas en fila*/
        int x = xInicial + i * espacioX;
        int y = yInicial;

        // Dibuja una carta como un rectángulo de 80 píxeles de ancho y 120 de alto
        DrawRectangle(x, y, 80, 120, LIGHTGRAY);
        DrawRectangleLines(x, y, 80, 120, BLACK);

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
            textoCarta = to_string(carta.valor);
            break;
        case Carta_Mas_dos:
            textoCarta = "➕2️⃣";
            break;
        case Carta_Mas_cuatro:
            textoCarta = "➕4️⃣";
            break;
        case Cambio_color:
            textoCarta = "Color";
            break;
        case Cambio_direccion:
            textoCarta = "🔄";
            break;
        case Carta_Bloqueo:
            textoCarta = "🚫";
            break;
        default:
            textoCarta = "?";
            break;
        }
        // Muestra el texto o símbolo de la carta dentro del rectángulo.
        DrawText(textoCarta.c_str(), x + 30, y + 50, 20, colorTexto);
    }

    // Dibuja el nombre del jugador sobre su fila de cartas.
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

        for(int i = 0; i < 3; i++){
            DrawRectangleRec(botones[i], LIGHTGRAY);
            DrawRectangleLinesEx(botones[i], 2, BLACK); //esta wea es pura estetica. son contornos para rectangulos
            DrawText(TextFormat("%d", i+2),botones[i].x + 35, botones[i].y + 10, 30, BLACK);  //el %d dice que espera un valor entero
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            Vector2 mouse = GetMousePosition();
            for (int i =0; i <3; i++){
                if(CheckCollisionPointRec(mouse, botones[i])){
                    juego.cantidadJugadores = i+2;
                    cantidadSeleccionada = true;
                }
            }
        }
}