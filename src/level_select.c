#include "simple_logger.h"
#include "persistent_data.h"
#include "level.h"
#include "music_manager.h"
#include "level_select.h"

GamePreset gp = {0};



void set_game_preset(const char* name, int winscore, Uint8 gamemode){
    int i;
    gp.gamemode = gamemode;

    gp.winscore = winscore;

    const char* mapname = current_map();

    if (name != NULL){
    mapname = name;
    } else {
    mapname = "__invalid__";
    }

    for (i = 0; i < level_count(); i++){
        if (strcmp(mapname, index_level_name(i)) == 0){
        slog("level found");
            return;
        }
    }
    slog("level doesn't exist");
    return;
}

void set_game_options(Uint8 id){
    gp.options |= 1 << id;
}

Uint8 get_game_options(Uint8 id){
    return 1 & (gp.options >> id);
}


char* current_track(){

    switch (gp.music){

        case 0:
            return "None";
            break;
        case 1:
            return "Kevin Macleod - There it is";
            break;
        case 2:
            return "Kevin Macleod - Wallpaper ";
            break;
        case 3:
            return "Kevin Macleod - Who likes to party";
            break;

    }
    return "unreachable";
}

char* current_gun(Uint8 id){
    int gun = 0;

    if(id == 0){
        gun = gp.pd.gun1;
    } else {
        gun = gp.pd.gun2;
    }

    switch (gun){

        case 0:
            return "N/A";
            break;
        case 1:
            return "RPG";
            break;
        case 2:
            return "One Tap Sniper";
            break;
        case 3:
            return "Semi Auto Rifle";
            break;
        case 4:
            return "Auto Rifle";
            break;
        case 5:
            return "Uzi";
            break;
        case 6:
            return "Shotgun";
            break;
        case 7:
            return "Pistol";
            break;

    }
    return NULL;

}
char* current_passive(){

    switch (gp.pd.passive){

        case 0:
            return "N/A";
            break;
        case 1:
            return "Armor";
            break;
        case 2:
            return "Speed";
            break;
        case 3:
            return "Reload";
            break;
        case 4:
            return "Jump";
            break;
        case 5:
            return "Regen";
            break;
    }
    return NULL;
}

char* current_map(){

    if (index_level_name(gp.map) != NULL){
    return index_level_name(gp.map);
    }

    return "Error!!";

}


char* current_active(){

    switch (gp.pd.active){

        case 0:
            return "N/A";
            break;
        case 1:
            return "Armor";
            break;
        case 2:
            return "Ammo Pack";
            break;
        case 3:
            return "Super Jump";
            break;
        case 4:
            return "Dash";
            break;
        case 5:
            return "Medkit";
            break;
    }
    return NULL;

}

char* current_melee(){

    switch (gp.pd.melee){

        case 0:
            return "N/A";
            break;
        case 1:
            return "Baseball Bat";
            break;
        case 2:
            return "Sword";
            break;
        case 3:
            return "Knife";
            break;
    }
    return NULL;

}


void select_next_gun(Uint8 id){
    int i;
    int prev = 0;
    if (id == 0){
        prev = gp.pd.gun1;
        if (prev == 7) {
            return;
        }
        for (i = gp.pd.gun1+1; i <= 7; i++){
            if (is_weapon_unlocked(i+3) && (i != gp.pd.gun2)) {
                prev = i;
                break;
            }
        }
        gp.pd.gun1 = prev;
        printf("gun1 %d \n", gp.pd.gun1);
    } else {

        prev = gp.pd.gun2;
        if (prev == 7) {
            return;
        }
            for (i = gp.pd.gun2+1; i <= 7; i++){

                if (is_weapon_unlocked(i+3) && (gp.pd.gun1 != i)){
                    prev = i;
                    break;
                }
            }
        gp.pd.gun2 = prev;
        printf("gun2 %d \n", gp.pd.gun2);
    }

}


void select_previous_gun(Uint8 id){

    int i;
    int prev = 0;
    if (id == 0){

        prev = gp.pd.gun1;
        if (prev == 0) {
            return;
        }
        for (i = gp.pd.gun1-1; i >= 0; i--){
            if ((is_weapon_unlocked(i+3) && i != gp.pd.gun2) || i == 0){
                prev = i;
                break;
            }
        }
        gp.pd.gun1 = prev;
        printf("gun1 %d \n", gp.pd.gun1);
    } else {

        prev = gp.pd.gun2;
        if (prev == 0) {
            return;
        }
        for (i = gp.pd.gun2-1; i >= 0; i--){
            if ((is_weapon_unlocked(i+3) && gp.pd.gun1 != i) || i == 0){
                prev = i;
                break;
            }
        }
         gp.pd.gun2 = prev;
         printf("gun2 %d \n", gp.pd.gun2);
    }
}

void select_next_melee(){

    int i;
    int prev = 0;

    prev = gp.pd.melee;
    if (prev == 3) {
        return;
    }
    for (i = gp.pd.melee+1; i <= 3; i++){
        if (is_weapon_unlocked(i+1)) {
            prev = i;
            break;
        }
    }
    gp.pd.melee = prev;

}
void select_previous_melee(){


    int i;
    int prev = 0;

    prev = gp.pd.melee;
    if (prev == 0) {
        return;
    }
    for (i = gp.pd.melee-1; i >= 0; i--){
        if (is_weapon_unlocked(i+1) || i == 0) {
            prev = i;
            break;
        }
    }
    gp.pd.melee = prev;
}

void select_next_passive(){

    int i;
    int prev = 0;

    prev = gp.pd.passive;
    if (prev == 5) {
        return;
    }
    for (i = gp.pd.passive+1; i <= 5; i++){
        if (is_passive_unlocked(i+1)) {
            prev = i;
            break;
        }
    }
    gp.pd.passive = prev;

}

void select_previous_passive(){


    int i;
    int prev = 0;

    prev = gp.pd.passive;
    if (prev == 0) {
        return;
    }
    for (i = gp.pd.passive-1; i >= 0; i--){
        if (is_passive_unlocked(i+1) || i == 0) {
            prev = i;
            break;
        }
    }
    gp.pd.passive = prev;

}

void select_next_active(){


    int i;
    int prev = 0;

    prev = gp.pd.active;
    if (prev == 5) {
        return;
    }
    for (i = gp.pd.active+1; i <= 5; i++){
        if (is_active_unlocked(i+1)) {
            prev = i;
            break;
        }
    }
    gp.pd.active = prev;

}
void select_previous_active(){

    int i;
    int prev = 0;

    prev = gp.pd.active;
    if (prev == 0) {
        return;
    }
    for (i = gp.pd.active-1; i >= 0; i--){
        if (is_active_unlocked(i+1) || i == 0) {
            prev = i;
            break;
        }
    }
    gp.pd.active = prev;

}



void select_next_map(){


    if (gp.map == level_count()) {
        return;
    }
    gp.map++;

}

void select_previous_map(){

    if (gp.map == 0) {
        return;
    }
    gp.map--;
}
void select_next_track(){


    if (gp.music >= 3){
        music_system_stop();
        return;
    }
    gp.music++;
    music_system_play_track(gp.music);
}

void select_previous_track(){


    if (gp.music <= 0){
         music_system_stop();
        return;
    }
    gp.music--;
    music_system_play_track(gp.music);
}
