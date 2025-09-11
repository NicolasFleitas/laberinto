// generador.c

#include <stdlib.h> // para rand()
#include "generador.h"

// La función 'cavar' solo es usada por 'generarLaberinto',
// así que la hacemos 'static' para que no sea visible fuera de este archivo.
static void cavar(int y, int x, int alto, int ancho, char** laberinto) {
    laberinto[y][x] = CAMINO;
    int direcciones[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp[2];
        temp[0] = direcciones[i][0];
        temp[1] = direcciones[i][1];
        direcciones[i][0] = direcciones[j][0];
        direcciones[i][1] = direcciones[j][1];
        direcciones[j][0] = temp[0];
        direcciones[j][1] = temp[1];
    }

    for (int i = 0; i < 4; i++) {
        int dy = direcciones[i][0];
        int dx = direcciones[i][1];
        int nuevaY = y + dy * 2;
        int nuevaX = x + dx * 2;

        if (nuevaY > 0 && nuevaY < alto - 1 && nuevaX > 0 && nuevaX < ancho - 1 && laberinto[nuevaY][nuevaX] == MURO) {
            laberinto[y + dy][x + dx] = CAMINO;
            cavar(nuevaY, nuevaX, alto, ancho, laberinto);
        }
    }
}

void generarLaberinto(int alto, int ancho, char** laberinto) {
    cavar(1, 1, alto, ancho, laberinto);
}