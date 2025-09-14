#ifndef LABERINTO_H
#define LABERINTO_H

// --- CONSTANTES ---
extern const char MURO;
extern const char CAMINO;
extern const char ENTRADA;
extern const char SALIDA;
extern const char RECORRIDO;

// --- PROTOTIPOS DE FUNCIONES ---
char** crearLaberinto(int alto, int ancho);
void inicializarLaberinto(int alto, int ancho, char** laberinto);
void visualizarLaberinto(int alto, int ancho, char** laberinto);
void liberarMemoria(int alto, char** laberinto);

#endif