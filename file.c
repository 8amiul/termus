#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "termus.h"


getMusic_STRUCT* getMusic(void)												// getMusic function gets all the information needed for 
{																			// loading music. getMusic_STRUCT is defined in "termus.h"
	char** DIRs = malloc(MAX_DIR * sizeof(char*));							// DIRs variable holds the directories where the program looks for music	
	int totalDIR = getDIR(DIRs);											// Total directories. getDIR functions return the number of directories found in config file


	MUSIC_STRUCT* music = malloc(MAX_MUSIC_FILE * sizeof(MUSIC_STRUCT));	// MUSIC_STRUCT defined in "termus.h"
	for (int i = 0; i < MAX_MUSIC_FILE; i++)								// A loop allocating enough space for music file name and path
	{
		music[i].music_file = malloc(MAX_MUSIC_FILE * sizeof(char));
		music[i].path = malloc(MAX_MUSIC_PATH * sizeof(char));
	}
	int totalMusic = scanMusic(DIRs, totalDIR, music);						// scanMusic function scan music files on provided directories and return total music files

	getMusic_STRUCT* getmusicVal = malloc(sizeof(getMusic_STRUCT));			// getMusic_STRUCT get music, dir, totalMusic, totalDir and return it
	getmusicVal->mus = music;
	getmusicVal->dir = DIRs;
	getmusicVal->totalMusic = totalMusic;
	getmusicVal->totalDIR = totalDIR;

	if (totalMusic == 0)
	{
		printf("ERROR: Music file not found in directories\n");
		freeTermus(getmusicVal);
		exit(1);
	}

	sortList(getmusicVal);													// Sort getMusic_STRUCT in ascending order
	return getmusicVal;
}

int getDIR(char** listDIR)
{
	//FILE* file = fopen(CONFIG_FILE, "r");
	FILE* file = NULL;

	char buff[MAX_DIR_LENGTH];												// Buffer for reading lines in config file
	int countDIR = 0;

	if (argc_ >= 1)
	{
		if (argv_[1] != NULL)
		{
			DIR* dirp = opendir(argv_[1]);		
			if (dirp == NULL)												// Error handling if provided argument is invalid
			{
				perror(__func__);
				free(listDIR);
				closedir(dirp);
				exit(errno);
			}
			else															// If directory is provided through argument
			{
				listDIR[0] = malloc(strlen(argv_[1]));
				strcpy(listDIR[0], argv_[1]);
			}
			closedir(dirp);
			return ++countDIR;
		}
		file = fopen(CONFIG_FILE, "r");										// Opening file in read mode
		if (file == NULL)													// Error handling if file is invalid
		{
			free(listDIR);
			printf("Config file not found\n"
			"Please create '%s' and provide directories there or in program argument to scan.\n\n", CONFIG_FILE);
			help(argv_[0]);
			exit(errno);
		}
	}

	while (fgets(buff, sizeof(buff), file) != NULL)							//	A loop reading each lines from file and writing it to buff
	{
		if (buff[0] == '#')													// It will ignore # commented lines
			continue;

		buff[strcspn(buff, "\n")] = '\0';									// It will exchange '\n' with '\0'

		listDIR[countDIR] = malloc(strlen(buff) + 1);						// Allocating space in listDIR with one greater than length of the line

		strcpy(listDIR[countDIR], buff);									// Copying modified string from buff to the end destination listDIR and countDIR++ 
		countDIR++;
		if (countDIR >= MAX_DIR_LENGTH)
			break;
	}
	fclose(file);


	return countDIR;
}
void concat(char* dir, char* musicFile, char* path)							// Merging music file with the path so that it can be used while loading music
{
	/*char* concatPath = malloc(strlen(dir) + strlen(musicFile) + 1);
	strcpy(concatPath, dir);
	strcat(concatPath, "/");	// Unix only
	strcat(concatPath, musicFile);
	strcpy(path, concatPath);*/

	strcpy(path, dir);														// Copying the dir of music file to the path in struct
	strcat(path, "/");														// Adding a '/' as it is unix based
	strcat(path, musicFile);												// Adding the music file

}

int scanMusic(char** DIRs, int totalDIR, MUSIC_STRUCT* music)
{
	int totalMusic = 0;
	int j = 0;
	for (int i = 0; i < totalDIR; i++)
	{
		DIR* dirp = opendir(DIRs[i]);

		struct dirent* file;
		while((file = readdir(dirp)) != NULL)								// reading file names in opened directory
		{

			char* fileName = file->d_name;
			char* file_ext3 = &fileName[strlen(fileName)-4];				// Getting the extensions for 3 letters and 4 letters
			char* file_ext4 = &fileName[strlen(fileName)-5];
			if (!strcmp(file_ext3, ".mp3") || !strcmp(file_ext4, ".flac") || !strcmp(file_ext3, ".ogg"))
			{
				strcpy(music[totalMusic].music_file, file->d_name);					// Copying the name to music_file
				concat(DIRs[i], file->d_name, music[totalMusic].path);				// Concatenate file name with path and save it to path
				totalMusic++;														// totalMusic variable works like a counter for n number of musics
			}
		}
		closedir(dirp);
	}
	return totalMusic;
}
