EXE=Tetris
CC=cmoc
FLAGS=-I./ 

tetris: main.c tetris.c tetris.h
	$(CC) $(FLAGS)  tetris.c main.c
