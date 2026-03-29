    #include "simple_logger.h"
    #include "world_def.h"



    PlayerData *load_player_data(Ent *self){
        SJson *json;
        SJson *config, *loadjson;
        json = sj_load("./def/player.def");

        if (!json){
            slog("bad player.def");
            return NULL;
        }

        config = sj_object_get_value(json, "player");

        PlayerData *pd = (PlayerData *) self->misc;

            Uint8 gun1,gun2,gun3,melee,team;

            loadjson = sj_array_get_nth(config,0);

            sj_object_get_uint8(loadjson, "passive", &pd->passiveId);
            sj_object_get_uint8(loadjson, "active", &pd->activeId);
            sj_object_get_uint8(loadjson, "gun1", &gun1);
            sj_object_get_uint8(loadjson, "gun2", &gun2);
            sj_object_get_uint8(loadjson, "gun3", &gun3);
            sj_object_get_uint8(loadjson, "melee", &melee);
            sj_object_get_uint8(loadjson, "team", &team);

            pd->guns[0] = copy_gun(gun1);
            pd->guns[1] = copy_gun(gun2);
            pd->guns[2] = copy_gun(gun3);
            pd->melee = copy_melee(melee);
            if (team == 0){
                self->collide.c_mask = CM_TEAM1;
            }
            if (team == 1) {
                self->collide.c_mask = CM_TEAM2;
            }
            if (team > 1) {
                self->collide.c_mask = CM_FFA;
            }



        sj_free(json);

        slog("finished player defs");
        return pd;


    }



    PropDef *create_propdef(SJson *prop){
        slog("creating propdef");
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
        sj_object_get_uint8(prop, "objflag", &def->stats[1]);


        return def;

    }

    void free_propdef(PropDef *def){

        if(!def)return;
        free(def);

    }

    void instance_props(SJson *leveljson, GFC_HashMap *prop_map){

        slog("Instancing_props");
        if(!leveljson)return;
        if(!prop_map)return;
        int i, c;
        SJson *config, *propjson;
        PropDef *propdef;
        Ent *prop;

        config = sj_object_get_value(leveljson, "props");
        c = sj_array_get_count(config);

        slog("Loading props... count = %d", c);

        for(i = 0; i < c; i++){

        slog("Prop %d", i);
        propjson = sj_array_get_nth(config, i);
        if(!propjson) continue;

        propdef = gfc_hashmap_get(prop_map, sj_object_get_string(propjson, "name"));

        prop = prop_new();


        if(!prop) return;

        prop->misc = (char*) propdef->name;

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
        prop->collide.c_dim.x = ((float)propdef->framewidth/10);
        prop->collide.c_dim.y = ((float)propdef->frameheight/10);



        // 1 is static, 2 is trigger/item, 4 is a blocker, 8 is passthrough,
        // neither 4 or 8 means the object can be moved through

        prop->_tags = TAG_STATIC;

        if ((prop->stats[1] & 1) == 1){
            prop->_tags = TAG_DYNAMIC;
        } else {
            prop->_tags = TAG_STATIC;
        }

        if ((prop->stats[1] & 2) == 2){
        prop->collide.c_mask = CM_TRIGGER;
        }
        if ((prop->stats[1] & 4) == 4){
        prop->collide.c_mask = CM_BLOCKER;
        }
        if ((prop->stats[1] & 8) == 8){
        prop->collide.c_mask = CM_PASSTHROUGH;
        }

        if(strcmp(propdef->name, "t1flag") == 0){

        set_flags(1, prop);

        } else if (strcmp(propdef->name, "t2flag") == 0)  {

        set_flags(2, prop);
        }

        insert_collision_layer(prop);

        // collision hitbox
        gfc_vector2d_scale_by(prop->collide.c_dim, prop->transform.scale, prop->collide.c_dim);


        slog("Prop Loaded");
        }

    }

