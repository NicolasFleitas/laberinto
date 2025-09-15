// BFS PRACTICA
#include <stdio.h>
#define MAX 100 // tamaño máximo de la cola
#define N 5
#define M 5
// Un nodo representa una posición en la matriz
typedef struct
{
    int y;
    int x;
} Nodo;

// Estructura de cola (queue) para guardar posiciones
typedef struct {
    Nodo data[MAX];
    int front; // Indice del primero
    int rear; // índice del último + 1
} Queue;
// Inicializa la cola vacía
void inicializarQueue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}
// Verifica si la cola está vacia
int isEmpty(Queue* q) {
    return q->front == q->rear;
}
// Inserta un nodo al final de la cola
void enqueue(Queue* q, Nodo val) {
    q->data[q->rear++] = val;
}
// Saca y devuelve el primer nodo de la cola
Nodo dequeue(Queue* q) {
    return q->data[q->front++];
}

int direcciones[4][2] = {
    {1, 0},   // abajo
    {-1, 0},  // arriba
    {0, 1},   // derecha
    {0, -1}   // izquierda
};


void bfs(int matriz[N][M], int startY, int startX, int endY, int endX) {
   
    int visitado[N][M] = {0}; // matriz para marcar visitados
    Nodo anterior[N][M]; // guarda el padre de cada nodo

    Queue q;
    inicializarQueue(&q);

    // Insertamos el nodo inicial
    Nodo inicio = {startY, startX};
    enqueue(&q, inicio);
    visitado[startY][startX] = 1;

    int encontrado = 0;

    // Mientras haya nodos en la cola
    while(!isEmpty(&q)) {
        Nodo actual = dequeue(&q);
        
        if (actual.y == endY && actual.x == endX) {
            int encontrado = 1;
            break; // Llegamos a la meta
        }

        // Miramos vecinos en 4 direcciones
        for (int i = 0; i < 4; i++) {
            int ny = actual.y + direcciones[i][0];
            int nx = actual.x + direcciones[i][1];

            // Validar limites y que no esté visitado
            if (ny >= 0 && ny < N && 
                nx >= 0 && nx < M &&
                !visitado[ny][nx] && matriz[ny][nx] == 0) {
                
                visitado[ny][nx] = 1;
                anterior[ny][nx] = actual; // guardamos de dónde venimos
                
                Nodo vecino = {ny, nx};
                enqueue(&q, vecino);
            }
        }
    }

    if(encontrado) {
        Nodo camino[MAX];
        int len = 0;

        Nodo actual = {endY, endX};

        while(!(actual.y == startY && actual.x == startX)) {
            camino[len++] = actual;
            actual = anterior[actual.y][actual.x];
        }
        camino[len++] = inicio; // agregamos el inicio

        printf("Camino mas corto (al reves): \n");
        for ( int i = 0; i >= 0; i--) {
            printf("(%d, %d)\n", camino[i].y, camino[i].x);
        }

    } else {
        pritnf("No hay camino disponible.\n");
    }

}

int main() {
    int laberinto[N][M] = {
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 0, 1, 0}
    };

    bfs(laberinto, 0, 0, 4, 4); // de inicio (0,0) a salida (4,4)
    return 0;
}






