#include "simple_logger.h"

#include "camera.h"
#include "projectile.h"

Projectile *projectiles;

#define AIM_RAD 100.0f
#define BULLET_SPEED 5000.0f

void unload_projectiles(){

    if(!projectiles)return;

    free(projectiles);

}

void load_projectiles(){

    int i, c;
    SJson *json;
    SJson *config, *loadjson;
    json = sj_load("./def/projectile.def");

    if (!json){
        slog("bad projectile.def");
        return;
    }

    config = sj_object_get_value(json, "projectile");
    c = sj_array_get_count(config);

    Projectile *p = (Projectile *) malloc(sizeof(Projectile)*c);


    for(i = 0; i < c; i++){

        loadjson = sj_array_get_nth(config,i);
        char* name;

        sj_object_get_uint8(loadjson, "id", &p[i].projectileId);
        name = (char*) sj_object_get_string(loadjson, "name");
        if(strcmp(name, "rocket")==0){
            sj_object_get_float(loadjson, "splash", &p[i].splash);
        } else {
            p[i].splash = 0;
        }
        sj_object_get_uint8(loadjson, "damage", &p[i].damage);
         sj_object_get_float(loadjson, "lifetime", &p[i].lifetime);
    }


    sj_free(json);

    slog("finished proj defs");
    projectiles = p;
}

void instance_projectile(Ent* owner, Gun *gun, float rotation) {

    Ent *self = ent_new_rev();

    self->sprite = gf2d_sprite_load_all(
        "images/bullet.png",
        25,
        16,
        1,
        0
    );

    self->transform.scale = gfc_vector2d(1,1);
    self->transform.center = gfc_vector2d(16,7.5);
    self->color = GFC_COLOR_ORANGE;

    Projectile *p = (Projectile*) malloc(sizeof(Projectile));

    *p = projectiles[gun->type];

    self->misc = (Projectile *) p;

    self->_tags = TAG_PROJECTILE;
    insert_collision_layer(self);
    GFC_Vector2D offset = gfc_vector2d(owner->transform.position.x + 25, owner->transform.position.y+25);

    p->owner = owner;
    p->hitCooldown = 0;
    // radians
    float rotationOffset = ((M_PI * gfc_random()) - (M_PI / 2))* 0.25 *(1.0f - gun->accuracy);


    self->transform.position.x = offset.x + (AIM_RAD*cos(rotation));
    self->transform.position.y = offset.y + (AIM_RAD*sin(rotation));

    rotation += rotationOffset;

    // unit vector
    GFC_Vector2D dir = gfc_vector2d(cos(rotation), sin(rotation));;

    gfc_vector2d_scale(self->transform.velocity, dir, BULLET_SPEED);

    self->transform.rotation = rotation * (180.0f / M_PI); // back to deg


    // this allows guns with lower fire rate to wallbang and shotguns to stop short

    self->update = projectile_update;
    self->think = projectile_think;


} // fire instances of a defined projectile

void instance_melee(Ent* owner, Melee *m, float rotation) {

    Ent *self = ent_new_rev();

    self->sprite = gf2d_sprite_load_all(
        "images/hit.png",
        25,
        50,
        8,
        0
    );

    self->transform.scale = gfc_vector2d(1,1);
    self->transform.center = gfc_vector2d(16,7.5);
    self->color = GFC_COLOR_ORANGE;

    Projectile *p = (Projectile*) malloc(sizeof(Projectile));


    *p = projectiles[0];
    self->transform.scale = gfc_vector2d(2,6);
    self->collide.c_dim.x *= 2;
    self->collide.c_dim.y *= 6;
    self->misc = (Projectile *) p;
    self->_tags = TAG_PROJECTILE;
    insert_collision_layer(self);
    GFC_Vector2D offset = gfc_vector2d(owner->transform.position.x + 50, owner->transform.position.y+25);
    p->damage = m->damage;
    p->lifetime = 1;
    p->owner = owner;
    p->hitCooldown = 0;
    // radians


    self->transform.position.x = offset.x + (AIM_RAD*cos(rotation));
    self->transform.position.y = offset.y + (AIM_RAD*sin(rotation));

    // unit vector
    self->transform.velocity = gfc_vector2d(0,0);
    self->transform.rotation = rotation * (180.0f / M_PI); // back to deg


    // this allows guns with lower fire rate to wallbang and shotguns to stop short

    self->update = projectile_update;
    self->think = projectile_think;


} // fire instances of a defined projectile


void projectile_hit(Ent* self, Ent* other){

    Projectile *proj = (Projectile *) self->misc;
    if(other->_tags == TAG_PLAYER || other->_tags == TAG_NPC){
    proj->hitCooldown = 0.2;
    }
    if (proj->projectileId == 5){
    // rocket explode
    }

    proj->lifetime -= 1;

}


void projectile_think(Ent *self) {

    Projectile *proj = (Projectile *) self->misc;

    if (proj->lifetime <= 0){
        free(proj);
        ent_free(self);
    }

}


void projectile_update(Ent *self, float deltatime) {

    Projectile *proj = (Projectile *) self->misc;

    self->transform.position.x += self->transform.velocity.x*deltatime;
    self->transform.position.y += self->transform.velocity.y*deltatime;

    if(proj->projectileId == 0){
    self->frame += deltatime*4;
    }

    if (proj->hitCooldown > 0){
        proj->hitCooldown -= 0.016;
    }
    proj->lifetime -= deltatime*2;

}
