# Termus
#### Video Demo: https://youtu.be/GzHTSfeRQZU

## Description:
Termus is a terminal based music player written in C using SDL_mixer and ncurses library.

## Files:
- ```main.c```  : initialize libraries and functions are called
- ```termus.c```: Manage keystrokes, free up memory and helping functions
- ```file.c```  : Manages directories, scan music and prepares music files for playing
- ```video.c``` : Functions regarding displaying windows and playing music
- ```termes.h```: A header file containing functon declaration and structs

## Used Libraries
- ```SDL2 & SDL2_mixer```: SDL2_mixer is used for audio and music management
- ```ncurses```: For displaying windows and text

## Functions

### ```termus.c```
---
```c
void controls(int c, int *highlight, int* j, getMusic_STRUCT* getMusicP, WIN_STRUCT* window, Mix_Music** musicSDL, int* volume, int* curPlaying_music)
```
- ```c```: takes user key press. every key input is saved in 'c'
- ```highlight``` is the variable which is limited. it keeps the index number of highligted song on the screen
- ```getMusicP``` here is used to get the total number of music and getting the music file
- ```window``` is used to get the current height of ```window[1]```. ```window[0]``` is the upper and ```window[1]``` is the lower window
- ```musicSDL``` holds the loaded music. So most of the SDL_mixer function is used using it
- ```volume``` gets the current volume
- ```curPlaying_music``` holds the index number of music playing of ```MUSIC_STRUCT``` elements ```(getMusicP->mus[*curPlaying_music].path)```

```c
void freeTermus(getMusic_STRUCT* getMusicP)
```
- freeTerumus function frees all the allocated memory using malloc. Most of the work is connected with ```getMusic_STRUCT```

```c
int compfn(const void* m1, const void* m2)
```
- ```compfn``` function is used in ```sortList()``` function. it is a function not used in anywhere else than ```compfn```. it compares two string and help ```qsort``` to sort the ```getMusicP```.

```c
void sortList(getMusic_STRUCT* getMusicP)
```
- ```sortList``` a function is used to sort ```getMusicP```

### ```video.c```
---
```c
void printMusicList(WIN_STRUCT* window, getMusic_STRUCT* getMusicP, int c, int* j, int* highlight, int* old_h1)
```
- ```printMusicList``` function prints the list and display it in ```window[1]```. It could simply print strings. But it converts it to ```wchar_t``` just to print wide characters. It wrap text so changing display size would wrap those lines which are larger then screen size.

```c
void playbackDuration(WIN_STRUCT* window, Mix_Music* musicSDL)
```
- It prints the progress time and duration in ```window[0]```. 

```c
void printProgressbar(WIN_STRUCT* window, Mix_Music* musicSDL)
```
- ```printProgressbar``` prints the progress bar in ```window[0]```.

```c
void display(WIN_STRUCT* window, getMusic_STRUCT* getMusicP, Mix_Music* musicSDL)
```
- main loop for display and run operate video and music task

```c
WIN_STRUCT* initScreen()
```
- initialize curses and return ```WIN_STRUCT*```

### ```file.c```
---

```c
getMusic_STRUCT* getMusic(void)
```
- getMusic function gets all the information needed for loading music

```c
int getDIR(char** listDIR)
```
- this function read config file for directories or check the program arguments

```c
void concat(char* dir, char* musicFile, char* path)
```
- Merging music file with the path so that it can be used while loading music

```c
int scanMusic(char** DIRs, int totalDIR, MUSIC_STRUCT* music)
```
- scan for music files on directories

## How to use this program?

First it has to compile for the system. using ```make``` this program can be compiled on linux. For windows SDL2 library and ncurses is necessary for compiling. One can provide directories to scan by giving it to the program argument or putting it in
```termus.conf``` file. After that running that program will start the program and a menu should be visible.

## Known bugs/Todo
- Resizing terminal the selection bar goes out of the screen.
- On windows resizing terminal breaks the screen.
- Volume isn't capped at 100
