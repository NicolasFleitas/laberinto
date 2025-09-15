// Definir esta macro antes de incluir las cabeceras asegura que nanosleep esté disponible.
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // Necesario para comparar strings (strcmp)
#include <stdbool.h>

#ifdef _WIN32
// --- Código para Windows ---
#include <windows.h>
#define LIMPIAR_PANTALLA "cls"
#define PAUSA(ms) Sleep(ms)
#define SYSTEM_PAUSE system("pause")
#else
// --- Código para Linux y macOS ---
#include <unistd.h>
#define LIMPIAR_PANTALLA "clear"
// Implementación de PAUSA usando nanosleep para alta precisión.
#define PAUSA(ms) do { \
    struct timespec ts; \
    ts.tv_sec = (ms) / 1000; \
    ts.tv_nsec = ((ms) % 1000) * 1000000L; \
    nanosleep(&ts, NULL); \
} while (0)
#define SYSTEM_PAUSE printf("Presiona Enter para continuar..."); getchar()
#endif

// === CONSTANTES ===
const char MURO = '#';
const char CAMINO = '*';
const char ENTRADA = 'E';
const char SALIDA = 'S';
const char RECORRIDO = '.';

typedef struct NodoBFS {
    int y;
    int x;
    struct NodoBFS* anterior;
} NodoBFS;

// === PROTOTIPOS DE FUNCIONES ===
void inicializarLaberinto(int alto, int ancho, char** laberinto);
void visualizarLaberinto(int alto, int ancho, char** laberinto);
void generarLaberinto(int alto, int ancho, char** laberinto);
void cavar(int y, int x, int alto, int ancho, char** laberinto);
void liberarMemoria(int alto, char** laberinto);
int resolverLaberinto(int alto, int ancho, char** laberinto, int animado);
int buscarSalida(int y, int x, int alto, int ancho, char** laberinto, int animado);
int buscarSalida_BFS(int y, int x, int alto, int ancho, char** laberinto, int animado);


// Prototipo de la función auxiliar para la cola
NodoBFS* crearNodo(int y, int x, NodoBFS* anterior);
void liberarCola(NodoBFS* cabeza);


// === FUNCIÓN PRINCIPAL ===
int main(int argc, char* argv[]) {
    int alto = 21;
    int ancho = 41;
    int animado = 0; // Por defecto, la animación está desactivada.

    // --- Variables para la medición de tiempo ---
    #ifdef _WIN32
        LARGE_INTEGER frecuencia;
        LARGE_INTEGER inicio_gen, fin_gen, inicio_sol, fin_sol;
        QueryPerformanceFrequency(&frecuencia); // Obtenemos los ticks por segundo del PC
    #else
        struct timespec inicio_gen, fin_gen, inicio_sol, fin_sol;
    #endif
    double tiempo_generacion, tiempo_resolucion;
    // ------------------------------------------

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
    // --- FIN DE GENERACION

    printf("--- Laberinto Generado ---\n");
    visualizarLaberinto(alto, ancho, laberinto);
    printf("\nEl laberinto ha sido generado.\n");
    SYSTEM_PAUSE;

    // --- Medición de la Resolución ---
    #ifdef _WIN32
        QueryPerformanceCounter(&inicio_sol);
    #else
        clock_gettime(CLOCK_MONOTONIC, &inicio_sol);
    #endif

    // int solucion_encontrada = resolverLaberinto(alto, ancho, laberinto, animado);
    int solucion_encontrada = buscarSalida_BFS(1, 1, alto, ancho, laberinto, animado);

    #ifdef _WIN32
        QueryPerformanceCounter(&fin_sol);
        tiempo_resolucion = (double)(fin_sol.QuadPart - inicio_sol.QuadPart) / frecuencia.QuadPart;
    #else
        clock_gettime(CLOCK_MONOTONIC, &fin_sol);
        tiempo_resolucion = (fin_sol.tv_sec - inicio_sol.tv_sec) + (fin_sol.tv_nsec - inicio_sol.tv_nsec) / 1e9;
    #endif

    // FIN DE RESOLUCIÓN  
    
    system(LIMPIAR_PANTALLA);
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
    
    SYSTEM_PAUSE;
    return 0;
}

// GENERACIÓN DEL LABERINTO

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
    
    int direcciones[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // {dy, dx}
    // (Algoritmo Fisher-Yates).
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        // Intercambiar direcciones[i] con direcciones[j]
        int temp[2];
        temp[0] = direcciones[i][0];
        temp[1] = direcciones[i][1];
        direcciones[i][0] = direcciones[j][0];
        direcciones[i][1] = direcciones[j][1];
        direcciones[j][0] = temp[0];
        direcciones[j][1] = temp[1];
    }
    
    for (int i = 0; i < 4; i++) {
        int dy = direcciones[i][0];
        int dx = direcciones[i][1];
        // Calculamos la coordenada de la celda a dos pasos de distancia.
        int nuevaY = y + dy * 2;
        int nuevaX = x + dx * 2;
        // Comprobamos si la nueva celda es un lugar válido para cavar.
        // Debe estar dentro de los límites y ser todavía un muro.
        if (nuevaY > 0 && nuevaY < alto - 1 && nuevaX > 0 && nuevaX < ancho - 1 && laberinto[nuevaY][nuevaX] == MURO) {
            // Cavamos el muro que está ENTRE la celda actual y la nueva.
            laberinto[y + dy][x + dx] = CAMINO;
            // Llamada recursiva: continuamos cavando desde la nueva celda.
            cavar(nuevaY, nuevaX, alto, ancho, laberinto);
        }
    }
}

// VISUALIZACION

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

// RESOLUCIÓN

int resolverLaberinto(int alto, int ancho, char** laberinto, int animado) {
    if (animado) {
        system(LIMPIAR_PANTALLA);
        visualizarLaberinto(alto, ancho, laberinto);
    }
    PAUSA(1000);
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
        system(LIMPIAR_PANTALLA); // Se limpia la pantalla antes de volver a mostrar el laberinto
        visualizarLaberinto(alto, ancho, laberinto);
        PAUSA(15);
    }
    

    if (buscarSalida(y, x + 1, alto, ancho, laberinto, animado)) return 1;
    if (buscarSalida(y + 1, x, alto, ancho, laberinto, animado)) return 1;
    if (buscarSalida(y, x - 1, alto, ancho, laberinto, animado)) return 1;
    if (buscarSalida(y - 1, x, alto, ancho, laberinto, animado)) return 1;

    laberinto[y][x] = CAMINO; // Backtracking: Marcamos como camino donde no hubo salida

    if (animado) {
        system(LIMPIAR_PANTALLA); // Limpiamos también al retroceder
        visualizarLaberinto(alto, ancho, laberinto);
        PAUSA(15);       
    }    
    return 0;
}

// BFS
int buscarSalida_BFS(int y_inicio, int x_inicio, int alto, int ancho, char** laberinto, int animado) {
    // Definimos una cola simple con nodos
    NodoBFS* cabeza = NULL;
    NodoBFS* cola = NULL;
    
    // Matriz para saber si una celda ya fue visitada
    // Se inicializa a false
    bool** visitado = (bool**)malloc(alto * sizeof(bool*));
    if (visitado == NULL) return 0;
    for (int i = 0; i < alto; i++) {
        visitado[i] = (bool*)malloc(ancho * sizeof(bool));
        if (visitado[i] == NULL) {
            // Liberar memoria ya asignada y retornar
            for(int k = 0; k < i; k++) free(visitado[k]);
            free(visitado);
            return 0;
        }
        for (int j = 0; j < ancho; j++) {
            visitado[i][j] = false;
        }
    }

    // Coordenadas de la entrada
    int y = y_inicio;
    int x = x_inicio;

    // Se crea el primer nodo y se encola
    NodoBFS* nodo_actual = crearNodo(y, x, NULL);
    cabeza = cola = nodo_actual;
    visitado[y][x] = true;

    // Arrays para las 4 direcciones
    int dy[] = {0, 0, 1, -1};
    int dx[] = {1, -1, 0, 0};

    // Bucle principal de BFS
    while (cabeza != NULL) {
        nodo_actual = cabeza;
        y = nodo_actual->y;
        x = nodo_actual->x;
        
        // Quitar de la cola
        cabeza = cabeza->anterior;
        if (cabeza == NULL) {
            cola = NULL;
        }

        if (animado) {
            laberinto[y][x] = RECORRIDO;
            system(LIMPIAR_PANTALLA);
            visualizarLaberinto(alto, ancho, laberinto);
            PAUSA(15);
        }

        // Si encontramos la salida, hemos terminado
        if (laberinto[y][x] == SALIDA) {
            // Reconstruir el camino desde el nodo actual
            NodoBFS* temp = nodo_actual;
            while(temp != NULL){
                if(laberinto[temp->y][temp->x] != ENTRADA && laberinto[temp->y][temp->x] != SALIDA){
                    laberinto[temp->y][temp->x] = CAMINO; // Re-marcar el camino de solución
                }
                temp = temp->anterior;
            }
            liberarCola(nodo_actual);
            
            // Liberar la memoria de la matriz de visitados
            for (int i = 0; i < alto; i++) {
                free(visitado[i]);
            }
            free(visitado);
            
            return 1; // Éxito
        }
        
        // Explorar los vecinos
        for (int i = 0; i < 4; i++) {
            int nuevaY = y + dy[i];
            int nuevaX = x + dx[i];
            
            // Comprobaciones de límites y si la celda es válida
            if (nuevaY >= 0 && nuevaY < alto && nuevaX >= 0 && nuevaX < ancho && 
                (laberinto[nuevaY][nuevaX] == CAMINO || laberinto[nuevaY][nuevaX] == SALIDA) && 
                !visitado[nuevaY][nuevaX]) {

                // Encolar el nuevo nodo
                NodoBFS* nuevo_nodo = crearNodo(nuevaY, nuevaX, NULL);
                if (cola == NULL) {
                    cabeza = cola = nuevo_nodo;
                } else {
                    cola->anterior = nuevo_nodo;
                    cola = nuevo_nodo;
                }
                visitado[nuevaY][nuevaX] = true;
            }
        }
    }

    // Liberar la memoria de la matriz de visitados
    for (int i = 0; i < alto; i++) {
        free(visitado[i]);
    }
    free(visitado);

    return 0; // No se encontró una solución
}

// Funciones auxiliares para la cola
NodoBFS* crearNodo(int y, int x, NodoBFS* anterior) {
    NodoBFS* nuevo = (NodoBFS*)malloc(sizeof(NodoBFS));
    if (nuevo == NULL) return NULL;
    nuevo->y = y;
    nuevo->x = x;
    nuevo->anterior = anterior;
    return nuevo;
}

void liberarCola(NodoBFS* cabeza) {
    NodoBFS* temp;
    while(cabeza != NULL) {
        temp = cabeza;
        cabeza = cabeza->anterior;
        free(temp);
    }
}


void liberarMemoria(int alto, char** laberinto) {
    for (int i = 0; i < alto; i++) {
        free(laberinto[i]);
    }
    free(laberinto);
}