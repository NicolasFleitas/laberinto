#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **inicializar_array(int alto, int ancho)
{
    int **matriz = (int **)malloc(alto * sizeof(int *));
    for (int i = 0; i < alto; i++)
    {
        matriz[i] = malloc(ancho * sizeof(int));
    }
    return matriz;
}

int main()
{
    int F = 0;
    int C = 0;
    printf("FILAS: \n");
    scanf("%d", &F);
    printf("COLUMNAS: \n");
    scanf("%d", &C);

    int **mi_matriz = inicializar_array(F, C);
    srand(time(NULL));

    for (int i = 0; i < F; i++)
    {
        for (int j = 0; j < C; j++)
        {
            mi_matriz[i][j] = rand() % 10;
        }
    }
    // IMPRIMIR
    for (int i = 0; i < F; i++)
    {
        for (int j = 0; j < C; j++)
        {
            printf("%d", mi_matriz[i][j]);
        }
        printf("\n");
    }
    int matriz_trans[F][C];
    // TRANPUESTA
    for (int i = 0; i < F; i++)
    {
        for (int j = 0; j < C; j++)
        {
            matriz_trans[j][i] = mi_matriz[i][j];
        }
    }

    // IMPRIMIR MATRIZ TRANSPUESTA
    printf("Matriz Transpuesta\n");
    for (int i = 0; i < F; i++)
    {
        for (int j = 0; j < C; j++)
        {
            printf("%d", matriz_trans[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < F; i++)
    {
        free(mi_matriz[i]);
    }
    free(mi_matriz);
    return 0;
}