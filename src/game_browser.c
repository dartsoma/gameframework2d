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

    for(int i = 0; i < meta_count; i++){

        slog("Title: %s", meta_arr[i].title);
        slog("Desc: %s", meta_arr[i].description);
        slog("Path: %s", meta_arr[i].path);
        slog("Thumbnail: %s", meta_arr[i].thumbnail);
        slog("Executable: %s", meta_arr[i].executable);
        slog("Last Played: %s", meta_arr[i].last_played);

        if( meta_arr[i].preferred_dim[0] == -1 || meta_arr[i].preferred_dim[0] == -1 ){
        slog("Preferred Dimensions: Default \n");
        } else {
        slog("Preferred Dimensions: %d, %d \n",meta_arr[i].preferred_dim[0], meta_arr[i].preferred_dim[1]);
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

    // clear last meta data

    memset(&recent_meta, 0, sizeof(GameMetaData));

    Uint8 _flags;
    DIR* directory;
    struct dirent *entry;



    // src folder, metadata, metadata, and pak file exist


    directory = opendir(path);

    if(directory == NULL){
        slog("open directory failed");
        return 1;
    }

    while ((entry = readdir(directory)) != NULL){

        if(strstr(entry->d_name, "metadata.json") != NULL){
            _flags |= 1;
            slog("Metadata Found.");
        }
        if(strstr(entry->d_name, "assets.pak") != NULL){
            _flags |= 2;
            slog("Asset.pak Found.");
        }
        if(strstr(entry->d_name, "metadata.json") != NULL){
            _flags |= 4;
            slog("Makefile Found.");
        }
        if(strstr(entry->d_name, "src") != NULL){
            _flags |= 8;
            slog("/src Found.");
        }
        if(strstr(entry->d_name, "include") != NULL){
            _flags |= 16;
            slog("/include Found.");
        }
    }

    if ((_flags & 31) != 31){
        slog("Missing key files");
        return 1;
    }


    strcpy(recent_meta.path, path);

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

    if (sj_object_get_string(json, "description") != NULL) {
        strcpy(recent_meta.description, sj_object_get_string(json, "description"));
    } else {
        slog("Invalid Metadata: Requires \"description\"");
        return 1;
    }


    if (sj_object_get_string(json, "version") != NULL) {
        strcpy(recent_meta.version, sj_object_get_string(json, "version"));
    } else {
        slog("Invalid Metadata: Requires \"version\"");
        return 1;
    }

    if (sj_object_get_string(json, "executable") != NULL) {
        strcpy(recent_meta.executable, sj_object_get_string(json, "executable"));
    } else {
        slog("Invalid Metadata: Requires \"executable\"");
        return 1;
    }

    if (sj_object_get_string(json, "thumbnail") != NULL) {
        strcpy(recent_meta.thumbnail, sj_object_get_string(json, "thumbnail"));
    } else {
        slog("Invalid Metadata: Requires \"thumbnail\"");
        return 1;
    }

    if (sj_object_get_string(json, "last-played") != NULL) {
        strcpy(recent_meta.last_played, sj_object_get_string(json, "last-played"));
    } else {
        slog("Invalid Metadata: Requires \"last-played\"");
        return 1;
    }

    if (sj_object_get_int(json, "window-width-override", &recent_meta.preferred_dim[0]) == 0) {
        slog("Invalid Metadata: Requires \"window-width-override\"");
        return 1;
    }

    if (sj_object_get_int(json, "window-height-override", &recent_meta.preferred_dim[1]) == 0) {
        slog("Invalid Metadata: Requires \"window-height-override\"");
        return 1;
    }

    if (sj_object_get_uint8(json, "fullscreen", &recent_meta.fullscreen) == 0) {
        slog("Invalid Metadata: Requires \"fullscreen\"");
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
