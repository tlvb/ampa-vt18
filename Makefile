CC=gcc
CFLAGS=--std=gnu99 -Wextra -g
vpath %.c src/
vpath %.h src/

.PHONY: default
default: ampa

ampa: ampa_main.c graphics_base.o gui.o light.o util.o network.o
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h, $^) -lSDL2 -lGL -lGLU -lm
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $(filter-out %.h, $^)

.PHONY: clean
clean:
	-rm *.o
	-rm ampa
