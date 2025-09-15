#include <stdio.h>

#define N 5
#define M 5
#define MAX 100

// Representa una posición en la matriz
typedef struct {
    int y, x;
} Nodo;

// Cola para BFS
typedef struct {
    Nodo data[MAX];
    int front, rear;
} Queue;

void initQueue(Queue* q) { q->front = q->rear = 0; }
int isEmpty(Queue* q) { return q->front == q->rear; }
void enqueue(Queue* q, Nodo val) { q->data[q->rear++] = val; }
Nodo dequeue(Queue* q) { return q->data[q->front++]; }

// Direcciones (abajo, arriba, derecha, izquierda)
int direcciones[4][2] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}
};

void bfs(int matriz[N][M], int startY, int startX, int endY, int endX) {
    int visitado[N][M] = {0};
    Nodo anterior[N][M];

    // Inicializamos todos los "anteriores" con -1
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            anterior[i][j].y = -1;
            anterior[i][j].x = -1;
        }
    }

    Queue q;
    initQueue(&q);

    Nodo inicio = {startY, startX};
    enqueue(&q, inicio);
    visitado[startY][startX] = 1;

    int encontrado = 0;

    while (!isEmpty(&q)) {
        Nodo actual = dequeue(&q);

        if (actual.y == endY && actual.x == endX) {
            encontrado = 1;
            break; // llegamos a la meta
        }

        for (int i = 0; i < 4; i++) {
            int ny = actual.y + direcciones[i][0];
            int nx = actual.x + direcciones[i][1];

            if (ny >= 0 && ny < N && nx >= 0 && nx < M &&
                !visitado[ny][nx] && matriz[ny][nx] == 0) {

                visitado[ny][nx] = 1;
                anterior[ny][nx] = actual; // guardamos de dónde venimos
                Nodo vecino = {ny, nx};
                enqueue(&q, vecino);
            }
        }
    }

    // Reconstrucción del camino
    if (encontrado) {
        Nodo camino[MAX];
        int len = 0;

        Nodo actual = (Nodo){endY, endX};
        while (actual.y != -1 && actual.x != -1) {
            camino[len++] = actual;
            actual = anterior[actual.y][actual.x];
        }

        printf("Camino más corto:\n");
        for (int i = len - 1; i >= 0; i--) {
            printf("(%d, %d)\n", camino[i].y, camino[i].x);
        }
    } else {
        printf("No hay camino disponible.\n");
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
