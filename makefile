CC := gcc
CFLAGS := -Wall -g
LIBS := -lm -lMLV
SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin
GRAPHSRC := graph.c  # Assuming this is your new source file for drawing functions

# Récupérer la liste des fichiers source .c de SRCDIR
SRC := $(wildcard $(SRCDIR)/*.c) $(SRCDIR)/$(GRAPHSRC)
# Convertir la liste des fichiers source en fichiers objets .o
OBJ := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
# Dépendances
DEPS := $(wildcard $(INCDIR)/*.h)

# Créer les répertoires s'ils n'existent pas
$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(BINDIR))

# Règle par défaut
all: $(BINDIR)/game

# Règle pour l'exécutable
$(BINDIR)/game: $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

# Règle pour les fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Nettoyer les fichiers compilés
clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/game

.PHONY: all clean
