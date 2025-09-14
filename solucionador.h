#ifndef SOLUCIONADOR_H
#define SOLUCIONADOR_H

#include "laberinto.h"
// Encuentra el camino desde la Entrada hasta la salida
// Devuelve 1 si se encontró solución, 0 en caso contrario.
int resolverLaberinto(int alto, int ancho, char** laberinto, int animado);
#endif 