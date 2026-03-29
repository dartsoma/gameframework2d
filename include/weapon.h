#ifndef __WEAPON_H__
#define __WEAPON_H__


#include "ent.h"


#define NONE 0
#define SHOTGUN 1
#define SNIPER 2
#define RIFLE 3
#define PISTOL 4
#define BLAST 5
#define MELEE 6

typedef struct {
  Uint8 gunId;
  char* name;
  Sprite *sprite;
  float firerate;
  float spread;
  float accuracy;
  Uint8 reserve; // in inventory
  Uint8 ammo; // in mag
  Uint8 apr; // max in round
  Uint8 maxammo; // max a weapon can hold
  Uint8 projectileId; // index of bullet type
  float fdebounce; // fire debounce
  float rdebounce; // reload debounce
  Uint8 type;
} Gun;



typedef struct {
  Uint8 meleeId;
  char* name;
  Sprite *sprite;
  float speed;
  Uint8 damage;
  float resist;
  Uint8 type;
  float debounce;
} Melee;

// for guns

// check if reloading or if firing cooldown or any other action that would prevent firing

// check ammo

// send out projectile

// put firing on cooldown
void unload_weapons();

void load_weapons();

void fire(Gun *g, Ent *owner);

void attack(Melee *m);

Gun copy_gun(Uint8 id);

Melee copy_melee(Uint8 id);


#endif
