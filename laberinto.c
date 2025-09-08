/**
 * @file laberinto.c
 * @brief Programa para generar y resolver laberintos aleatorios.
 *
 * Implementa un generador de laberintos usando el algoritmo de Vuelta Atrás Recursiva
 * para garantizar que siempre sean resolubles. Posteriormente, utiliza el mismo
 * algoritmo de backtracking para encontrar y mostrar el camino desde la entrada
 * hasta la salida. El tamaño del laberinto es configurable por el usuario.
 *
 * Versión Final: Incluye medición de rendimiento y una visualización animada opcional
 * de la resolución del laberinto.
 *
 * @author Tu Programador Senior Guía
 * @date 09 de Septiembre de 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // Necesario para comparar strings (strcmp)

// Para la animación, necesitamos pausar el programa y limpiar la consola.
// Esto se hace de forma diferente en Windows vs. Linux/macOS.
#ifdef _WIN32
#include <windows.h>
#define LIMPIAR_PANTALLA "cls"
#define PAUSA(ms) Sleep(ms)
#define SYSTEM_PAUSE system("pause")
#else
#include <unistd.h>
#define LIMPIAR_PANTALLA "clear"
#define PAUSA(ms) usleep(ms * 3000)
#define SYSTEM_PAUSE printf("Presiona Enter para continuar..."); getchar()
#endif


// === CONSTANTES ===
const char MURO = '#';
const char CAMINO = '*';
const char ENTRADA = 'E';
const char SALIDA = 'S';
const char RECORRIDO = '.';

// === PROTOTIPOS DE FUNCIONES ===
void inicializarLaberinto(int alto, int ancho, char** laberinto);
void visualizarLaberinto(int alto, int ancho, char** laberinto);
void generarLaberinto(int alto, int ancho, char** laberinto);
void cavar(int y, int x, int alto, int ancho, char** laberinto);
void liberarMemoria(int alto, char** laberinto);
int resolverLaberinto(int alto, int ancho, char** laberinto, int animado);
int buscarSalida(int y, int x, int alto, int ancho, char** laberinto, int animado);

// === FUNCIÓN PRINCIPAL ===
int main(int argc, char* argv[]) {
    int alto = 21;
    int ancho = 41;
    int animado = 0; // Por defecto, la animación está desactivada.

    // Procesamos los argumentos de la línea de comandos
    if (argc >= 3) {
        alto = atoi(argv[1]);
        ancho = atoi(argv[2]);
        if (alto % 2 == 0) alto++;
        if (ancho % 2 == 0) ancho++;
        printf("Generando un laberinto personalizado %dx%d.\n", alto, ancho);
    } else {
        printf("Usando un tamaño por defecto: %dx%d. \n", alto, ancho);
        printf("Consejo: Puedes ejecutar como './laberinto 25 51' o './laberinto 25 51 --animado'.\n");
    }

    // Comprobamos si el último argumento es "--animado" independientemente del tamaño
    if (argc > 1 && strcmp(argv[argc - 1], "--animado") == 0) {
        animado = 1;
        printf("¡Modo animación activado!\n");
    }
    
    printf("\n");
    
    char** laberinto = (char**)malloc(alto * sizeof(char*));
    if (laberinto == NULL) { return 1; }
    for (int i = 0; i < alto; i++) {
        laberinto[i] = (char*)malloc(ancho * sizeof(char));
        if (laberinto[i] == NULL) { return 1; }
    }

    srand(time(NULL));

    clock_t inicio_gen = clock();
    inicializarLaberinto(alto, ancho, laberinto);
    generarLaberinto(alto, ancho, laberinto);
    if (laberinto[alto - 2][ancho - 2] == MURO) {
        laberinto[alto - 2][ancho - 2] = CAMINO;
    }
    laberinto[1][0] = ENTRADA;
    laberinto[alto - 2][ancho - 1] = SALIDA;
    clock_t fin_gen = clock();
    double tiempo_generacion = ((double)(fin_gen - inicio_gen)) / CLOCKS_PER_SEC;

    printf("--- Laberinto Generado ---\n");
    visualizarLaberinto(alto, ancho, laberinto);

    // Añadimos una pausa para que el usuario pueda ver el laberinto generado.
    printf("\nEl laberinto ha sido generado.\n");
    SYSTEM_PAUSE;

    printf("\n--- Resolviendo el Laberinto... ---\n");
    
    clock_t inicio_sol = clock();
    int solucion_encontrada = resolverLaberinto(alto, ancho, laberinto, animado);
    clock_t fin_sol = clock();
    double tiempo_resolucion = ((double)(fin_sol - inicio_sol)) / CLOCKS_PER_SEC;

    // Limpiamos la pantalla justo ANTES de mostrar el resultado final para una vista limpia.
    system(LIMPIAR_PANTALLA);
    visualizarLaberinto(alto, ancho, laberinto);

    if (solucion_encontrada) {
        printf("¡Salida encontrada!\n");
    } else {
        printf("El laberinto no tiene solución. (Esto no debería pasar)\n");
    }

    printf("\n--- ANÁLISIS DE RENDIMIENTO ---\n");
    printf("Tiempo de generación: %f segundos.\n", tiempo_generacion);
    printf("Tiempo de resolución: %f segundos.\n", tiempo_resolucion);

    liberarMemoria(alto, laberinto);
    return 0;
}

int resolverLaberinto(int alto, int ancho, char** laberinto, int animado) {
    if (animado) {
        system(LIMPIAR_PANTALLA);
        visualizarLaberinto(alto, ancho, laberinto);
    }
    return buscarSalida(1, 1, alto, ancho, laberinto, animado);
}

int buscarSalida(int y, int x, int alto, int ancho, char** laberinto, int animado) {
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
        system(LIMPIAR_PANTALLA); // Limpiamos la pantalla ANTES de dibujar el nuevo frame
        visualizarLaberinto(alto, ancho, laberinto);
        PAUSA(15); // Pausa de 15 milisegundos para que sea visible
    }

    if (buscarSalida(y, x + 1, alto, ancho, laberinto, animado)) return 1;
    if (buscarSalida(y + 1, x, alto, ancho, laberinto, animado)) return 1;
    if (buscarSalida(y, x - 1, alto, ancho, laberinto, animado)) return 1;
    if (buscarSalida(y - 1, x, alto, ancho, laberinto, animado)) return 1;

    laberinto[y][x] = CAMINO; // Backtracking: recogemos la "miga de pan"

    if (animado) {
        system(LIMPIAR_PANTALLA); // Limpiamos también al retroceder
        visualizarLaberinto(alto, ancho, laberinto);
        PAUSA(15);
    }
    
    return 0;
}

void inicializarLaberinto(int alto, int ancho, char** laberinto) {
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            laberinto[i][j] = MURO;
        }
    }
}

void generarLaberinto(int alto, int ancho, char** laberinto) {
    cavar(1, 1, alto, ancho, laberinto);
}

void cavar(int y, int x, int alto, int ancho, char** laberinto) {
    laberinto[y][x] = CAMINO;
    int direcciones[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int tempY = direcciones[i][0];
        int tempX = direcciones[i][1];
        direcciones[i][0] = direcciones[r][0];
        direcciones[i][1] = direcciones[r][1];
        direcciones[r][0] = tempY;
        direcciones[r][1] = tempX;
    }
    for (int i = 0; i < 4; i++) {
        int dx = direcciones[i][1];
        int dy = direcciones[i][0];
        int nuevaX = x + dx * 2;
        int nuevaY = y + dy * 2;
        if (nuevaY > 0 && nuevaY < alto - 1 && nuevaX > 0 && nuevaX < ancho - 1 && laberinto[nuevaY][nuevaX] == MURO) {
            laberinto[y + dy][x + dx] = CAMINO;
            cavar(nuevaY, nuevaX, alto, ancho, laberinto);
        }
    }
}

void visualizarLaberinto(int alto, int ancho, char** laberinto) {
    // La llamada a system(LIMPIAR_PANTALLA) se ha movido a las funciones
    // que la necesitan (main y buscarSalida) para un mejor control.
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
    for (int i = 0; i < alto; i++) {
        free(laberinto[i]);
    }
    free(laberinto);
}




