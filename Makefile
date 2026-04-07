# Nom de l'exécutable
TARGET = labyrinthe

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11

# Dossiers
SRC_DIR = .
API_DIR = ../LAB26_subject/labyrinthe26

# Fichiers sources
SRCS = main.c node.c vector2.c labyrinthe.c player.c ../LAB26_subject/labyrinthe26/labyrinthAPI.c ../LAB26_subject/labyrinthe26/clientAPI.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Règle principale
all: $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation des .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -I$(API_DIR) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all