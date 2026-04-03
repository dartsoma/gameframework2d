#include "simple_logger.h"
#include "player.h"
#include "camera.h"
#include "projectile.h"
#include "weapon.h"


Gun *gundef;
Melee *meleedef;


// for guns

// check if reloading or if firing cooldown or any other action that would prevent firing

// check ammo

// intstance projectile with velocity, unless sniper than raycast

// put firing on cooldown

void unload_weapons() {

    free(gundef);
    free(meleedef);

}

void load_weapons(){

    int i, c;
    SJson *json;
    SJson *config, *loadjson;
    json = sj_load("./def/gun.def");

    if (!json){
        slog("bad gun.def");
        return;
    }

    config = sj_object_get_value(json, "guns");
    c = sj_array_get_count(config);

    Gun *g = (Gun*) malloc(sizeof(Gun)*c);


    for(i = 0; i < c; i++){

        char* spriteloc;

        loadjson = sj_array_get_nth(config,i);

        sj_object_get_uint8(loadjson, "id", &g[i].gunId);
        g[i].name = (char*) sj_object_get_string(loadjson, "name");
        spriteloc = (char*) sj_object_get_string(loadjson, "sprite");

        if(i != 0){
                g[i].sprite = gf2d_sprite_load_all(
                    spriteloc,
                    50,
                    50,
                    1,
                    0
                );
        }

        sj_object_get_float(loadjson, "firerate", &g[i].firerate);
        sj_object_get_float(loadjson, "spread", &g[i].spread);
        sj_object_get_float(loadjson, "accuracy", &g[i].accuracy);
        sj_object_get_uint8(loadjson, "reserve", &g[i].reserve);
        sj_object_get_uint8(loadjson, "ammo", &g[i].ammo);
        g[i].maxammo = g[i].reserve;
        g[i].apr = g[i].ammo;
        sj_object_get_uint8(loadjson, "projectileId", &g[i].projectileId);
        sj_object_get_uint8(loadjson, "type", &g[i].type);
    }

    sj_free(json);

    slog("finished gun defs");

    gundef = g;

    config = sj_object_get_value(json, "melee");
    c = sj_array_get_count(config);

    Melee *m = (Melee*) malloc(sizeof(Melee)*c);


    for(i = 0; i < c; i++){

        char* spriteloc;

        loadjson = sj_array_get_nth(config,i);

        sj_object_get_uint8(loadjson, "id", &m[i].meleeId);
        m[i].name = (char*) sj_object_get_string(loadjson, "name");
        spriteloc = (char*) sj_object_get_string(loadjson, "sprite");

        if(i != 0){
                m[i].sprite = gf2d_sprite_load_all(
                    spriteloc,
                    50,
                    50,
                    1,
                    0
                );
        }

        sj_object_get_float(loadjson, "speed", &m[i].speed);
        sj_object_get_uint8(loadjson, "damage", &m[i].damage);
        sj_object_get_float(loadjson, "resist", &m[i].resist);

    }

    sj_free(json);

    slog("finished melee defs");

    meleedef = m;

}

void fire(Gun *g, Ent *owner){

    if(g->fdebounce > 0 || g->rdebounce > 0) return;

    if (g->ammo == 0 && g->reserve >= 0) {

    if (((g->reserve - g->apr ) < 0 ) && g->reserve != 0){
        slog("reloading");
        g->ammo = g->reserve;
        g->reserve = 0;
    } else {
        slog("reloading");
        g->reserve -=g->apr;
        g->ammo += g->apr;
    }
    if(g->reserve == 0 ){
        slog("empty");
        return;
    }
    owner->status |= 8;
    // a genius idea, most guns have a relatively stable apr other than snipers and rpgs
    g->rdebounce = g->maxammo/g->apr;
    return;
    }

    GFC_Vector2D offset = gfc_vector2d(owner->transform.position.x + 25, owner->transform.position.y+25);

    // depending on rotation

    GFC_Vector2D mouse = get_mouse_pos();

    float dx = mouse.x - offset.x;
    float dy = mouse.y - offset.y;

    float rotation = atan2f(dy,dx);

    for(int i = 0; i<g->spread+1; i++){

        instance_projectile(owner, g, rotation);

    }
    g->ammo--;
    g->fdebounce = g->firerate;
    owner->status |= 8;
}


Gun copy_gun(Uint8 id){

    return gundef[id];
}

Melee copy_melee(Uint8 id) {

    return meleedef[id];

}

void attack(Melee *m, Ent *owner){

    // cast ray
    if(m->debounce > 0) return;


    GFC_Vector2D offset = gfc_vector2d(owner->transform.position.x + 25, owner->transform.position.y+25);

    GFC_Vector2D mouse = get_mouse_pos();

    float dx = mouse.x - offset.x;
    float dy = mouse.y - offset.y;

    float rotation = atan2f(dy,dx);

    m->debounce = m->speed;
    owner->status |= 8;

    instance_melee(owner, m, rotation);

}
