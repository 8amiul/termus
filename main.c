#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <unistd.h>
#include <locale.h>
#include <ncurses.h>
#include "termus.h"
#include <string.h>

int argc_;
char** argv_;
int main(int argc, char* argv[])
{
	argc_ = argc;
	argv_ = argv;
	setlocale(LC_ALL, "");

	if (argc > 1 && strcmp(argv[1], "-h") == 0)			// Help page print if argument is provided 
		return help(argv[0]);

	getMusic_STRUCT* getMusicP = getMusic();			// getMusic() returning getMusic_STRUCT

	WIN_STRUCT* window = initScreen();					// init screen

	Mix_Music* musicSDL = NULL;							// musicSDL varible for loading music
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioDeviceID audio = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);	// SDL audio init
	Mix_VolumeMusic(DEFAULT_VOLUME);
	

	display(window, getMusicP, musicSDL);				// The main function managing display, loop, controls
	
	delwin(window[0].win);								// Memory clean up
	delwin(window[1].win);
	endwin();
	exit_curses(1);

	Mix_CloseAudio();
	Mix_FreeMusic(musicSDL);
	Mix_Quit();
	SDL_CloseAudioDevice(audio);
	SDL_Quit();

	freeTermus(getMusicP);
	return 0;
}
