all : draw curver

draw : shape.o proj2.o
	gcc shape.o proj2.o -o draw -lglut -lGL -lGLU

shape.o : shape.c shape.h
	gcc -c -ggdb shape.c 

proj2.o : proj2.c
	gcc -c proj2.c 

curver : curver.c shape.o
	gcc -ggdb curver.c shape.o -o curver 

movie :
	cat *.ppm | ppmtoy4m -o 1 -n 127 -F 25:1 | mpeg2enc -o movie.mpg
