#include "simple_logger.h"
#include "camera.h"
#include "gfc_input.h"
#include "gfc_shape.h"
#include "level_editor.h"

InstancedProp props[100];
InstancedProp *currprop;
EditorTransform actions;
PropDef *defs;
Uint8 defcount;
PropDef *currdef;

void load_editor(){

int i;
SJson *json; //free
SJson *config, *propjson;
json = sj_load("./def/props.def");

memset(props, 0, sizeof(InstancedProp)*100);
memset(&actions, 0, sizeof(EditorTransform));

if (!json){

    slog("bad propdef");
}

config = sj_object_get_value(json, "props");
defcount = sj_array_get_count(config);

defs = (PropDef *) malloc(sizeof(PropDef) * defcount);

for(i = 0; i < defcount; i++){

    propjson = sj_array_get_nth(config,i);
    if (!propjson) continue;

    // Create a propdef object
    slog("creating propdef");
    // prop is object

    strcpy(defs[i].name, sj_object_get_string(propjson, "name"));
    strcpy(defs[i].imagepath, sj_object_get_string(propjson, "imagepath"));
    sj_object_get_int(propjson, "framewidth", &defs[i].framewidth );
    sj_object_get_int(propjson, "frameheight", &defs[i].frameheight);
    sj_object_get_int(propjson, "framesperline", &defs[i].framesperline);

}

currdef = defs;
currprop = NULL;

sj_free(json);
}


void spawn_prop(GFC_Vector2D vector){
    int i;

    for (i = 0; i < 100; i++){
        if (props[i].framesperline > 0) continue;

        strcpy(props[i].name, currdef->name);
        props[i].position = vector;
        props[i].scale = gfc_vector2d(1,1);
        props[i].color = GFC_COLOR_WHITE;
        props[i].framewidth = currdef->framewidth;
        props[i].frameheight = currdef->frameheight;
        props[i].framesperline = currdef->framesperline;

        props[i].sprite = gf2d_sprite_load_all(
            currdef->imagepath,
            props[i].framewidth,
            props[i].frameheight,
            props[i].framesperline,
            0
        );

        slog("item spawned: %s #%d", props[i].name, i);
        break;
    }


}

void delete_prop(){

    if(currprop) return;

    memset(currprop, 0, sizeof(InstancedProp));

}

void prop_cycle_l(){

    if(currdef == &defs[0]){
    currdef = &defs[defcount-1];
    } else {
        currdef--;
    }

}

void prop_cycle_r(){

    if(currdef == &defs[defcount-1]){
        currdef = &defs[0];
    } else {
    currdef++;
}

}


void save_level(LevelBase *base){

    int i;

    SJson *json;
    json = sj_load("./def/levels.def");
    slog("saving level");
    if (!json) {
        slog("bad level def");
    }


    SJson *config, *new_level, *value, *propjson;

    slog("loading level");

    config = sj_object_get_value(json, "levels");

    if (!config) return;

    new_level = sj_object_new();

    value = sj_new_str(base->name);

    sj_object_insert(new_level, "name", value);

    value = sj_new_str(base->imagepath);

    sj_object_insert(new_level, "imagepath", value);

    value = sj_new_int(base->width);

    sj_object_insert(new_level, "width", value);

    value = sj_new_int(base->height);

    sj_object_insert(new_level, "height", value);

    propjson = sj_array_new();


    for (i = 0; i < 100; i++){
        if (props[i].framesperline == 0) continue;
        SJson *p = sj_object_new();

        value = sj_new_str(props[i].name);
        sj_object_insert(p, "name", value);

        value = sj_new_float(props[i].position.x);
        sj_object_insert(p, "xpos", value);

        value = sj_new_float(props[i].scale.x);
        sj_object_insert(p, "xscale", value);

        value = sj_new_float(props[i].scale.y);
        sj_object_insert(p, "yscale", value);

        value = sj_new_float(props[i].color.r);
        sj_object_insert(p, "rcolor", value);

        value = sj_new_float(props[i].color.g);
        sj_object_insert(p, "gcolor", value);

        value = sj_new_float(props[i].color.b);
        sj_object_insert(p, "bcolor", value);

        value = sj_new_float(props[i].color.a);
        sj_object_insert(p, "acolor", value);

        sj_array_append(propjson, p);

    }

    sj_object_insert(new_level, "props", propjson);

    sj_array_append(config, new_level);

    sj_save(json, "./def/levels.def");

    sj_free(json);

    free(base);

    slog("saved level");
}

void editor_draw_all(){
    int i;

    for (i = 0; i < 100; i++){
        if (props[i].framesperline == 0) continue;

        GFC_Vector2D position, offset;
        offset = camera_get_pos();
        gfc_vector2d_sub(position, props[i].position, offset);

        if(props[i].sprite){
            gf2d_sprite_render(
                props[i].sprite,
                position,
                &props[i].scale,
                NULL,
                NULL,
                NULL,
                &props[i].color,
                NULL,
                0);
        }
        slog("pos %f, %f", position.x, position.y);
    }


}


char* get_def_name() {
    return currdef->name;
}

void editor_think(){

    Uint8 clicking = click_status();
    GFC_Vector2D mouse_loc = absolute_mouse_pos();
    GFC_Circle click_loc = gfc_circle(mouse_loc.x, mouse_loc.y, 10.0f);

    if(clicking == 1 && currprop != NULL) {

        if ( actions.modes & 3 ){
            actions.old_pos = currprop->position;
            actions.old_scale = currprop->scale;
            clicking = 0;
        }

        currprop = NULL;
        actions.modes = 0;

    } else if (gfc_input_key_down("DELETE") && currprop) {

        currprop->framesperline = 0;

        currprop = NULL;
        actions.modes = 0;

    } else if (gfc_input_key_down("g") && currprop) {
        currprop->scale = actions.old_scale;
        actions.modes &= ~(2U);
        actions.modes |= 1;
    } else if (gfc_input_key_down("s") && currprop) {
        currprop->position = actions.old_pos;
        actions.modes &= ~(1U);
        actions.modes |= 2;
    } else if (gfc_input_key_down("y") && (actions.modes & 3 )) {


        actions.modes &= ~(8U);
        actions.modes |= 4;

    } else if (gfc_input_key_down("x") && (actions.modes & 3 )) {

        actions.modes &= ~(4U);
        actions.modes |= 8;
    }




    for (int i = 0; i < 100; i++){
        if (props[i].framesperline == 0) continue;
        GFC_Vector2D pos;
        gfc_vector2d_sub(pos, props[i].position, camera_get_pos());
        GFC_Rect rect = gfc_rect(pos.x, pos.y, props[i].scale.x * props[i].framewidth , props[i].scale.y * props[i].frameheight);

        if (gfc_circle_rect_overlap(click_loc, rect) & clicking){
            currprop = &props[i];
            actions.old_pos = props[i].position;
            actions.old_scale = props[i].scale;
            actions.modes = 0;
            slog("Selected Prop # %d", i);
        }
        // if moving
        // if scaling
    }


    // collision check

    if (!currprop) return;



    if(actions.modes & 1){

        currprop->position = get_mouse_pos();

        if (actions.modes & 4){
            currprop->position.x = actions.old_pos.x;
        } else if (actions.modes & 8) {
            currprop->position.y = actions.old_pos.y;
        }

        currprop->position.x = (int) currprop->position.x;
        currprop->position.y = (int) currprop->position.y;

    } else if (actions.modes & 2){


            currprop->scale.x = (mouse_loc.x - currprop->position.x) / 20;
            currprop->scale.y = (mouse_loc.x - currprop->position.x) / 20;

        if (actions.modes & 4){
            currprop->scale.y = actions.old_scale.y;
        } else if (actions.modes & 8) {
            currprop->scale.x = actions.old_scale.x;
        }
        currprop->scale.x = (int) currprop->scale.x;
        currprop->scale.y = (int) currprop->scale.y;
    }
}

void free_defs(){

    free(defs);

}


void delete_all() {

    int i;

    for (i = 0; i < 100; i++) {
        if (props[i].framesperline == 0) continue;
        memset(&props[i], 0, sizeof(InstancedProp));
    }


}
