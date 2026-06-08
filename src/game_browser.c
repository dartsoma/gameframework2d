#include "simple_logger.h"
#include "gfc_hashmap.h"
#include "game_browser.h"

Uint8 max_id = -1; // assumes no games

int populate_browser() {

    DIR* directory;
    struct dirent *entry;
    char pathname[11] = "./projects";


    directory = opendir(pathname);

    if(directory == NULL){
        slog("open directory failed");
        return 1;
    }
    // get list of possible directories and add them to a queue where they will be filtered and added to a hashmap upon success
    while ((entry = readdir(directory)) != NULL) {


        if (entry->d_type == DT_DIR){
            char relativepath[500] = "";

            // filter current and parent directory

            if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0) {
                continue;
                }

                strcat(relativepath, pathname);
                strcat(relativepath, "/");
                strcat(relativepath, entry->d_name);

            slog("%s\n", relativepath);
        }
    }

    if (closedir(directory) == -1){
        slog("error closing directory");
        return 1;
    }


    // parse them

    // add them to hash map on success / skip on failure
    return 0;
}

/*

// frees table and sets maxid to 0

void free_browser() {


}

int is_valid_game(const char* path) {


    // folder, metadata, pak and makefile/src exists
    if (is_directory_path(path) != 0){
        return 1;
    }

    // metadata is populated

    if ( parse_metadata() != 0){
        return 1;
    }
    // check metadata

    max_id +=1;
    return 0;
}

int is_directory(const char* path){
    struct stat st;
    // check if file path exists and grab its attributes
    if (STAT_FUNC(path, &st) != 0 ) {
        // check if file path is directory
        if (!S_ISDIR(st.st_mode)){
            return 1;
        }
    }

slog("Not a directory or invalid pathing");

return 0;

}


*/
