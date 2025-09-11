#include <stdio.h>
#include <stdlib.h>
#include "laberinto.h" 

// Definición de las constantes globales
const char MURO = '#';
const char CAMINO = '*';
const char ENTRADA = 'E';
const char SALIDA = 'S';
const char RECORRIDO = '.';

char** crearLaberinto(int alto, int ancho) {
    char** laberinto = (char**)malloc(alto * sizeof(char*));
    if (laberinto == NULL) {
        return NULL;
    }
    for (int i = 0; i < alto; i++) {
        laberinto[i] = (char*)malloc(ancho * sizeof(char));
        if (laberinto[i] == NULL) {
            // Si falla una, liberamos lo que ya habíamos asignado
            for (int j = 0; j < i; j++) {
                free(laberinto[j]);
            }
            free(laberinto);
            return NULL;
        }
    }
    return laberinto;
}

void inicializarLaberinto(int alto, int ancho, char** laberinto) {
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            laberinto[i][j] = MURO;
        }
    }
}

void visualizarLaberinto(int alto, int ancho, char** laberinto) {
    if (ancho > 0 && alto > 0) {
        for (int i = 0; i < alto; i++) {
            for (int j = 0; j < ancho; j++) {
                printf("%c", laberinto[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void liberarMemoria(int alto, char** laberinto) {
    if (laberinto == NULL) return;
    for (int i = 0; i < alto; i++) {
        free(laberinto[i]);
    }
    free(laberinto);
}