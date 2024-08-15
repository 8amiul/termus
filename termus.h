#ifndef TERMUS_H
#define TERMUS_H

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include <ncursesw/curses.h>
#elif __linux__
#include <curses.h>
#else
#error "Unsupported operating system"
#endif

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <string.h>


struct ThreadArgs
{
	int* pressedKey;
	Mix_Music* music;
	pthread_t* Key_t;
	char** argv;
	int argc;
	int argvN;
};

typedef struct ThreadArgs ThreadArgs;

void* KeyStrokes(void* _arg);
void* printInfo(void* _arg);
void* playMusic(void* _arg);
int help(char* arg);

#endif
