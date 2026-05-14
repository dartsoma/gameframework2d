
#ifndef __LEVEL_SELECT_H_
#define __LEVEL_SELECT_H_



typedef struct {
    Uint8 gun1;
    Uint8 gun2;
    Uint8 melee;
    Uint8 passive;
    Uint8 active;
} PlayerPreset;


typedef struct {
    PlayerPreset pd;
    Uint8 map;
    int winscore;
    Uint8 gamemode;
    Uint8 options; //1 - big heads | 2 - super speed | 4  - recoil |  8 - one tap | 16 - low grav
    Uint8 music;
} GamePreset;


void select_next_gun(Uint8 id);
void select_previous_gun(Uint8 id);
void select_next_melee();
void select_previous_melee();

void select_next_passive();
void select_previous_passive();
void select_next_active();
void select_previous_active();


void select_next_map();
void select_previous_map();


void select_next_track();
void select_previous_track();



void set_game_preset(const char* name, int winscore, Uint8 gamemode);

void set_game_options(Uint8 id);
Uint8 get_game_options(Uint8 id);

char* current_gun(Uint8 id);
char* current_passive();
char* current_melee();
char* current_map();
char* current_active();
char* current_track();




#endif
