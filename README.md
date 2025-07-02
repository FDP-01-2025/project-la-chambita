[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/mi1WNrHU)
# Proyecto de C++ - UNO con minijuegos

## Descripción del Proyecto
Este proyecto es una versión digital del clásico juego de cartas UNO, programada en C++ con gráficos usando raylib. Su propósito es ofrecer una experiencia de juego tradicional mejorada con minijuegos interactivos que se activan al jugar ciertas cartas especiales.

El objetivo es quedarse sin cartas antes que los demás, siguiendo las reglas clásicas (combinar por color o número y usar cartas especiales como +2, reversa, salta turno y comodines). Para usar algunas cartas especiales, el jugador debe ganar un minijuego; si pierde, el efecto se le devuelve.

Los minijuegos desafían velocidad, memoria, lógica y reflejos, mezclando estrategia con acción en tiempo real y creando una experiencia única y dinámica para el jugador.

## Equipo

- **Nombre del equipo:** [LA CHAMBITA]

### Integrantes del equipo

1.**Nombre completo:** Aaron Enmanuel Fuentes Murillo "the goat"
  **Carnet:** 00075925

2.**Nombre completo:** Joel Andres Escalante Rivas
  **Carnet:** 00031825

3.**Nombre completo:** Michelle Carolina Escobar Navas
  **Carnet:** 00081518

4.**Nombre completo:** Heberto Johalmo Estrada Hernandez
  **Carnet:** 00138821

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

- descomprime el .zip y muevelo al escritorio
- colócala dentro del repositorio **en la raíz del proyecto**, de modo que quede así:

project-la-chambita/
├── main.cpp
├── README.md
├── archivos/
│   └── ...
├── assets/
│   └── ...
├── include/
│   └── ...
├── src/
│   └── ...
└── raylib/                <----- Carpeta de la librería raylib
    └── install/
        ├── include/
        │   └── raylib.h
        └── lib/
            └── libraylib.a

3. Compilar usando MSYS2 MINGW64 Shell

- abre: MSYS2 MINGW64 Shell.
- muevete entre directorios hacia la carpeta donde clonaste este repositorio:
      cd /c/Users/TU_USUARIO_VA_AQUI/Desktop/project-la-chambita/

      (en el caso de que lo clones en el escritorio)

- compila el proyecto usando:
g++ main.cpp src/juegoUNO.cpp src/minijuego_Palabra.cpp -o UNO -I../install/include -L../install/lib -lraylib -lopengl32 -lgdi32 -lwinmm
 
-ejecuta el programa usando:
./UNO.exe
