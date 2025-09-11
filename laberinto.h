#ifndef LABERINTO_H
#define LABERINTO_H

// === CONSTANTES ===
// Para usar en cualquier parte del programa
extern const char MURO;
extern const char CAMINO;
extern const char ENTRADA;
extern const char SALIDA;
extern const char RECORRIDO;

// === PROTOTIPOS DE FUNCIONES ===
/**
 * @brief Reserva memoria para un laberinto de las dimensiones dadas.
 * @param alto El número de filas.
 * @param ancho El número de columnas.
 * @return Un puntero doble (char**) al laberinto creado, o NULL si falla.
 */
char** crearLaberinto(int alto, int ancho);

/**
 * @brief Inicializa todas las celdas del laberinto como muros.
 * @param alto El alto del laberinto.
 * @param ancho El ancho del laberinto.
 * @param laberinto El laberinto a inicializar.
 */
void inicializarLaberinto(int alto, int ancho, char** laberinto);

/**
 * @brief Muestra el estado actual del laberinto en la consola.
 * @param alto El alto del laberinto.
 * @param ancho El ancho del laberinto.
 * @param laberinto El laberinto a mostrar.
 */
void visualizarLaberinto(int alto, int ancho, char** laberinto);

/**
 * @brief Libera toda la memoria asignada para el laberinto.
 * @param alto El número de filas del laberinto.
 * @param laberinto El laberinto a liberar.
 */
void liberarMemoria(int alto, char** laberinto);

#endif // LABERINTO_H