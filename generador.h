// generador.h

#ifndef GENERADOR_H
#define GENERADOR_H

#include "laberinto.h" // Necesita saber sobre la estructura del laberinto

/**
 * @brief Genera un laberinto aleatorio usando backtracking recursivo.
 * @param alto El alto del laberinto.
 * @param ancho El ancho del laberinto.
 * @param laberinto El laberinto a modificar.
 */
void generarLaberinto(int alto, int ancho, char** laberinto);

#endif // GENERADOR_H