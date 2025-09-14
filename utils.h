#ifndef UTILS_H
#define UTILS_H

#define _POSIX_C_SOURCE 199309L

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

void limpiar_pantalla();
void pausa(int ms);
void system_pause();

#endif