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
}

void barajarMazo(Juego_UNO &mazo)
{
    random_device rd;
    mt19937 generador(rd());
    shuffle(juego.mazo, juego.mazo + juego.cartasEnMazo, generador);

}

void repartirCartas(Juego_UNO & juego)
{
    const int CARTAS_POR_JUGADOR = 7;
    int CartaActual = 0;
    // recorre cada jugador
    for (int i = 0; i < juego.cantidadJugadores; i++)
    {
        // a cada jugador se le dan 7 cartas
        for (int j = 0; j < CARTAS_POR_JUGADOR; j++)
        {
            if (juego.cartasEnMazo <= 0)
            {
             if (juego.cartasEnMazo <= 0) {
                cout << "Error: no hay suficientes cartas para repartir." << endl;
                return;
            }
            juego.jugadores[i].mano[j] =juego.mazo[--juego.cartasEnMazo];
        }
    }
}

void ingresarNombres (Juego_UNO & juego);
{
    int cantidad;

    // pedir cantidad de jugadores
    do
    {
        cout << "cuantos jugadores van a jugar, de 2 a 4? ";
        cin >> cantidad;
        if (cantidad < 2 || cantidad > 4)
        {
            cout << "numero no valido. tiene que ser entre 2 y cuatro." << endl;
        }
    } while (cantidad < 2 || cantidad > 4);

    cin.ignore();

    // pedir nombre a cada jugador
    for (int i = 0; i < cantidad_jugadores; i++)
    {
        Jugador nuevo;
        cout << "nombre del jugador " << i + 1 << ": " << endl;
        getline(cin, nuevo.nombre);
        nuevo.minijuegos_ganados = 0;
        nuevo.partidas_ganadas = 0;
        nuevo.esTurno = false;
        jugadores.push_back(nuevo);
    }

    // guardar nombres en un archivo
    ofstream archivo("registro_jugadores.txt", ios::app);
    if (archivo.is_open())
    {
        for (const auto &j : jugadores)
        {
            archivo << j.nombre << endl;
        }
        archivo.close();
    }
    else
    {
        cout << "no se pudo abrir el archivo de registro. " << endl;
    }
}

// Esta función dibuja en pantalla todas las cartas que un jugador tiene en su mano
void dibujarCartasJugador(const Jugador &jugador, int xInicial, int yInicial)
{

    // esto defne el espcio horizontal entre una carta y otra para que no se encimen.
    int espacioX = 100;

    // Recorre cada carta de la mano del jugador.
    for (size_t i = 0; i < jugador.mano.size(); i++)
    {
        const Carta &carta = jugador.mano[i];

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