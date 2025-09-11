# --- Configuración del compilador ---
CC = gcc
RM = rm -f

# --- Nombres de los archivos de tu proyecto ---
OBJETOS = generador.o laberinto.o main.o solucionador.o utils.o
EJECUTABLE = laberinto

# --- Regla principal: compila todo el proyecto ---
all: $(EJECUTABLE)

# --- Regla para crear el ejecutable ---
$(EJECUTABLE): $(OBJETOS)
	$(CC) $(OBJETOS) -o $(EJECUTABLE)

# --- Reglas para compilar los archivos .c en archivos objeto .o ---
generador.o: generador.c
	$(CC) -c generador.c

laberinto.o: laberinto.c
	$(CC) -c laberinto.c

main.o: main.c
	$(CC) -c main.c

solucionador.o: solucionador.c
	$(CC) -c solucionador.c

utils.o: utils.c
	$(CC) -c utils.c

# --- Regla para limpiar los archivos temporales ---
clean:
	$(RM) $(OBJETOS) $(EJECUTABLE)