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

int main(int argc, char* argv[])
{
	pthread_t Key_t, printInfo_t, playMusic_t;	// Thread IDs 
	int pressedKey;					// Keypress collector
	Mix_Music* music = NULL;			// Place for loading music to
	ThreadArgs *args = calloc(sizeof(ThreadArgs), 1);
	int playing = 1;
	
	if (argc == 1)
		return help(argv[0]);

	initscr();					// Initiating curses window
	cbreak();					// Don't buffer keypress
	noecho();					// Doesn't echo keys
	refresh();					// Refresh the window
	keypad(stdscr, TRUE);				// Allows input every keys
	curs_set(0);

	SDL_Init(SDL_INIT_AUDIO);			// Initialize SDL and SDL audio
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);	// OpenAudio Device
	
	args->pressedKey = &pressedKey;
	args->music = music;
	args->Key_t = &Key_t;
	args->argv = argv;
	args->argvN = 1;
	args->argc = argc;

	playMusic(args);
	Mix_VolumeMusic(50);

	pthread_create(&printInfo_t, NULL, printInfo, args);
	pthread_create(&Key_t, NULL, KeyStrokes, args);

	while (*(args->pressedKey) != 'q')
	{
		if (Mix_PlayingMusic())
			sleep(1);
		else if (args->argc > 1 && Mix_PlayingMusic() == 0)
		{
			if (args->argc > (args->argvN)+1)
			{
				(args->argvN)++;
				playMusic(args);
			}
			else 
				*(args->pressedKey) = 'q';
		}
	}

	echo();
	nocbreak();
	endwin();
	Mix_CloseAudio();
	Mix_FreeMusic(music);
	Mix_Quit();
	SDL_Quit();
	return 0;
}
