#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "ent.h"

typedef struct {

    int damage;
    GFC_VECTOR2D vel;
    self* owner;

} PROJECTILE;

Projectile projectile_create(); // define the projectile

Ent *instance_projectile(Ent* owner, Projectile *projectile); // fire instances of a defined projectile

void projectile_think(Ent *self);

void projectile_update(Ent *self, float deltatime);


#endif
