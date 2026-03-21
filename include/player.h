#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ent.h"


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

void player_think(Ent *self);

void player_update(Ent *self, float deltatime);

void move(Ent *self, int dir);


#endif
