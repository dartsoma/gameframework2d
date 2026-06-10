#include "simple_logger.h"
#include "gfc_hashmap.h"
#include "game_browser.h"

GameMetaData *meta_arr;
GameMetaData recent_meta;

int populate_browser() {

    DIR* directory;
    struct dirent *entry;
    char pathname[11] = "./projects";


    directory = opendir(pathname);

    if(directory == NULL){
        slog("open directory failed");
        return 1;
    }

    Uint8 meta_count = 0;
    Uint8 meta_capacity = 0;

    while ((entry = readdir(directory)) != NULL && meta_count < 100) {

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


            // reallocate memory with each valid directory

            if(is_valid_game(relativepath) == 0){
                if (meta_count >= meta_capacity){
                    meta_capacity = meta_capacity == 0 ? 8 : meta_capacity * 2;
                    meta_arr = realloc(meta_arr, meta_capacity * sizeof(GameMetaData));
                    if (!meta_arr){
                        slog("Err: No metadata array");
                        return 1;
                    }
                }
                /*if(recent_meta == NULL){
                    slog("Err: Metadata for %s, is NULL", relativepath);
                    return 1;
                }*/
                meta_arr[meta_count] = recent_meta;
                meta_count++;
            }
            slog("%s\n", relativepath);
        }
    }


    if (closedir(directory) == -1){
        slog("error closing directory");
        return 1;
    }

    return 0;
}



// frees table and sets maxid to 0

int free_browser() {

    free(meta_arr);

    return 0;
}


int is_valid_game(const char* path) {

    // metadata exists and is populated

    char metapath[50];
    strcpy(metapath, path);
    strcat(metapath,"/metadata.json");

    SJson *json;
    json = sj_load(metapath);

    if (!json){
    slog("No metadata found for: %s", path);
    return 1;
    }

    if (sj_object_get_string(json, "title") != NULL) {
        strcpy(recent_meta.title, sj_object_get_string(json, "title"));
    } else {
        slog("Invalid Metadata: Requires \"title\"");
        return 1;
    }

    sj_free(json);

    return 0;
}
/*
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
