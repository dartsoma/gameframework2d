#include "persistent_data.h"


PersistentData pd;


void load_persistent_data(){

memset(&pd, 0, sizeof(PersistentData));

int i;

SJson *json, *value;
json = sj_load("./def/persistent.def");
slog("saving level");

if (!json) {
    slog("bad level def");
}
Uint8 unlocks[20];

sj_object_get_int(json, "money", &pd->money[10]);

sj_object_get_uint8(json, "weapon1", &unlocks[0]);
sj_object_get_uint8(json, "weapon2", &unlocks[1]);
sj_object_get_uint8(json, "weapon3", &unlocks[2]);
sj_object_get_uint8(json, "weapon4", &unlocks[3]);
sj_object_get_uint8(json, "weapon5", &unlocks[4]);
sj_object_get_uint8(json, "weapon6", &unlocks[5]);
sj_object_get_uint8(json, "weapon7", &unlocks[6]);
sj_object_get_uint8(json, "weapon8", &unlocks[7]);
sj_object_get_uint8(json, "weapon9", &unlocks[8]);
sj_object_get_uint8(json, "weapon10", &unlocks[9]);

sj_object_get_uint8(json, "passive1", &unlocks[10]);
sj_object_get_uint8(json, "passive2", &unlocks[11]);
sj_object_get_uint8(json, "passive3", &unlocks[12]);
sj_object_get_uint8(json, "passive4", &unlocks[13]);
sj_object_get_uint8(json, "passive5", &unlocks[14]);

sj_object_get_uint8(json, "active1", &unlocks[15]);
sj_object_get_uint8(json, "active2", &unlocks[16]);
sj_object_get_uint8(json, "active3", &unlocks[17]);
sj_object_get_uint8(json, "active4", &unlocks[18]);
sj_object_get_uint8(json, "active5", &unlocks[19]);



for (i = 0; i < 20; i++) {
    pd->unlocks |= unlocks[i] << i;
}

}

Uint8 is_weapon_unlocked(Uint8 id){
    return (pd.unlocks & (1<<(id-1)));
}

Uint8 is_passive_unlocked(Uint8 id){
    return (pd.unlocks & (1<<(id+9)));
}

Uint8 is_active_unlocked(Uint8 id){
    return (pd.unlocks & (1<<(id+14)));
}

int get_money(){
    return pd.money;
}

void add_money(int m){
    money += m;
}

void remove_money(int m){
    money -= m;
}

void unlock_weapon(Uint8 id){
    pd.unlocks |= (1<<(id-1));
}

void unlock_passive(Uint8 id){
    pd.unlocks |= (1<<(id-9));
}

void unlock_active(Uint8 id){
    pd.unlocks |= (1<<(id-14));
}



void save_data(){

    SJson *new_pd, *value;

    slog("saving data");

    if (!config) return;

    new_pd = sj_object_new();


    value = sj_new_uint8(base->width);

    value = sj_new_uint8(is_weapon_unlocked(1));
    sj_object_insert(new_pd, "weapon1", value);

    value = sj_new_uint8(is_weapon_unlocked(2));
    sj_object_insert(new_pd, "weapon2", value);

    value = sj_new_uint8(is_weapon_unlocked(3));
    sj_object_insert(new_pd, "weapon3", value);

    value = sj_new_uint8(is_weapon_unlocked(4));
    sj_object_insert(new_pd, "weapon4", value);

    value = sj_new_uint8(is_weapon_unlocked(5));
    sj_object_insert(new_pd, "weapon5", value);

    value = sj_new_uint8(is_weapon_unlocked(6));
    sj_object_insert(new_pd, "weapon6", value);

    value = sj_new_uint8(is_weapon_unlocked(7));
    sj_object_insert(new_pd, "weapon7", value);

    value = sj_new_uint8(is_weapon_unlocked(8));
    sj_object_insert(new_pd, "weapon8", value);

    value = sj_new_uint8(is_weapon_unlocked(9));
    sj_object_insert(new_pd, "weapon9", value);

    value = sj_new_uint8(is_weapon_unlocked(10));
    sj_object_insert(new_pd, "weapon10", value);

    value = sj_new_uint8(is_passive_unlocked(1));
    sj_object_insert(new_pd, "passive1", value);

    value = sj_new_uint8(is_passive_unlocked(2));
    sj_object_insert(new_pd, "passive2", value);

    value = sj_new_uint8(is_passive_unlocked(3));
    sj_object_insert(new_pd, "passive3", value);

    value = sj_new_uint8(is_passive_unlocked(4));
    sj_object_insert(new_pd, "passive4", value);

    value = sj_new_uint8(is_passive_unlocked(5));
    sj_object_insert(new_pd, "passive5", value);

    value = sj_new_uint8(is_active_unlocked(1));
    sj_object_insert(new_pd, "active1", value);

    value = sj_new_uint8(is_active_unlocked(2));
    sj_object_insert(new_pd, "active2", value);

    value = sj_new_uint8(is_active_unlocked(3));
    sj_object_insert(new_pd, "active3", value);

    value = sj_new_uint8(is_active_unlocked(4));
    sj_object_insert(new_pd, "active4", value);

    value = sj_new_uint8(is_active_unlocked(5));
    sj_object_insert(new_pd, "active5", value);

    sj_save(json, "./def/persistent_data.def");

    sj_free(json);

    slog("saved level");

}
