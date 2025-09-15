#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// Se incluyen todas las cabeceras
#include "laberinto.h"
#include "generador.h"
#include "solucionador.h"
#include "utils.h"
// La medición de tiempo depende del sistema operativo.
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char* argv[]) {
    int alto = 21;
    int ancho = 41;
    int animado = 0;

    // --- Variables para la medición de tiempo ---
    #ifdef _WIN32
        LARGE_INTEGER frecuencia;
        LARGE_INTEGER inicio_gen, fin_gen, inicio_sol, fin_sol;
        QueryPerformanceFrequency(&frecuencia);
    #else
        struct timespec inicio_gen, fin_gen, inicio_sol, fin_sol;
    #endif
    double tiempo_generacion, tiempo_resolucion;

    // Procesamiento de argumentos
    if (argc >= 3) {
        alto = atoi(argv[1]);
        ancho = atoi(argv[2]);
        if (alto % 2 == 0) alto++;
        if (ancho % 2 == 0) ancho++;
        printf("Generando un laberinto personalizado %dx%d.\n", alto, ancho);
    } else {
        printf("Usando un tamaño por defecto: %dx%d. \n", alto, ancho);
        printf("Para generar un laberinto personalizado. Puedes ejecutar: './laberinto 25 51' o './laberinto 25 51 --animado'.\n");
    }

    if (argc > 1 && strcmp(argv[argc - 1], "--animado") == 0) {
        animado = 1;
        printf("---Modo Animación---\n");
    }
    printf("\n");

   // Función del modulo laberinto
    char** laberinto = crearLaberinto(alto, ancho);
    if (laberinto == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el laberinto.\n");
        return 1;
    }

    srand(time(NULL));

    // --- Medición de la Generación ---
    #ifdef _WIN32
        QueryPerformanceCounter(&inicio_gen);
    #else
        clock_gettime(CLOCK_MONOTONIC, &inicio_gen);
    #endif

    inicializarLaberinto(alto, ancho, laberinto);
    generarLaberinto(alto, ancho, laberinto); 
    if (laberinto[alto - 2][ancho - 2] == MURO) {
         laberinto[alto - 2][ancho - 2] = CAMINO;
    }
    laberinto[1][0] = ENTRADA;
    laberinto[alto - 2][ancho - 1] = SALIDA;

    #ifdef _WIN32
        QueryPerformanceCounter(&fin_gen);
        tiempo_generacion = (double)(fin_gen.QuadPart - inicio_gen.QuadPart) / frecuencia.QuadPart;
    #else
        clock_gettime(CLOCK_MONOTONIC, &fin_gen);
        tiempo_generacion = (fin_gen.tv_sec - inicio_gen.tv_sec) + (fin_gen.tv_nsec - inicio_gen.tv_nsec) / 1e9;
    #endif

    printf("--- Laberinto Generado ---\n");
    visualizarLaberinto(alto, ancho, laberinto);
    printf("\nEl laberinto ha sido generado.\n");
    system_pause();

    // --- Medición de la Resolución ---
    #ifdef _WIN32
        QueryPerformanceCounter(&inicio_sol);
    #else
        clock_gettime(CLOCK_MONOTONIC, &inicio_sol);
    #endif

    int solucion_encontrada = resolverLaberinto(alto, ancho, laberinto, animado); // Función del módulo 'solucionador'

    #ifdef _WIN32
        QueryPerformanceCounter(&fin_sol);
        tiempo_resolucion = (double)(fin_sol.QuadPart - inicio_sol.QuadPart) / frecuencia.QuadPart;
    #else
        clock_gettime(CLOCK_MONOTONIC, &fin_sol);
        tiempo_resolucion = (fin_sol.tv_sec - inicio_sol.tv_sec) + (fin_sol.tv_nsec - inicio_sol.tv_nsec) / 1e9;
    #endif

    limpiar_pantalla();
    visualizarLaberinto(alto, ancho, laberinto);

    if (solucion_encontrada) {
        printf("¡Salida encontrada!\n");
    } else {
        printf("El laberinto no tiene solución.\n");
    }

    printf("\n--- ANÁLISIS DE RENDIMIENTO ---\n");
    printf("Tiempo de generación: %f segundos.\n", tiempo_generacion);
    if (animado) {
        printf("Duración de la animación: %f segundos.\n", tiempo_resolucion);
    } else {
        printf("Tiempo de resolución (algoritmo): %f segundos.\n", tiempo_resolucion);
    }
    
    liberarMemoria(alto, laberinto); 

    return 0;
}