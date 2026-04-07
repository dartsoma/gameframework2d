#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ent.h"
#include "projectile.h"
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
#define RESPAWNTIMER 4

Ent *player_new();

typedef struct {
    Uint8 passiveId;
    Uint8 activeId;
    float activeCharge;
    float passiveCd;
    float hillTimer;
    Sprite *head;
    eTransform hTransform;
    eCollide hCollide;
    Gun *guns; // hold x amount of guns
    Gun *hand;
    Melee melee;
    int points;
} PlayerData;


void player_think(Ent *self);

void player_free(Ent *self);

// draws the addons, head tracking, guns etc
void player_draw(Ent *self);

void player_update(Ent *self, float deltatime);

void move(Ent *self, int dir);

void player_hit(Ent *attacker, Ent *self, Uint8 tag);

void player_respawn(Ent* self);

void activate_ability(Ent *self, Uint8 id);

void apply_passive(Ent *self, Uint8 id);

int get_points(Ent *self);

char* display_points();
#endif
