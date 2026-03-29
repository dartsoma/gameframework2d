#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "simple_json.h"
#include "weapon.h"
#include "ent.h"

typedef struct {

    Uint8 projectileId;
    Uint8 damage;
    Uint8 speed;
    float splash;
    float lifetime;
    float hitCooldown;
    Ent *owner;

} Projectile;

void instance_projectile(Ent* owner, Gun *gun, float rotation);

void unload_projectiles();

void load_projectiles();

void projectile_hit(Ent *self, Ent *other);

void projectile_think(Ent *self);

void projectile_update(Ent *self, float deltatime);


#endif
