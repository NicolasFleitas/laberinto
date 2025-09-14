#include "solucionador.h"
#include "utils.h" 

static int buscarSalida(int y, int x, int alto, int ancho, char** laberinto, int animado) {
    if (y < 0 || y >= alto || x < 0 || x >= ancho) {
        return 0;
    }
    if (laberinto[y][x] == SALIDA) {
        return 1;
    }
    if (laberinto[y][x] == MURO || laberinto[y][x] == RECORRIDO || laberinto[y][x] == ENTRADA) {
        return 0;
    }

    laberinto[y][x] = RECORRIDO;

    if (animado) {
        limpiar_pantalla();
        visualizarLaberinto(alto, ancho, laberinto);
        pausa(15);
    }

    if (buscarSalida(y, x + 1, alto, ancho, laberinto, animado)) return 1;
    if (buscarSalida(y + 1, x, alto, ancho, laberinto, animado)) return 1;
    if (buscarSalida(y, x - 1, alto, ancho, laberinto, animado)) return 1;
    if (buscarSalida(y - 1, x, alto, ancho, laberinto, animado)) return 1;

    laberinto[y][x] = CAMINO; // Backtracking

    if (animado) {
        limpiar_pantalla();
        visualizarLaberinto(alto, ancho, laberinto);
        pausa(15);
    }    
    return 0;
}

int resolverLaberinto(int alto, int ancho, char** laberinto, int animado) {
    if (animado) {
        limpiar_pantalla();
        visualizarLaberinto(alto, ancho, laberinto);
    }
    return buscarSalida(1, 1, alto, ancho, laberinto, animado);
}