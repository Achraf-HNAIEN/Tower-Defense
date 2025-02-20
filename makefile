CC := gcc
CFLAGS := -Wall -g -pedantic -O1
LIBS := -lm -lMLV
SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin
GRAPHSRC := graph.c  

SRC := $(wildcard $(SRCDIR)/*.c) $(SRCDIR)/$(GRAPHSRC)
OBJ := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS := $(wildcard $(INCDIR)/*.h)

$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(BINDIR))

all: $(BINDIR)/game

$(BINDIR)/game: $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@ $(LIBS)

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/game

.PHONY: all clean
