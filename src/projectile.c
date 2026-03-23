#include "simple_logger.c"
#include "projectile.h"

Projectile projectiles = {0}; // 10 kinds of projectiles

Projectile projectile_create(){



} // define the projectile

Ent *instance_projectile(Ent* owner, Projectile *projectile); // fire instances of a defined projectile

void projectile_think(Ent *self);

void projectile_update(Ent *self, float deltatime);
