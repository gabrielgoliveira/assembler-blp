# Definindo variáveis
CC = gcc
CFLAGS = -Wall
SOURCES = src/main.c src/stack/stack.c src/execution-context/execution_context.c src/interpreter/interpreter.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = tradutor

# Regra principal
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar arquivos objeto e o executável
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
