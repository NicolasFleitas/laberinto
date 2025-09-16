#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* inicializar_array(int n) {
    int* vector = malloc(n*sizeof(int));
    return vector;
}

int main() {
    int n,m; 
    scanf("%dIngrese un tamaño para el vector",&n);
    int* mi_vector = inicializar_array(n);    
    srand(time(NULL));
  
    // Suma
    int acumulador = 0;
    for (int i=0; i<n; i++) {
        mi_vector[i] = rand()%10;
        acumulador += mi_vector[i];
    }

    printf("La suma inicial %d\n",acumulador);
    double promedio = (double)acumulador / n;
    printf("El promedio es: %.2f\n",promedio);

    // Redimensionar con realloc
    printf("Ingrese el nuevo tamaño del array:");
    scanf("%d",&m);

    int* temp = realloc(mi_vector, m * sizeof(int));

    if (temp == NULL) {
        printf("Error al dimensionra memoria\n");
        free(mi_vector);
        return 1;    
    }

    mi_vector = temp; // Usando el nuevo puntero

    // Cargar las nuevas posiciones
    for (int i = 0; i < m; i++) {
        if (i >= n) { // posiciones nuevas
            mi_vector[i] = rand() % 10;
        }
        acumulador += mi_vector[i];
    }

    printf("Nueva suma = %d\n", acumulador);
    promedio = (double)acumulador / m;
    printf("Nuevo promedio = %.2f\n", promedio);

    free(mi_vector);
    return 0;
}