#include <SDL2/SDL_mixer.h>
#include <time.h>
#define _XOPEN_SOURCE_EXTENDED
#include <stdlib.h>
#include "termus.h"
#include "video.h"
#include <string.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>

/*
	printMusicList function prints the list and display it in window[1].
	It could simply print strings. But it converts it to wchar_t just to print
	wide characters. It wrap text so changing display size would wrap those lines which
	are largert then screen size.
*/
void printMusicList(WIN_STRUCT* window, getMusic_STRUCT* getMusicP, int c, int* j, int* highlight, int* old_h1)
{
	int condition = 0;
	if (getMusicP->totalMusic < window[1].h-2)
		condition = getMusicP->totalMusic;
	else
		condition = window[1].h-2;

	for (int i = 0; i < condition; i++)
	{
		/* 
			This if block will refresh the window if screen 
			size is changed. it has 'old_h1' which helps to get
			the difference. if 'old_h1' isn't same as current height
			then it refreshes the windows.
			it mainly set the list highlight bar in position if terminal size is changed
		*/
		if (*old_h1 != window[1].h)		
		{
			*highlight = window[1].h-3;
			*old_h1 = window[1].h;
			wrefresh(window[0].win);
			wrefresh(window[1].win);
		}
		/*
			getting enough space for converting string to wchar_t plus extra one for null char.
			Allocating space with malloc in wchar_str. Converting char to wchar_t and saving it
			to wchar_str
		*/
		size_t len = mbstowcs(NULL, getMusicP->mus[i+(*j)].music_file, 0) + 1;
		wchar_t *wchar_str = malloc(len * sizeof(wchar_t));
		mbstowcs(wchar_str, getMusicP->mus[i+(*j)].music_file, len);

		if (*highlight == i)
		{
			//NON WIDE AND WRAP STRING PROCESS
			//wattron(window[1].win, A_REVERSE);
			//mvwaddstr(window[1].win, i+1, 1, getMusicP->mus[i+(*j)].music_file);
			//wattroff(window[1].win, A_REVERSE);

			//NON WIDE AND NON WRAP CHTYPE PROCESS
			//for (k = 0; (getMusicP->mus[i+(*j)].music_file)[k] != '\0'; k++)
			//	chstr[k] = (getMusicP->mus[i+(*j)].music_file)[k];
			//chstr[k] = '\0';
			//mvwaddchstr(window[2].win, i+1, 1, chstr);

			//WIDE AND WRAP WCHAR_T PROCESS
			wattron(window[1].win, A_REVERSE);
			mvwaddwstr(window[1].win, i+1, 1, wchar_str);
			wattroff(window[1].win, A_REVERSE);
		}
		else
		{
			//NON WIDE AND WRAP STRING PROCESS
			//mvwaddstr(window[1].win, i+1, 1, getMusicP->mus[i+(*j)].music_file);

			//NON WIDE AND NON WRAP CHTYPE PROCESS
			//for (k = 0; (getMusicP->mus[i+(*j)].music_file)[k] != '\0'; k++)
			//	chstr[k] = (getMusicP->mus[i+(*j)].music_file)[k];
			//chstr[k] = '\0';
			
			//WIDE AND WRAP WCHAR_T PROCESS
			mvwaddwstr(window[1].win, i+1, 1, wchar_str);
		}
		free(wchar_str);
	}
}

/*
	It prints the progress time and duration in window[0]. 
*/
void playbackDuration(WIN_STRUCT* window, Mix_Music* musicSDL)
{
	int timepos = (int) Mix_GetMusicPosition(musicSDL);
	int hour, min, sec;
	hour = timepos / 3600;
	min = (timepos / 60) - (hour * 60);
	sec = timepos - ((timepos / 60) * 60);

	int Dur = (int) Mix_MusicDuration(musicSDL);
	int hourDur, minDur, secDur;
	hourDur = Dur / 3600;
	minDur = (Dur / 60) - (hourDur * 60);
	secDur = Dur - ((Dur / 60) * 60);

	// The 'x' positing in mvwprintw() is set in a unusual way 
	if (Dur >= 3600)
		mvwprintw(window[0].win, 3, window[0].w-19, "%.2d:%.2d:%.2d/%.2d:%.2d:%.2d", hour, min, sec, hourDur, minDur, secDur);
	else
		mvwprintw(window[0].win, 3, window[0].w-13, "%.2d:%.2d/%.2d:%.2d", min, sec, minDur, secDur);
}

/*
	printProgressbar prints the progress bar in window[0].
*/
void printProgressbar(WIN_STRUCT* window, Mix_Music* musicSDL)
{
	double dur = Mix_MusicDuration(musicSDL);
	int pos = (int) Mix_GetMusicPosition(musicSDL);
	int bar_length;
	if (dur < 3600)
		bar_length = getmaxx(window[0].win) - 13-3;
	else
		bar_length = getmaxx(window[0].win) - 19-3;

	mvwhline(window[0].win, 3, 2, ACS_HLINE, round((bar_length / dur) * pos));
	mvwprintw(window[0].win, 3, 2, "|");
	//mvwprintw(window[0].win, 3, bar_length+1, "|");
}


void display(WIN_STRUCT* window, getMusic_STRUCT* getMusicP, Mix_Music* musicSDL)
{
		int j = 0;
		int highlight = 0;	// Index number of highlighted text. To get the music it adds 'j' with it to get the actual index in getMusicP->mus[n]
		int c;				// Manage keystrokes
		int old_h1 = getmaxy(window[1].win);	// Used to find difference and refresh if display size changes
		int volume = DEFAULT_VOLUME;
		int curPlaying_music = 0;
		window[0].win = newwin(HEIGHT_0, WIDTH_0, Y_0, X_0);
		window[1].win = newwin(window[1].h, window[1].w, window[1].y, window[1].x);
		srand(time(NULL));

		while ((c = getch()) != 'q')
		{
			int max_x, max_y;
			getmaxyx(stdscr, max_y, max_x);

			//volume = Mix_GetMusicVolume(musicSDL);

			clear();
			wclear(window[0].win);
			wclear(window[1].win);

			getmaxyx(stdscr, max_y, max_x);

			window[0].w = max_x;

			window[1].w = max_x;
			window[1].h = max_y-HEIGHT_0-1;

			wresize(window[0].win, window[0].h, window[0].w);
			wresize(window[1].win, window[1].h, window[1].w);

			wborder(window[0].win, 0, 0, 0, 0, 0, 0, 0, 0);
			wborder(window[1].win, 0, 0, 0, 0, 0, 0, 0, 0);
			


			if (musicSDL != NULL)
			{
				if (strlen(Mix_GetMusicTitle(musicSDL)) != 0)
					mvwprintw(window[0].win, 1, 2, "%s", Mix_GetMusicTitle(musicSDL));
				else
					mvwprintw(window[0].win, 1, 2, "%s", getMusicP->mus[curPlaying_music].music_file);

				if (Mix_GetMusicVolume(musicSDL) != 0)
					mvwprintw(window[0].win, 2, window[0].w-13, "Vol: %d", volume);
				else
					mvwprintw(window[0].win, 2, window[0].w-13, "Vol: [X]");


				playbackDuration(window, musicSDL);

				if (Mix_PausedMusic())
					mvwprintw(window[0].win, 2, 2, "[Paused]");
				else
					mvwprintw(window[0].win, 2, 2, "[Playing]");

				printProgressbar(window, musicSDL);
			}
			else
			{

				char* x[10] = {"#", "*", "&", "@", "$", "^", "~", "!", "+", "?"};
				wattron(window[0].win, A_BOLD);
				mvwprintw(window[0].win, rand() % window[0].h, rand() % window[0].w, "%s", x[rand() % 10]);
				mvwprintw(window[0].win, rand() % window[0].h, rand() % window[0].w, "%s", x[rand() % 10]);
				mvwprintw(window[0].win, rand() % window[0].h, rand() % window[0].w, "%s", x[rand() % 10]);
				wattroff(window[0].win, A_BOLD);
			}

			printMusicList(window, getMusicP, c, &j, &highlight, &old_h1);
		
			controls(c, &highlight, &j, getMusicP, window, &musicSDL, &volume, &curPlaying_music);

			//Keep playing music until it reaches the end of list
			if (musicSDL != NULL && !Mix_PlayingMusic() && curPlaying_music < getMusicP->totalMusic)
			{
				curPlaying_music++;
				musicSDL = Mix_LoadMUS(getMusicP->mus[curPlaying_music].path);
				Mix_PlayMusic(musicSDL, 0);
			}
		
			wrefresh(window[0].win);
			wrefresh(window[1].win);

			napms(10);
		}
}



WIN_STRUCT* initScreen()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
	
	WIN_STRUCT* window = malloc(2 * sizeof(WIN_STRUCT));
	
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);

	window[0].y = Y_0;
	window[0].x = X_0;
	window[0].w = max_x;
	window[0].h = HEIGHT_0;
	window[0].win = newwin(window[0].h, window[0].w, window[0].y, window[0].x);

	window[1].y = Y_1;
	window[1].x = X_1;
	window[1].w = max_x-1;
	window[1].h = max_y-HEIGHT_0-1;
	window[1].win = newwin(window[1].h, window[1].w, window[1].y, window[1].x);

	return window;
}
