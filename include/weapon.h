#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "projectile.h"



typdef enum {

  SHOTGUN = 1,
  SNIPER = 2,
  RIFLE = 3,
  PISTOL = 4,
  BLAST = 5,
  MELEE = 6

} WeaponType;

typedef struct {
  int gunId;
  char* name;
  Sprite sprite;
  float firerate;
  float spread;
  float accuracy;
  int reserve; // in inventory
  int ammo; // in mag
  Projectile projectile; // store a copy and then every fire will use an instance of this as an entity
  WeaponType type;
} Gun;

typedef struct {
  int meleeId;
  char* name;
  Sprite sprite;
  float speed;
  int damage;
  float resist;
  WeaponType type;
} Melee;

void load_weapons();

// for guns

// check if reloading or if firing cooldown or any other action that would prevent firing

// check ammo

// send out projectile

// put firing on cooldown

void fire(Gun *g);

void hit(Melee *m);



#endif
