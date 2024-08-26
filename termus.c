#include <SDL2/SDL_mixer.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "termus.h"
#include "video.h"

/*
	control functions takes c, highlight, j, getMusicP, window, musicSDL, volume, curPlaying_music
	'c' takes user key press. every key input is saved in 'c'. 'highlight' is the variable which is limited.
	it keeps the index number of highligted song on the screen. 'getMusicP' here is used to get the total number of music and getting the music file.
	'window' is used to get the current height of window[1]. window[0] is the upper and window[1] is the lower window.
	'musicSDL' holds the loaded music. So most of the SDL_mixer function is used using it. 'volume' gets the current volume.
	'curPlaying_music' holds the index number of music playing of MUSIC_STRUCT elements (getMusicP->mus[*curPlaying_music].path)
*/
void controls(int c, int *highlight, int* j, getMusic_STRUCT* getMusicP, WIN_STRUCT* window, Mix_Music** musicSDL, int* volume, int* curPlaying_music)
{
	if ((c == 's' | c == KEY_DOWN)&& (*highlight)+(*j) < getMusicP->totalMusic-1)
	{
	if (*highlight > window[1].h-4)
		(*j)++;
	else
		(*highlight)++;
	}
	if (c == 'w' | c == KEY_UP)
	{
		if (*highlight == 0 && (*j) != 0)
			(*j)--;
		else if (*highlight != 0)
			(*highlight)--;
	}
	if (c == KEY_RIGHT)
	{
		double dur = Mix_MusicDuration(*musicSDL);
		double pos = Mix_GetMusicPosition(*musicSDL); 
		if ((dur - pos) > SEEK)
			Mix_SetMusicPosition(pos+SEEK);
	}
	if (c == KEY_LEFT)
	{
		double dur = Mix_MusicDuration(*musicSDL);
		double pos = Mix_GetMusicPosition(*musicSDL); 
		if (pos >= SEEK)
			Mix_SetMusicPosition(pos-SEEK);
	}
	if (c == '0')
	{
		*volume += SEEK_VOL;
		Mix_VolumeMusic(*volume);
	}
	if (c == '9')
	{
		*volume -= SEEK_VOL;
		Mix_VolumeMusic(*volume);
	}
	if (c == '\n')
	{
		Mix_FreeMusic(*musicSDL);
		*curPlaying_music = (*highlight) + (*j);
		*musicSDL = Mix_LoadMUS(getMusicP->mus[*curPlaying_music].path);
		Mix_PlayMusic(*musicSDL, LOOP);
	}
	if (c == ' ')
	{
		if (Mix_PausedMusic())
			Mix_ResumeMusic();
		else
			Mix_PauseMusic();
	}
	if (c == 'l' && *curPlaying_music+1 != getMusicP->totalMusic)
	{
		Mix_FreeMusic(*musicSDL);
		(*curPlaying_music)++;
		*musicSDL = Mix_LoadMUS(getMusicP->mus[*curPlaying_music].path);
		Mix_PlayMusic(*musicSDL, LOOP);
	}
	if (c == 'h' && *curPlaying_music > 0)
	{
		Mix_FreeMusic(*musicSDL);
		(*curPlaying_music)--;
		*musicSDL = Mix_LoadMUS(getMusicP->mus[*curPlaying_music].path);
		Mix_PlayMusic(*musicSDL, LOOP);
	}
	if (c == 'm')				//NEED FIX HERE. TODO -> SETUP A MUTE SYSTEM AND SET VOLUME LIMITED TO 100
	{
		if (*volume != 0)
			Mix_VolumeMusic(0);
		else
			Mix_VolumeMusic(*volume);

	}
}

/*
	freeTerumus function frees all the allocated memory using malloc.
	Most of the work is connected with getMusic_STRUCT
*/
void freeTermus(getMusic_STRUCT* getMusicP)
{
	for (int i = 0; i <getMusicP->totalDIR; i++)
		free(getMusicP->dir[i]);
	free(getMusicP->dir);

	for (int i = 0; i < MAX_MUSIC_FILE; i++)
	{
		free(getMusicP->mus[i].music_file);
		free(getMusicP->mus[i].path);
	}
	free(getMusicP->mus);
	free(getMusicP);
}

/*
	compfn function is used in sortList() function. it is a function not used in anywhere else than compfn.
	it compares two string and help qsort to sort the getMusicP.
*/
int compfn(const void* m1, const void* m2)
{
	return strcmp(*(const char**) m1, *(const char**) m2);
}

/*
	sortList a function is used to sort getMusicP
*/
void sortList(getMusic_STRUCT* getMusicP)
{
	qsort(getMusicP->mus, getMusicP->totalMusic, sizeof(getMusicP->mus[0]), compfn);
}

int help(char* arg)
{
	printf("Usage: %s [-h] music-directory\n", arg);
	printf("\n -h: Displays this message\n");
	printf("\n KEY_0\t\t\t: Volume up\n"
		" KEY_9\t\t\t: Volume down\n"
		" KEY_UP & KEY_DOWN\t: Navigate music list\n"
		" KEY_RIGHT\t\t: Rewind music\n"
		" KEY_LEFT\t\t: Forward music\n"
		" KEY_H\t\t\t: Previous music\n"
		" KEY_L\t\t\t: Next music\n"
		" KEY_SPACE\t\t: Pause\n"
		" KEY_ENTER\t\t: Play\n"
		"\nIn '%s' file music directory path should be saved\n"
		"Path can be ignored if it is commented with '#'\n", CONFIG_FILE);
	return 0;
}
