#ifndef __GAME_BROWSER_H__
#define __GAME_BROWSER_H__

#include "sys/stat.h"
#include "simple_json.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

// will be listed in a table with numerical id's

typedef struct {
    Uint8 id;
    char title[50];
    char description[500];
    char version[8];

    char path[500]; // relative filepath
    char thumbnail[50]; // thumnail file name
    char executable[50]; // executable name

    char last_played[50]; // XX-XX-XXXX XX:XX:XX
    int preferred_dim[2]; // preffered_dim
    Uint8 fullscreen; // 0 - no 1 - yes
} GameMetaData;

// Create list and populate metadata to a table
int populate_browser();
int free_browser();


/**
 * @brief checks for valid directories
 * @param filepath
 **/
int is_directory(const char* path);


/**
 * @brief checks for valid metadata, asset.pak, and makefile
 * @param filepath
 **/
int is_valid_game(const char* path);

/**
 * @brief read and populate available games using metadata
 **/
int parse_metadata();

#endif
