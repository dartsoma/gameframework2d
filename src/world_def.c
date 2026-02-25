#include "simple_logger.h"
#include "world_def.h"

PropDef *create_propdef(SJson *prop){

    // prop is object

    PropDef *def = malloc(sizeof(PropDef));

    if (!def) return NULL;
    // arrays

    strcpy(def->name, sj_object_get_string(prop, "name"));
    strcpy(def->imagepath, sj_object_get_string(prop, "imagepath"));
    sj_object_get_int(prop, "framewidth", &def->framewidth );
    sj_object_get_int(prop, "frameheight", &def->frameheight);
    sj_object_get_int(prop, "framesperline", &def->framesperline);
    sj_object_get_uint8(prop, "health", &def->stats[0]);
    sj_object_get_uint8(prop, "hazardflags", &def->stats[1]);
    sj_object_get_uint8(prop, "hazardradius", &def->stats[2]);
    sj_object_get_uint8(prop, "destructable", &def->stats[3]);

    return def;

}

void free_propdef(PropDef *def){

    if(!def)return;
    free(def);

}

void instance_props(SJson *leveljson, GFC_HashMap *prop_map){

    if(!leveljson)return;
    if(!prop_map)return;
    int i, c;
    SJson *config, *propjson;
    PropDef *propdef;
    Ent *prop;

    config = sj_object_get_value(leveljson, "props");
    c = sj_array_get_count(config);

    for(i = 0; i < c; i++){

    propjson = sj_array_get_nth(config, i);
    if(!propjson) continue;

    propdef = gfc_hashmap_get(prop_map, sj_object_get_string(propjson, "name"));

    prop = prop_new();

    if(!prop) return;

    prop->sprite = gf2d_sprite_load_all(
        propdef->imagepath,
        propdef->framewidth,
        propdef->frameheight,
        propdef->framesperline,
        0
    );

    prop->stats[0] = propdef->stats[0];
    prop->stats[1] = propdef->stats[1];
    prop->stats[2] = propdef->stats[2];
    prop->stats[3] = propdef->stats[3];
    sj_object_get_float(propjson, "xpos", &prop->transform.position.x);
    sj_object_get_float(propjson, "ypos", &prop->transform.position.y );
    sj_object_get_float(propjson, "xscale", &prop->transform.scale.x );
    sj_object_get_float(propjson, "yscale", &prop->transform.scale.y);
    sj_object_get_float(propjson, "rcolor", &prop->color.r);
    sj_object_get_float(propjson, "gcolor", &prop->color.g);
    sj_object_get_float(propjson, "bcolor", &prop->color.b);
    sj_object_get_float(propjson, "acolor", &prop->color.a);
    slog("Prop Loaded");
    }

}

