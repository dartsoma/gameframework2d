#ifndef __PERSISTENT_H__
#define __PERSISTENT_H__

typedef struct {

    int unlocks; // first 10 weapons, next 5
    int money;

} PersistantData;

Uint8 is_weapon_unlocked(Uint8 id);

Uint8 is_passive_unlocked(Uint8 id);

Uint8 is_active_unlocked(Uint8 id);

void load_persistent_data();

int get_money();

void add_money(int m);

void remove_money(int m);

void unlock_active(Uint8 id);

void unlock_passive(Uint8 id);

void unlock_weapon(Uint8 id);

void save_data();



#endif
