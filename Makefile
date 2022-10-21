CC = gcc
LD = gcc

CFLAGS = -ansi -Wall -pedantic -c -g -std=c99
LFLAGS = `sdl-config --libs` -lglut -lGLU -lGLEW -lGL  -lm 

OBJS = ass2-base.o sdl-base.o shaders.o objects.o

PROG = ass2-base

default: printblank $(PROG)

printblank:
	@echo "" ; echo "" ; echo "" ; echo "" ; echo "" ; 

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

ass2-base.o: ass2-base.c shaders.h sdl-base.h
	$(CC) $(CFLAGS) ass2-base.c

sdl-base.o: sdl-base.c sdl-base.h
	$(CC) $(CFLAGS) sdl-base.c

shaders.o: shaders.c shaders.h
	$(CC) $(CFLAGS) shaders.c

objects.o: objects.c objects.h
	$(CC) $(CFLAGS) objects.c

clean:
	rm -rf *.o $(PROG)
