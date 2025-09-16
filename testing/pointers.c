#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Nivel 1 – Punteros básicos
// Declarar un entero, un puntero a entero y mostrar:
// Dirección de memoria del entero.
// Valor del entero.
// Valor accedido desde el puntero.
// Intercambiar dos variables usando una función que reciba punteros como parámetros.
// Crear una función que reciba un puntero a entero y le asigne un número aleatorio.

void intercambio(int* n1, int* n2){
    int aux = *n1;
    *n1 = *n2;
    *n2 = aux;
}

void cambiarnumero(int* x) {
    *x = rand() % 100;
}

int main () {
    int x = 3;
    int y = 120;
    printf("Valor de x %d : valor de y %d antes del intercambio\n");
    int z = 69;
    int numero = 10;
    int* puntero;
    puntero = &numero;

    srand(time(NULL));

    printf("\nDirección de memoria del entero %p", &numero);
    printf("\nValor del entero %d", numero);
    printf("\nValor accecido desde el puntero %d", *puntero);

    intercambio(&x,&y);
    cambiarnumero(&z);
    
    printf("\nDespués del intercambio: x = %d, y = %d\n", x, y);

    printf("Número aleatorio: %d",z);

    
}