all:
	gcc main.c termus.c termus.h -I src/include -I src/include/ncursesw -L src/lib -lSDL2 -lSDL2_mixer -lncursesw -o main