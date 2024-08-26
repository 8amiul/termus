#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define CONFIG_FILE "termus.conf"
#define MAX_DIR 256
#define MAX_MUSIC_FILE 1024
#define MAX_MUSIC_PATH 1024
#define MAX_DIR_LENGTH 256

#define DEFAULT_VOLUME 50
#define LOOP 0

extern int argc_;
extern char **argv_;

/*
	MUSIC_STRUCT holds the music file name with extension and the path where it is located
*/
struct MUSIC_STRUCT
{
	char *music_file;
	char *path;
};
typedef struct MUSIC_STRUCT MUSIC_STRUCT;

/*
	getMusic_STRUCT holds the directory provided through config file, MUSIC_STRUCT, total directory and music file
*/
struct getMusic_STRUCT
{
	char **dir;
	MUSIC_STRUCT *mus;
	int totalDIR;
	int totalMusic;
};
typedef struct getMusic_STRUCT getMusic_STRUCT;

/*
	WIN_STRUCT use information about windows and curse window itself
*/
struct WIN_STRUCT
{
	int x;
	int y;
	int w;
	int h;
	WINDOW *win;
};
typedef struct WIN_STRUCT WIN_STRUCT;

getMusic_STRUCT *getMusic(void);
int getDIR(char **listDIR);
int scanMusic(char **DIRs, int totalDIR, struct MUSIC_STRUCT *music);
void sortList(getMusic_STRUCT *getMusicP);
void freeTermus(getMusic_STRUCT *getMusicP);

WIN_STRUCT *initScreen();
void display(WIN_STRUCT *window, getMusic_STRUCT *getMusicP, Mix_Music *musicSDL);
void controls(int c, int *highlight, int *j, getMusic_STRUCT *getMusicP, WIN_STRUCT *window, Mix_Music **musicSDL, int *volume, int *curPlaying_music);

Mix_Music *initMusic();

int help(char *arg);
