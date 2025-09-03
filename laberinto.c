/**
* @file laberinto.c
* @brief Día 1: Generador de Laberintos - Fundamentos y Estructura.
*
* En este primer día, sentamos las bases de nuestro programa:
* 1. Definimos la estructura de datos para el laberinto (una matriz 2D).
* 2. Creamos una función para inicializar el laberinto, llenándolo completamente de muros.
* 3. Implementamos una función para visualizar el estado actual del laberinto en la consola.
* 4. Damos el primer paso en la generación, creando un punto de partida.
*
* Autor: Nicolás Fleitas
* Fecha: 02 de Septiembre de 2025
*/

 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>

// === CONSTANTES ===
 const char MURO = '#';
 const char CAMINO = '*';
 const char JUGADOR = 'J'; // Lo usaremos más adelante

// === PROTOTIPOS DE FUNCIONES ===
// Es una buena práctica declarar las funciones antes de usarlas.
// Así, el compilador sabe qué esperar y podemos mantener nuestro ´main´ al principio.

void inicializarLaberinto(int alto, int ancho, char** laberinto);
void visualizarLaberinto(int alto, int ancho, char** laberinto);
void generarLaberinto(int alto, int ancho, char** laberinto);
void liberarMemoria(int alto, char** laberinto);

// === FUNCIÓN PRINCIPAL ===

int main() {
    int alto = 21;
    int ancho = 41;
    
    printf("¡Bivenvenido al Generador de Laberintos \n! ");
    printf("Dia 1 : Creando la estrucutra inicial  de  %dx%d. \n\n", alto, ancho);    

    // --- Gestión de memoria ---
    // Creamos un puntero a punteros de char. Esto nos permitirá crear una matriz 2D dinámica.
    // Primero, reservamos memoria para las 'filas' ( un array de punteros ).
    char** laberinto = (char**)malloc(alto * sizeof(char*));

    if(laberinto == NULL) {
        printf("Error: no se pudo asignar memoria para las filas. \n");
        return 1; 
    }
    // Luego, para cada fila, reservamos memoria para las 'columnas'.
    for (int i = 0; i < alto; i++) {
        laberinto[i] = (char*)malloc(ancho * sizeof(char));
        if (laberinto[i] == NULL) {
            printf("Error: No se pudo asignar memoria para la fila %d. \n", i);
            // Si falla, debemos liberar la memoria que ya asignamos antes de salir.
            for(int j = 0; j < i ; j++) {
                free(laberinto[j]);
            }
            free(laberinto);
            return 1;
        }
    }

    // Inicalizamos la semilla para los números aleatorios.
    // Usamos time(NULL) para que la semilla sea diferente cada vez que ejecutemos el programa.
    srand(time(NULL));

    // 1. Llenamos el laberinto de muros.
    inicializarLaberinto(alto, ancho, laberinto);

    // 2. Damos el primero paso para "cavar" el laberinto.
    generarLaberinto(alto, ancho, laberinto);

    // 3. Mostramos el resultado en la consola
    visualizarLaberinto(alto, ancho, laberinto);

    // 4. Liberamos la memoria que reservamos. ¡Muy importante para no tener fugas de memoria!
    liberarMemoria(alto, laberinto);

    return 0; // All is well   
}

/**
 * @brief Llena la matriz del laberinto con el carácter del MURO.
 * 
 * Esta función es nuestro punto de partida. Crea un bloque sólido de "roca"
 * del que luego tallaremos los pasillos.
 * @param alto ALtura de la matriz.
 * @param ancho Ancho de la matriz.
 * @param laberinto Puntero a la matriz 2D.
 */

 void inicializarLaberinto(int alto, int ancho, char** laberinto) {
    for (int i = 0; i < alto; i++ ) {
        for (int j = 0; j < ancho; i++) {
            laberinto[i][j] = MURO;
        }
    }
 }

 /**
  * @brief Muestra el laberinto en la consola.
  * 
  * Recorre la matriz y la imprime carácter por carácter. Es nuestra
  * herramienta esencial para ver qué está pasando dentro del programa.
  * @param alto Altura de la matriz
  * @param ancho Ancho de la matriz
  * @param laberinto Puntero a la matriz 2D.
  */


void visualizarLaberinto(int alto, int ancho, char** laberinto) {
    // Usamos un pequeño truco para limpiar la consola en algunos sistemas
    // y que la visualización sea más limpia.
    // system("cls"); // Para Windows
    // system("clear") // Para Linux/macOS

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            print("%c", laberinto[i][j]);
        }
        print("\n"); // Nueva línea al final de cada fila.
    }
    print("\n");
}

/*
* @brief Inicia la generación del laberinto.
*
* Por ahora, esta función solo establece un punto de partida.
* En el Día 2, implementaremos aquí el algoritmo recursivo completo.
* @param alto Altura de la matriz.
* @param ancho Ancho de la matriz
* @param laberinto Puntero a la matriz 2D.
*/

void generarLaberinto(int alto, int ancho, char** laberinto) {
    // Empezamos a "cavar" en una posicón fija.
    // Elegimos (1,1) para asegurarnos de que siempre esté rodeado por un borde de muros.
    laberinto[1][1] = CAMINO;
}