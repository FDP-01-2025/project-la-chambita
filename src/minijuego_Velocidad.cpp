#include "raylib.h"
#include <string>
#include <ctime>
#include <fstream>
#include "../include/juegoUNO.h"
#include "../include/minijuego_Velocidad.h"

// Estado interno del minijuego
static bool iniciado = false;
static bool terminado = false;
static bool gano = false;
static float tiempoLimite = 2.0f; // 2 segundos para reaccionar
static float tiempoTranscurrido = 0.0f;
static char teclaCorrecta = 'A';
static int framesDesdeInicio = 0;

// Guardar puntaje
void guardarPuntajeVelocidad(char tecla, bool gano, float tiempo)
{
    std::ofstream archivo("archivos/minijuego_velocidad.txt", std::ios::app);
    if (archivo.is_open())
    {
        archivo << "Tecla presionada: " << tecla << "\n";
        archivo << "Tiempo usado: " << tiempo << "s\n";
        archivo << "Resultado: " << (gano ? "GANASTE" : "PERDISTE") << "\n";
        archivo << "----------------------\n";
        archivo.close();
    }
}

// Inicializar minijuego
void iniciarMinijuegoVelocidad()
{
    // Generar tecla aleatoria entre A-Z
    teclaCorrecta = 'A' + GetRandomValue(0, 25);
    
    tiempoTranscurrido = 0.0f;
    terminado = false;
    gano = false;
    iniciado = true;
    framesDesdeInicio = 0;
}

// Actualizar lógica y dibujar minijuego
void actualizarMinijuegoVelocidad(Jugador &jugador)
{
    framesDesdeInicio++;
    if (!iniciado)
        return;

    float deltaTime = GetFrameTime();
    tiempoTranscurrido += deltaTime;

    // Verificar si se presionó la tecla correcta
    if (IsKeyPressed((int)teclaCorrecta))
    {
        gano = true;
        terminado = true;
        guardarPuntajeVelocidad(teclaCorrecta, true, tiempoTranscurrido);
    }
    // Verificar si se acabó el tiempo
    else if (tiempoTranscurrido >= tiempoLimite)
    {
        terminado = true;
        guardarPuntajeVelocidad('N', false, tiempoLimite);
    }

    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Título
    const char *titulo = "¡MINIJUEGO DE VELOCIDAD!";
    int anchoTitulo = MeasureText(titulo, 40);
    DrawText(titulo, (screenWidth - anchoTitulo) / 2, 100, 40, YELLOW);

    // Instrucciones
    const char *instrucciones = "Presiona la tecla que aparece en pantalla lo más rápido posible";
    int anchoInstrucciones = MeasureText(instrucciones, 25);
    DrawText(instrucciones, (screenWidth - anchoInstrucciones) / 2, 180, 25, LIGHTGRAY);

    // Mostrar la tecla a presionar
    char teclaStr[2] = {teclaCorrecta, '\0'};
    int anchoTecla = MeasureText(teclaStr, 80);
    DrawText(teclaStr, (screenWidth - anchoTecla) / 2, 250, 80, WHITE);

    // Tiempo restante
    float tiempoRestante = tiempoLimite - tiempoTranscurrido;
    if (tiempoRestante < 0) tiempoRestante = 0;
    
    char tiempoStr[50];
    sprintf(tiempoStr, "Tiempo restante: %.1f", tiempoRestante);
    int anchoTiempo = MeasureText(tiempoStr, 30);
    DrawText(tiempoStr, (screenWidth - anchoTiempo) / 2, 350, 30, RED);

    // Barra de progreso del tiempo
    float progreso = tiempoTranscurrido / tiempoLimite;
    if (progreso > 1.0f) progreso = 1.0f;
    
    int barraWidth = 400;
    int barraHeight = 20;
    int barraX = (screenWidth - barraWidth) / 2;
    int barraY = 400;
    
    // Fondo de la barra
    DrawRectangle(barraX, barraY, barraWidth, barraHeight, DARKGRAY);
    // Progreso de la barra
    DrawRectangle(barraX, barraY, (int)(barraWidth * progreso), barraHeight, RED);

    // Mensaje de resultado
    if (terminado && framesDesdeInicio > 1)
    {
        const char *mensaje = gano ? "¡Excelente! Presionaste la tecla a tiempo." : "¡Demasiado lento! Se acabó el tiempo.";
        int anchoMensaje = MeasureText(mensaje, 25);
        DrawText(mensaje, (screenWidth - anchoMensaje) / 2, 450, 25, gano ? DARKGREEN : RED);
        
        if (gano)
        {
            char tiempoUsadoStr[50];
            sprintf(tiempoUsadoStr, "Tiempo usado: %.2f segundos", tiempoTranscurrido);
            int anchoTiempoUsado = MeasureText(tiempoUsadoStr, 20);
            DrawText(tiempoUsadoStr, (screenWidth - anchoTiempoUsado) / 2, 480, 20, LIGHTGRAY);
        }
        
        const char *continuar = "Presiona ENTER para continuar";
        int anchoContinuar = MeasureText(continuar, 20);
        DrawText(continuar, (screenWidth - anchoContinuar) / 2, 520, 20, WHITE);

        if (IsKeyPressed(KEY_ENTER))
        {
            iniciado = false;
        }
    }
}

// Verificar si ya terminó
bool minijuegoVelocidadTerminado()
{
    return terminado;
}

// Verificar si ganó
bool minijuegoVelocidadGano()
{
    return gano;
} 