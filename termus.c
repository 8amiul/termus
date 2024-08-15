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
#include "termus.h"


void* KeyStrokes(void* _arg)
{
	ThreadArgs* arg = (ThreadArgs*) _arg;

	int* c = arg->pressedKey;
	pthread_t* key_t = arg->Key_t;

	refresh();
	*(arg->pressedKey) = getch();
	if (*(arg->pressedKey) == 'q')
	{
	}
	else if (*c == ' ')
	{
		if (Mix_PausedMusic())
			Mix_ResumeMusic();
		else
			Mix_PauseMusic();
	}
	else if (*(arg->pressedKey) == KEY_RIGHT)
	{
		if (((Mix_MusicDuration(arg->music) - Mix_GetMusicPosition(arg->music)) > 20))
			Mix_SetMusicPosition(Mix_GetMusicPosition(arg->music)+20.0);
	}
	else if (*c == KEY_LEFT)
	{
		if (Mix_GetMusicPosition(arg->music) >= 20)
			Mix_SetMusicPosition(Mix_GetMusicPosition(arg->music)-20.0);
	}
	else if (*c == KEY_UP)
	{
		Mix_VolumeMusic(Mix_GetMusicVolume(arg->music) + 10);
	}
	else if (*c == KEY_DOWN)
	{
		Mix_VolumeMusic(Mix_GetMusicVolume(arg->music) - 10);
	}
	else if ((*(arg->pressedKey) == 'l') && (arg->argv)[(arg->argvN)+1] != NULL)
	{
		(arg->argvN)++;
		playMusic(_arg);
	}
	else if (*(arg->pressedKey) == 'h' && arg->argvN > 1)
	{
		(arg->argvN)--;
		playMusic(_arg);
	}
	pthread_create(key_t, NULL, KeyStrokes, _arg);
}


void* printInfo(void* _arg)
{
	ThreadArgs* arg = (ThreadArgs*) _arg;

	int* c = arg->pressedKey;
	double musicDuration = Mix_MusicDuration(arg->music);
	double sec, min, hr;
	double t_sec, t_min, t_hr;
	double musicPosition;

	while (*c != 'q')
	{
		musicPosition = Mix_GetMusicPosition(arg->music);

		if (strcmp(Mix_GetMusicTitle(arg->music),""))
			printw("Now playing: %s[%.f]\n", Mix_GetMusicTitle(arg->music), musicPosition);
		else
			printw("Music playing [%.f]\n", musicPosition);
		printw("Volume: %d\n", Mix_GetMusicVolume(arg->music));
		if (Mix_PausedMusic())
			printw("[Paused]\n");
		refresh();
		clear();
		usleep(100000);
	}
}

void* playMusic(void* _arg)
{
	ThreadArgs* arg = (ThreadArgs*) _arg;
	arg->music = Mix_LoadMUS((arg->argv)[arg->argvN]);
	Mix_PlayMusic(arg->music, 0);
	
}


int help(char *arg)
{
	printf("Usage: %s [MUSIC_FILE] ...\n", arg);
	printf("\tKEY_L: Next music\n" 
		"\tKEY_H: Previous music\n"
		"\tKEY_UP: Volume up\n"
		"\tKEY_DOWN: Volume down\n"
		"\tKEY_RIGHT: Forward music\n"
		"\tKEY_LEFT: Rewind music\n");
	return 0;
}
