#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
// -- WINDOWS --
void limpiar_pantalla() { system("cls"); }
void pausa(int ms) { Sleep(ms); }
void system_pause() { system("pause"); }
#else
// -- LINUX Y macOS --
#include <unistd.h>
void limpiar_pantalla() { system("clear"); }
void pausa(int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}
void system_pause() { 
    printf("Presiona Enter para continuar..."); 
    // Limpiamos el buffer de entrada antes de esperar el carácter
    while (getchar() != '\n');
    getchar();
}
#endif