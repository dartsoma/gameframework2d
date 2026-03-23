#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ent.h"
#include "weapon.h"

#define LEFT 0
#define RIGHT 1
#define JUMP 2


/**
 * @note Used in stats
 *
 **/
#define HEALTH 0
#define ARMOR 1
#define SPEED 2
#define JUMPPOWER 3

Ent *player_new();

typedef struct {
    Uint8 passiveId;
    Uint8 activeId;
    float activeCharge;
    float passiveCd;
    Sprite *head;
    Gun **guns; // hold x amount of guns
    Melee melee;
} PlayerData;


void player_think(Ent *self);

void player_free(Ent *self);

// draws the addons, head tracking, guns etc
void player_draw(Ent *self, Sprite *hitbox);

void player_update(Ent *self, float deltatime);

void move(Ent *self, int dir);


#endif
