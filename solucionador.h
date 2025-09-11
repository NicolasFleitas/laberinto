// solucionador.h

#ifndef SOLUCIONADOR_H
#define SOLUCIONADOR_H

#include "laberinto.h"

/**
 * @brief Resuelve el laberinto, encontrando el camino de E a S.
 * @param alto El alto del laberinto.
 * @param ancho El ancho del laberinto.
 * @param laberinto El laberinto a resolver.
 * @param animado 1 si se debe mostrar la animación, 0 si no.
 * @return 1 si se encontró solución, 0 en caso contrario.
 */
int resolverLaberinto(int alto, int ancho, char** laberinto, int animado);

#endif // SOLUCIONADOR_H