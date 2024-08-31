all:
	gcc main.c file.c termus.c video.c -lncursesw -lSDL2 -lSDL2_mixer -lm -o termus
