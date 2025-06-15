[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/mi1WNrHU)
# Proyecto de C++ - UNO con minijuegos

## Descripción del Proyecto

Breve descripción del juego, su propósito, cómo se juega y cualquier otro detalle importante que describa la idea general del proyecto.

**Por ejemplo:**
Este proyecto consiste en un juego de aventuras donde el jugador debe explorar un mundo virtual, recoger objetos, resolver acertijos y enfrentar enemigos. El juego utiliza estructuras de control como `if`, `switch`, y bucles `while` y `for` para generar interacciones dinámicas.

## Equipo

- **Nombre del equipo:** [Nombre del equipo]

### Integrantes del equipo

1. **Nombre completo:** Aaron Enmanuel Fuentes Murillo "the goat"  
   **Carnet:** 00075925

2. **Nombre completo:** [Nombre del estudiante 2]  
   **Carnet:** [Carnet del estudiante 2]

3. **Nombre completo:** [Nombre del estudiante 3]  
   **Carnet:** [Carnet del estudiante 3]

(Agregar más integrantes si es necesario)

## Instrucciones de Ejecución

## requisitos
- [MSYS2](https://www.msys2.org/) con MinGW 64-bit instalado
- Raylib descargado y ubicado en: `project-la-chambita/raylib/`

## 🔧 Configuración (una sola vez)

1. Clona este repositorio en tu máquina local (de preferencia en el escritorio):
   ```bash
   git clone https://github.com/FDP-01-2025/project-la-chambita.git

2. Descarga la carpeta `raylib` desde este enlace de Google Drive
(https://drive.google.com/drive/u/3/folders/1uWnt6Va2Kp9Ir7MU2Te1IuwiFfcBpKq8)
 y colócala dentro del repositorio **en la raíz del proyecto**, de modo que quede así:

project-la-chambita/
├── main.cpp
├── juegoUNO.cpp
├── juegoUNO.h
├── raylib/         <-----   (esta es la carpeta que acabas de descargar)
│   └── install/
│       ├── include/
│       │   └── raylib.h
│       └── lib/
│           └── libraylib.a


3. Compilar usando MSYS2 MINGW64 Shell

- abre: MSYS2 MINGW64 Shell.
- muevete entre directorios hacia la carpeta donde clonaste este repositorio:
      cd /c/Users/TU_USUARIO_VA_AQUI/Desktop/project-la-chambita/

      (en el caso de que lo clones en el escritorio)

- compila el proyecto usando:
g++ main.cpp juegoUNO.cpp -o UNO -I../install/include -L../install/lib -lraylib -lopengl32 -lgdi32 -lwinmm 

-ejecuta el programa usando:
./UNO.exe