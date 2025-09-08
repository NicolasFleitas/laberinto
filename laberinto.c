/**
 * @file laberinto.c
 * @brief Programa para generar y resolver laberintos aleatorios.
 *
 * Implementa un generador de laberintos usando el algoritmo de Vuelta Atrás Recursiva
 * para garantizar que siempre sean resolubles. Posteriormente, utiliza el mismo
 * algoritmo de backtracking para encontrar y mostrar el camino desde la entrada
 * hasta la salida. El tamaño del laberinto es configurable por el usuario.
 * El programa también mide y reporta los tiempos de ejecución para la generación
 * y la resolución.
 *
 * @author Nico Fleitas
 * @date 08 de Septiembre de 2025
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <string.h> 

 // Para la animación, necesitamos pausar el programa y limpiar la consola.
// Esto se hace de forma diferente en Windows vs. Linux/macOS.
#ifdef _WIN32
#include <windows.h>
#define LIMPIAR_PANTALLA "cls"
#define PAUSA(ms) Sleep(ms)
#else
#include <unistd.h>
#define LIMPIAR_PANTALLA "clear"
#define PAUSA(ms) usleep(ms * 1000)
#endif

// === CONSTANTES ===

 const char MURO = '#';
 const char CAMINO = '*';
 const char JUGADOR = 'J'; // Lo usaremos más adelante
 const char ENTRADA = 'E';
 const char SALIDA = 'S';
 const char RECORRIDO = '.'; // Marcará el camino correcto
 const char VISITADO = 'v'; // Marcará las celdas ya exploradas

// === PROTOTIPOS DE FUNCIONES ===
// Es una buena práctica declarar las funciones antes de usarlas.
// Así, el compilador sabe qué esperar y podemos mantener nuestro ´main´ al principio.

void inicializarLaberinto(int alto, int ancho, char** laberinto);
void visualizarLaberinto(int alto, int ancho, char** laberinto);
void generarLaberinto(int alto, int ancho, char** laberinto);
void cavar(int x, int y, int alto, int ancho, char** laberinto); // Mi funcion recursiva
void liberarMemoria(int alto, char** laberinto);
int resolverLaberinto(int alto, int ancho, char** laberinto,int animado);
int buscarSalida(int y, int x, int alto, int ancho, char** laberinto, int animado);

// === FUNCIÓN PRINCIPAL ===

int main(int argc, char* argv[]) {
    int alto = 21;
    int ancho = 41;
    int animado = 0; 
    
    // Procesamos los argumentos de la línea de comandos
    if (argc > 1) {
        if (argc >= 3) {
            alto = atoi(argv[1]);
            ancho = atoi(argv[2]);
            if (alto % 2 == 0) alto++;
            if (ancho % 2 == 0) ancho++;
        }
        // Comprobamos si el último argumento es "--animado"
        if (strcmp(argv[argc - 1], "--animado") == 0) {
            animado = 1;
            printf("¡Modo animación activado!\n");
        }
    }
    
    if (argc < 3) {
        printf("Usando un tamaño por defecto: %dx%d. \n", alto, ancho);
        printf("Consejo: Puedes ejecutar como './laberinto 25 51' o './laberinto 25 51 --animado'.\n");
    }
    printf("\n");
    
    char** laberinto = (char**)malloc(alto * sizeof(char*));
    if (laberinto == NULL) { return 1; }
    for (int i = 0; i < alto; i++) {
        laberinto[i] = (char*)malloc(ancho * sizeof(char));
        if (laberinto[i] == NULL) { return 1; }
    }

    srand(time(NULL));

    // --- Medición de Tiempo de Generación ---
    clock_t inicio_gen = clock();

    // 1. Llenamos el laberinto de muros.
    inicializarLaberinto(alto, ancho, laberinto);
    // 2. Damos el primero paso para "cavar" el laberinto.
    generarLaberinto(alto, ancho, laberinto);
    // Conectamos la salida
    if (laberinto[alto - 2][ancho - 2]== MURO) {
        laberinto[alto - 2][ancho - 2 ] = CAMINO;
    }
    // Marcamos la entrada y la salida
    laberinto[1][0] = ENTRADA;
    laberinto[alto - 2][ancho - 1] = SALIDA;

    clock_t fin_gen = clock();
    double tiempo_generacion = ((double)(fin_gen - inicio_gen)) / CLOCKS_PER_SEC;
    // -------------------------------------------

    printf("--- Laberinto Generado ---\n");
    visualizarLaberinto(alto, ancho, laberinto);

    printf("\n--- Resolviendo el Laberinto... ---\n");

    // ---- Medición de Tiempo de Resolución ----

    clock_t inicio_sol = clock();
    int solucion_encontrada = resolverLaberinto(alto, ancho, laberinto, animado);
    clock_t fin_sol = clock();
    double tiempo_resolucion = ((double)(fin_sol - inicio_sol)) / CLOCKS_PER_SEC;
    // -------------------------------------------

    if (animado) { system(LIMPIAR_PANTALLA); } // Limpiamos una última vez para la salida final
    visualizarLaberinto(alto, ancho, laberinto);

    if (solucion_encontrada) {
        printf("¡Salida encontrada!\n");
    } else {
        printf("El laberinto no tiene solución. (Esto no debería pasar)\n");
    }

      // --- Reporte de Tiempos ---
    printf("\n--- ANÁLISIS DE RENDIMIENTO ---\n");
    printf("Tiempo de generación: %f segundos.\n", tiempo_generacion);
    printf("Tiempo de resolución: %f segundos.\n", tiempo_resolucion);


    
    // Importante, evita fugas de memoria.
    liberarMemoria(alto, laberinto);
    return 0; // All is well 
}

/**
 * @brief Funcion de entrada para el algoritmo de resolución
 * Inicia la busqueda desde la posicion inicial (1,1).
 */

 int resolverLaberinto(int alto, int ancho, char** laberinto, int animado) {
    // Empezamos la búsqueda desde la primera celda de camino, junto a la Entrada.
    return buscarSalida(1, 1, alto, ancho, laberinto, animado);
 }

 /**
  * @brief Busca la salida usando Backtracking recursivo.
  * 
  * Esta es la funcion clave de resolución. Funciona así:
  * 1. (Caso Base) Sí estamos fuera de los límites, en un muuero o en un sitio ya visitado, volvemos (false).
  * 2. (Caso Base) SI encontramos la SALIDA, hemos terminado (true).
  * 3. Marcamos la celda actual como parte del RECORRIDO (dejamos una "miga de pan").
  * 4. Intentamos movermos recursivamente en las 4 direcciones (Norte, Sur, Este, Oeste).
  * 5. Si algunas de esas llamadas recursivas devuelve 'true', significa que encontró la salida. Devolvemos 'true'.
  * 6. Si ninguna direcciones lleva a la salida, este es un callejón sin salida.
  * Recogemos nuestra "miga de pan" (restauramos la celda a CAMINO) y devolvemos 'false'.
  * 
  * @return int 1 si se encontró la salida, 0 en caso contrario.
  */

  int buscarSalida(int y, int x, int alto, int ancho, char** laberinto, int animado) {
    // --- Casos Base (Condiciones para detener la recursión) ---
   
    // 0.1 ¿Estamos fuera de los limites del laberinto?
    if (y < 0 || y >= alto || x < 0 || x >= ancho) {
        return 0; // Si estamos fuera, este no es un camino válido.
    }

    // 1. ¿Hemos encontrado la SALIDA? (Ahora es seguro comprobarlo)
    if (laberinto[y][x] == SALIDA) {
        return 1; // ¡Éxito!
    }

    // 2. ¿Estamos en un MURO o en un camino que ya forma parte de la solución?
    if (laberinto[y][x] == MURO || laberinto[y][x] == RECORRIDO) {
        return 0; // No podemos pasar por aqui.
    }

    // --- Paso recursivo ---

    // Marcamos la celda actual como parte de nuestro camino tentativo.
    laberinto[y][x] = RECORRIDO;

    if (animado) {
        visualizarLaberinto(alto, ancho, laberinto);
        PAUSA(15); // Pausa de 15 milisegundos para que sea visible
    }

    // Intentamos movernos en las 4 direcciones.
    // Si alguna de estas llamadas encuentra la salida, el 'return 1' se propagará hacia atrás.
    if (buscarSalida(y, x+1, alto, ancho, laberinto, animado)) return 1; // Este
    if (buscarSalida(y+1, x, alto, ancho, laberinto, animado)) return 1; // Sur
    if (buscarSalida(y, x-1, alto, ancho, laberinto, animado)) return 1; // Oeste
    if (buscarSalida(y-1, x, alto, ancho, laberinto, animado)) return 1; // Norte

    // Si llegamos aquí, significa que ninguna direccion funcionó. Es un callejo sin salida.
    // "Recogemos nuestra migaa de pan" y volvemos atrás.
    laberinto[y][x] = CAMINO; // Backtracking: recogemos la "miga de pan"

    if (animado) {
        visualizarLaberinto(alto, ancho, laberinto);
        PAUSA(15);
    }
    return 0; // Informaremos al paso anterior que este camino no lleva ninguna parte.
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
        for (int j = 0; j < ancho; j++) {
            laberinto[i][j] = MURO;
        }
    }
 }

/** 
 * @brief Inicia el proceso de generación del laberinto.
 *
 * Esta función ahora sirve como punto de entrada. Prepara el terreno
 * y luego llama a la función recursiva 'cavar' para que haga el trabajo pesado.
 */

 void generarLaberinto(int alto, int ancho, char** laberinto) {
     // Empezamos a "cavar" en una posicón fija.
     // Elegimos (1,1) para asegurarnos de que siempre esté rodeado por un borde de muros.
     laberinto[1][1] = CAMINO;
     cavar(1,1,alto,ancho,laberinto);
 }

/* @brief Talla los caminos del laberinto usando Backtracking (Vuelta atras recursiva) 
* 
* Esta es la funcion clave del dia 2. Funciona asi:
* 1 - Marca la celda actual (y, x) como un camino.
* 2 - Crea una lista de 4 direcciones (Norte, Sur, Este, Oeste)
* 3 - Baraja esa lista de direcciones de forma aleatoria.
* 4 - Para cada direccion aleatoria:
* a. Calcula la posicion de la nueva celda (2 pasos en esa direccion).
* b. Si la nueva celda esta dentro del laberinto y todavia es un muro: 
* i.    "Cava" el muro que hay entre la celda actual y la nueva.
* ii.   Llama a esta misma funcion (cavar) para la nueva celda.
*
* Este proceso se repite hasta que ya no haya más celdas válidas a las que moverse
* Creando asi caminos que se ramifican por todo el laberinto.
*/
void cavar(int y, int x, int alto, int ancho, char** laberinto) {
    laberinto[y][x] = CAMINO; // Marca la celda actual como camino
    // Define las 4 direcciones posibles (cambios en y, x)
    int direcciones[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // Este, Oeste, Sur, Norte
    // Barajamos las direcciones para que el laberinto sea aleatorio
    for (int i = 0; i < 4; i++) {
        int r =  rand() % 4;
        int tempY = direcciones[i][0];
        int tempX = direcciones[i][1];
        direcciones[i][0] = direcciones[r][0];
        direcciones[i][1] = direcciones[r][1];
        direcciones[r][0] = tempY;
        direcciones[r][1] = tempX;
    }
    // Explora cada direccion
    for (int i = 0; i < 4; i++) {
        int dx = direcciones[i][1];
        int dy = direcciones[i][0];
        // Calculamos la posicion del muro intermedio y de la nueva celda
        int muroX = x + dx;
        int muroY = y + dy;
        int nuevaX = x + dx * 2;
        int nuevaY = y + dy * 2;
        // Verificamos si la nueva celda esta dentro de los limites y es un muro
        if (nuevaY > 0 && nuevaY < alto - 1 && nuevaX > 0 && nuevaX < ancho - 1 && laberinto[nuevaY][nuevaX] == MURO) {
            laberinto[muroY][muroX] = CAMINO; // Tumbamos el muro
            cavar(nuevaY, nuevaX, alto, ancho, laberinto); // LLAMADA RECURSIVA
        }
    }
}

void visualizarLaberinto(int alto, int ancho, char** laberinto) {
    // Usamos un pequeño truco para limpiar la consola en algunos sistemas
    // y que la visualización sea más limpia.
    // system("cls"); // Para Windows
    // system("clear") // Para Linux/macOS

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            printf("%c", laberinto[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * @brief Libera la memoria asignada dinamicamente por el laberinto.
 * Es crucial liberar la memoria en el orden inverso a como la asignamos:
 * primero cada fila, y luego el array de punteros a las filas.
*/
void liberarMemoria(int alto, char** laberinto) {
    for (int i = 0; i < alto; i++) {
        free(laberinto[i]);
    }
    free(laberinto);
}
