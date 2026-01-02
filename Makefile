## Makefile for the final project, Andre Mayard

project: project.c
	gcc project.c gfx2.o -lm -lX11 -o project
clean:
	rm project
