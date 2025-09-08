/**
* @file laberinto.c
* @brief Día 2: Generador de Laberintos - Fundamentos y Estructura.
* Pasos anteriores: DIA 1
* En este primer día, sentamos las bases de nuestro programa:
* 1. Definimos la estructura de datos para el laberinto (una matriz 2D).
* 2. Creamos una función para inicializar el laberinto, llenándolo completamente de muros.
* 3. Implementamos una función para visualizar el estado actual del laberinto en la consola.
* 4. Damos el primer paso en la generación, creando un punto de partida.
* * Pasos anteriores: DIA 2

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
int resolverLaberinto(int alto, int ancho, char** laberinto);
int buscarSalida(int y, int x, int alto, int ancho, char** laberinto);

// === FUNCIÓN PRINCIPAL ===

int main(int argc, char* argv[]) {
    int alto = 21;
    int ancho = 41;
    
    if (argc == 3) {
        alto = atoi(argv[1]); // atoi convierte un string a un entero
        ancho = atoi(argv[2]);
        // Truco pro: el algoritmo funciona mucho mejor con dimensiones impares
        // si el usuario nos da un numero par, lo convertimos al siguiente impar
        // para asegurar que haya paredes en los caminos.
        if (alto % 2 == 0) alto++;
        if (ancho % 2 == 0) ancho++;

        printf("Generando un laberinto personalizado %dx%d.\n",alto,ancho);
    } else {
        printf("Usando un tamaño por defecto: %dx%d. \n",alto,ancho);
        printf("Consejo: Puedes ejecutar como './laberinto 25 51 para un tamaño personalizado.\n");
    }
    printf("\n");
    
    // Creamos un puntero a punteros de char. 
    // Esto nos permitirá crear una matriz 2D dinámica.
    char** laberinto = (char**)malloc(alto * sizeof(char*));
    if (laberinto == NULL) { return 1; }
    for (int i = 0; i < alto; i++) {
        laberinto[i] = (char*)malloc(ancho * sizeof(char));
        if (laberinto[i] == NULL) { return 1; }
    }

    srand(time(NULL));

    // 1. Llenamos el laberinto de muros.
    inicializarLaberinto(alto, ancho, laberinto);
    // 2. Damos el primero paso para "cavar" el laberinto.
    generarLaberinto(alto, ancho, laberinto);
    // Conectamos la salida
    if (laberinto[alto - 2][ancho - 2]== MURO) {
        laberinto[alto -2][ancho -2 ] == CAMINO;
    }
    // Marcamos la entrada y la salida
    laberinto[1][0] = ENTRADA;
    laberinto[alto - 2][ancho - 1] = SALIDA;
    // 3. Mostramos el resultado en la consola
    printf("--- Laberinto Generado ---\n");

    visualizarLaberinto(alto, ancho, laberinto);

    //  --- Aca resolvemos los laberintos---
    printf("\n--- Resolviendo el Laberinto... ---\n");
    if (resolverLaberinto(alto, ancho, laberinto)) {
        visualizarLaberinto(alto, ancho, laberinto);
        printf("Salida encontrada!\n");
    } else {
        printf("El laberinto no tiene solución. (Esto no deberia pasar)\n");
    }
    // 4. Liberamos la memoria que reservamos. ¡Muy importante para no tener fugas de memoria!
    liberarMemoria(alto, laberinto);

    return 0; // All is well 
}

/**
 * @brief Funcion de entrada para el algoritmo de resolución
 * Inicia la busquedaw desde la posicion inicial (1,1).
 */

 int resolverLaberinto(int alto, int ancho, char** laberinto) {
    // Empezamos la búsqueda desde la primera celda de camino, junto a la Entrada.
    return buscarSalida(1, 1, alto, ancho, laberinto);
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

  int buscarSalida(int y, int x, int alto, int ancho, char** laberinto) {
    // --- Casos Base (Condiciones para detener la recursión) ---
    
    // ¡CORRECIÓN ! Esta es la valla de seguridad.

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

    // Intentamos movernos en las 4 direcciones.
    // Si alguna de estas llamadas encuentra la salida, el 'return 1' se propagará hacia atrás.
    if (buscarSalida(y, x+1, alto, ancho, laberinto)) return 1; // Este
    if (buscarSalida(y+1, x, alto, ancho, laberinto)) return 1; // Sur
    if (buscarSalida(y, x-1, alto, ancho, laberinto)) return 1; // Oeste
    if (buscarSalida(y-1, x, alto, ancho, laberinto)) return 1; // Norte

    // Si llegamos aquí, significa que ninguna direccion funcionó. Es un callejo sin salida.
    // "Recogemos nuestra migaa de pan" y volvemos atrás.
    laberinto[y][x] = CAMINO;

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
