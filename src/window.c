#include "simple_logger.h"
#include "gfc_shape.h"
#include "gfc_vector.h"
#include "camera.h"
#include "gfc_input.h"
#include "level_editor.h"
#include "font.h"
#include "persistent_data.h"
#include "window.h"

// ** //

/**
* @note each window will have its own blueprint in json
**/

#define MAX_Z 100
#define MIN_Z 0
#define TYPE_CD 0.100

int win_count;
W_Element **ui; // stores all window references
Uint8 istyping;
W_TextArea *typingarea;
float input_debounce = 0;
GFC_HashMap *hash;
GFC_Color base;

// Reads json and populates ui elements
void define_windows(){

    SJson *json, *win;
    int i, c, j, k;

    json = sj_load("./def/ui.def");


    if (!json){
        slog("bad ui.def");
        return;
    }
    base = GFC_COLOR_WHITE;
    json = sj_object_get_value(json, "ui");
    c = sj_array_get_count(json);
    win_count = c;
    hash = gfc_hashmap_new();
    ui = (W_Element**) malloc(sizeof(W_Element*) * c);
    typingarea = NULL;
    for (i = 0; i < c; i++){


        SJson *value;
        char imagepath[255] = "";
        imagepath[0] = '\0';
        W_Element *element = (W_Element *) malloc(sizeof(W_Element));
        W_Window *root = (W_Window *) malloc(sizeof(W_Window));
        if(!root) continue;
        element->data = (W_Window *) root;
        root->name[0] = '\0';
        element->active = 0;
        element->parent = NULL;
        win = sj_array_get_nth(json,i);
        strcpy(root->name, sj_object_get_string(win, "name"));


        // index
        gfc_hashmap_insert(hash, root->name, element);

        value = sj_object_get_value(win, "size");
        value = sj_array_get_nth(value,0);
        sj_get_float_value(value, &root->size.x);
        value = sj_object_get_value(win, "size");
        value = sj_array_get_nth(value,1);
        sj_get_float_value(value, &root->size.y);

        value = sj_object_get_value(win, "position");
        value = sj_array_get_nth(value,0);
        sj_get_float_value(value, &root->position.x);
        value = sj_object_get_value(win, "position");
        value = sj_array_get_nth(value,1);
        sj_get_float_value(value, &root->position.y);

        if(sj_object_get_string(win, "sprite") != NULL){
           strcpy(imagepath, sj_object_get_string(win, "sprite"));
           slog("background loaded for %s", root->name);
        }

        if(strcmp(imagepath, "") != 0){
        root->sprite = gf2d_sprite_load_image(imagepath);
        }

        win = sj_object_get_value(win, "elements");
        k = sj_array_get_count(win);
        root->objs = (W_Element**) malloc(sizeof(W_Element*)*k);
        memset(root->objs, 0, sizeof(W_Element*) * k);

        // must be uint8
        root->obj_count = k;

        for (j = 0; j < k; j++) {
            SJson *el;
            int type;
            el = sj_array_get_nth(win,j);
            sj_object_get_int(el, "type", &type);

            switch (type){
                case EL_BUTTON:
                root->objs[j] = create_button(el, element);
                break;
                case EL_LABEL:
                root->objs[j] = create_label(el, element);
                break;
                case EL_TEXTAREA:
                root->objs[j] = create_textarea(el, element);
                break;
                case EL_CONTAINER:
                root->objs[j] = create_container(el, element);
                break;
                case EL_IMAGE:
                root->objs[j] = create_image(el, element);
                break;
                default:
                break;
            }
            slog("Entity %d complete", j);
        }
        ui[i] = element;
        slog("Window %d complete", i);
    }
    sj_free(json);

}

W_Element *create_container(SJson *j, W_Element *parent){

    //  create
    int i, k;
    SJson *value;

    W_Element *element = (W_Element*) malloc(sizeof(W_Element));
    W_Window *cont = (W_Window*) malloc(sizeof(W_Window));
    if(!cont) {
        free(element);
        return NULL;
    }
    cont->name[24] = '\0';
    element->data = (W_Window*) cont;
    element->parent = parent;

    strcpy(cont->name, sj_object_get_string(j, "name"));


    gfc_hashmap_insert(hash, cont->name, element);

    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &cont->size.x);
    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &cont->size.y);

    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &cont->position.x);
    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &cont->position.y);

    sj_object_get_uint8(j, "active", &element->active);
    sj_object_get_uint8(j, "type", &element->type);

    j = sj_object_get_value(j, "elements");
    k = sj_array_get_count(j);
    cont->objs = (W_Element**) malloc(sizeof(W_Element*)*k);
    memset(cont->objs, 0, sizeof(W_Element*) * k);

    cont->obj_count = k;
    for (i = 0; i < k; i++) {
        SJson *el;
        int type;
        el = sj_array_get_nth(j,i);

        sj_object_get_int(el, "type", &type);

        switch (type){
            case EL_BUTTON:
                cont->objs[i] = create_button(el, element);
                break;
            case EL_LABEL:
                cont->objs[i] = create_label(el, element);
                break;
            case EL_TEXTAREA:
                cont->objs[i] = create_textarea(el, element);
                break;
            case EL_CONTAINER:
                cont->objs[i] = create_container(el, element);
                break;
            case EL_IMAGE:
                cont->objs[i] = create_image(el, element);
                break;
            default:
                break;
        }
        slog("Sub Entity %d from %s complete", i, cont->name);
    }

    return element;
}

void update_label(W_Label *l){

    if (l->id == 0){

    }
    switch (l->id){
        // editor
        case 1:

        if(get_def_name()){
        strcpy(l->text, get_def_name());
        }

        break;
        case 2:


        break;
    }

}

void update_text (float deltatime){

    int i;

    if (!istyping) return;
    if (!typingarea) return;

    input_debounce -= deltatime;
    if (input_debounce > 0) return;

    int slen = strlen(typingarea->text);

    if (gfc_input_key_down("RETURN") && slen > 0) {

        submit_text(typingarea->id);
        strcpy(typingarea->text, "");
        return;

    }

    if (gfc_input_key_down("BACKSPACE") && slen > 0) {
        typingarea->text[slen-1] = '\0';
        input_debounce = TYPE_CD;
        return;
    }

    if (slen >= 254) return;

    if (gfc_input_key_down(" ")) {
        char temp[2] = {' ', '\0'};
        strcat(typingarea->text, temp);
        input_debounce = TYPE_CD;
    } else {
    if (gfc_input_key_down("/")) {
        char temp[2] = {' ', '\0'};
        strcat(typingarea->text, temp);
        input_debounce = TYPE_CD;
    } else {

    // higher ascii values have higher precedence rather than what was pressed last on the last active frame

    for (i = 33; i < 123; i++) {

         char temp[2] = {i, '\0'};

        if(gfc_input_key_down(temp) && i != 33){
            strcat(typingarea->text, temp);
            input_debounce = TYPE_CD;
        break;

        strcat(typingarea->text, temp);
        input_debounce = TYPE_CD;
        break;

        }
    }

    }

}

}


// Reads json and populates ui elements


void update_container(W_Element *w, GFC_Circle *click_loc, Uint8 *clicking, Uint8 *gamestate){

    int i;

    W_Window *win = (W_Window *) w->data;


    for (i = 0; i < win->obj_count; i++) {

        if (!win->objs[i]->active)continue;

        switch(win->objs[i]->type){
            case 1:{
                W_Button *butt = (W_Button*) win->objs[i]->data;

                GFC_Vector2D position = gfc_vector2d(0,0);
                position.x = win->position.x + butt->pos.x;
                position.y = win->position.y + butt->pos.y;


                // yes ik i hardcoded it
                if (butt->eventId > 13 && butt->eventId < 34){


                if(butt->eventId > 13 && butt->eventId < 24 && is_weapon_unlocked(butt->eventId - 13)){
                    if(butt->sprite){
                        gf2d_sprite_render(
                            butt->sprite,
                            position,
                            &butt->size,
                            NULL,
                            NULL,
                            NULL,
                            &base,
                            NULL,
                            1);
                    }
                    break;

                } else if ((butt->eventId > 23 && butt->eventId < 29 && is_passive_unlocked(butt->eventId - 23))) {

                    if(butt->sprite){
                        gf2d_sprite_render(
                            butt->sprite,
                            position,
                            &butt->size,
                            NULL,
                            NULL,
                            NULL,
                            &base,
                            NULL,
                            1);
                    }

                } else if ((butt->eventId > 28 && butt->eventId < 34 && is_active_unlocked(butt->eventId - 28))) {


                    if(butt->sprite){
                        gf2d_sprite_render(
                            butt->sprite,
                            position,
                            &butt->size,
                            NULL,
                            NULL,
                            NULL,
                            &base,
                            NULL,
                            1);
                    }

                    }
                    break;
                }
                if(butt->sprite){
                    gf2d_sprite_render(
                        butt->sprite,
                        position,
                        &butt->size,
                        NULL,
                        NULL,
                        NULL,
                        &base,
                        NULL,
                        0);
                }

                GFC_Rect box = gfc_rect(position.x, position.y, butt->dim.x, butt->dim.y);

                if (*clicking) {
                    if (gfc_circle_rect_overlap(*click_loc, box)){
                        event_activate(butt->eventId, gamestate);
                        slog("event %d", butt->eventId);
                    }
                }

                break;
            }

            case 2:{
                W_Label *la = (W_Label*) win->objs[i]->data;

                // show text at font size at location in regards to size
                GFC_Vector2D position = gfc_vector2d(0,0);
                position.x = win->position.x + la->pos.x;
                position.y = win->position.y + la->pos.y;

                // background
                if(la->sprite){
                    gf2d_sprite_render(
                        la->sprite,
                        position,
                        &la->size,
                        NULL,
                        NULL,
                        NULL,
                        &base,
                        NULL,
                        0);
                }

                // text
                font_draw_test(la->text, la->f_size, base, position);

                break;
            }

            case 3:{
                W_TextArea *ta = (W_TextArea*) win->objs[i]->data;

                GFC_Vector2D position = gfc_vector2d(0,0);
                position.x = win->position.x + ta->pos.x;
                position.y = win->position.y + ta->pos.y;
                GFC_Rect box = gfc_rect(position.x, position.y, ta->dim.x, ta->dim.y);

                if(ta->sprite){
                    gf2d_sprite_render(
                        ta->sprite,
                        position,
                        &ta->size,
                        NULL,
                        NULL,
                        NULL,
                        &base,
                        NULL,
                        0);
                }

                if (*clicking) {
                    if (gfc_circle_rect_overlap(*click_loc, box)){
                        typingarea = ta;
                        istyping = 1;
                    }
                }
                if (strlen(ta->text) > 0){
                font_draw_test(ta->text, ta->f_size, base, position);
                } else {
                font_draw_test(ta->placeholder, ta->f_size, base, position);
                }
                break;
            }

            case 4: {
                update_container(win->objs[i], click_loc, clicking, gamestate);
                break;
            }
            case 5:{
                W_Image *img = (W_Image*) win->objs[i]->data;

                // show text at font size at location in regards to size
                GFC_Vector2D position = gfc_vector2d(0,0);
                position.x = win->position.x + img->pos.x;
                position.y = win->position.y + img->pos.y;

                // background
                if(img->sprite){
                    gf2d_sprite_render(
                        img->sprite,
                        position,
                        &img->size,
                        NULL,
                        NULL,
                        NULL,
                        &base,
                        NULL,
                        0);
                }
                break;
            }
        }
    }
}

void submit_text(Uint8 id){

    switch (id) {

        case 0:
        break;
    }

}

W_Element *get_window(const char* name){

    return gfc_hashmap_get(hash, name);

}


void event_activate(Uint8 id, Uint8 *gamestate){


switch (id) {

    case 0:
    // do nothing
    break;

    case 1:
    // exit game
    *gamestate = 0;
    break;

    case 2:
    // turn off main menu and boot up game

    toggle_windows("mainmenu");
    toggle_windows("mapselect");
    *gamestate = 2;
    break;

    case 3:
    toggle_windows("mainmenu");
    toggle_windows("shop");
    toggle_windows("shop_weapon");
    *gamestate = 3;
    break;

    case 4:
    toggle_windows("mainmenu");
    toggle_windows("editor");
    *gamestate = 6;
    break;

    case 5: {

    W_Element *el = gfc_hashmap_get(hash, "editor");
    W_Window *win = (W_Window *) el->data;

    GFC_Vector2D vec;

    for(int i = 0; i < win->obj_count; i++){



        if (win->objs[i]->type != 3) continue;

        W_TextArea *ta = (W_TextArea *) win->objs[i]->data;

        if (ta->id == 1) {
        vec.x = atof(ta->text);
        strcpy(ta->text, "");
            continue;
        }

        if (ta->id == 2) {
        vec.y = atof(ta->text);
        strcpy(ta->text, "");
            break;
        }

    }

    spawn_prop(vec);

        break;
    }

    // Save Button
    case 6:{
        W_Element *el = gfc_hashmap_get(hash, "ed_popup");
        el->active = 1;

    break;
    }

    // Delete all Button
    case 7:

        delete_all();

    break;
    // Confirm Button
    case 8: {
        W_Element *el = gfc_hashmap_get(hash, "ed_popup");
        W_Window *win = (W_Window *) el->data;

        el->active = 0;

        LevelBase *base;
        base = (LevelBase *) malloc(sizeof(LevelBase));

        for(int i = 0; i < win->obj_count; i++){



            if (win->objs[i]->type != 3) continue;

            W_TextArea *ta = (W_TextArea *) win->objs[i]->data;

            switch(ta->id){

                case 3:
                    strcpy(base->name, ta->text);
                    strcpy(ta->text, "");
                    break;
                case 4:
                    strcpy(base->imagepath, ta->text);
                    strcpy(ta->text, "");
                    break;
                case 5:
                    base->width = atoi(ta->text);
                    strcpy(ta->text, "");
                    break;
                case 6:
                    base->height = atoi(ta->text);
                    strcpy(ta->text, "");
                    break;
            }

            if (ta->id == 6){
                break;
            }

        }

        save_level(base);

    break;
    }
    // Cancel Button
    case 9:{

        W_Element *el = gfc_hashmap_get(hash, "ed_popup");
        W_Window *win = (W_Window *) el->data;

        el->active = 0;

        for(int i = 0; i < win->obj_count; i++){



            if (win->objs[i]->type != 3) continue;

            W_TextArea *ta = (W_TextArea *) win->objs[i]->data;

            switch(ta->id){

                case 3:
                    strcpy(ta->text, "");
                    break;
                case 4:
                    strcpy(ta->text, "");
                    break;
                case 5:
                    strcpy(ta->text, "");
                    break;
                case 6:
                    strcpy(ta->text, "");
                    break;
            }

            if (ta->id == 3){
                break;
            }

        }

    }

    // Cycle Left
    case 10:

        prop_cycle_l();

    // Cycle Right
    case 11:

        prop_cycle_r();

    break;

    case 12:

    toggle_windows("shop_weapon");
    toggle_windows("shop_perk");
    break;

    case 13:

    toggle_windows("shop_perk");
    toggle_windows("shop_weapon");

    break;
    default:
        if(id > 13 && id < 24 ){

            if(get_money() >= 100){
                remove_money(100);
                unlock_weapon(id-13);
                save_data();
            }
        }
        if(id > 23 && id < 29 ){

            if(get_money() >= 100){
                remove_money(100);
                unlock_passive(id-23);
                save_data();
            }
        }
        if(id > 28 && id < 34 ){

            if(get_money() >= 100){
                remove_money(100);
                unlock_active(id-28);
                save_data();
            }
        }

    break;

}


}


void update_windows(Uint8 *gamestate){

    int i, j;
    Uint8 clicking = click_status();

    GFC_Vector2D mouse_loc = absolute_mouse_pos();
    GFC_Circle click_loc = gfc_circle(mouse_loc.x, mouse_loc.y, 10.0f);

    if (clicking == 1) {
        typingarea = NULL;
        istyping = 0;
    }

    for (i = 0; i < win_count; i++){

        if (!(ui[i]->active)) continue;
        W_Window *win = (W_Window *) ui[i]->data;

        for (j = 0; j < win->obj_count; j++) {

            if (!win->objs[j]->active) continue;

            switch (win->objs[j]->type){


                case 1:{
                    W_Button *butt = (W_Button*) win->objs[j]->data;

                    GFC_Vector2D position = gfc_vector2d(0,0);
                    position.x = win->position.x + butt->pos.x;
                    position.y = win->position.y + butt->pos.y;
                    GFC_Rect box = gfc_rect(position.x, position.y, butt->dim.x, butt->dim.y);

                    if(butt->sprite){
                        gf2d_sprite_render(
                            butt->sprite,
                            position,
                            &butt->size,
                            NULL,
                            NULL,
                            NULL,
                            &base,
                            NULL,
                            0);
                    }

                    if (clicking) {
                        if (gfc_circle_rect_overlap(click_loc, box)){
                            event_activate(butt->eventId, gamestate);
                            slog("event %d", butt->eventId);
                        }
                    }

                    break;
                }

                case 2:{
                    W_Label *la = (W_Label*) win->objs[j]->data;

                    // show text at font size at location in regards to size
                    GFC_Vector2D position = gfc_vector2d(0,0);
                    position.x = win->position.x + la->pos.x;
                    position.y = win->position.y + la->pos.y;

                    // background
                    if(la->sprite){
                        gf2d_sprite_render(
                            la->sprite,
                            position,
                            &la->size,
                            NULL,
                            NULL,
                            NULL,
                            &base,
                            NULL,
                            0);
                    }

                    // display text

                    font_draw_test(la->text, la->f_size, base, position);

                    break;
                }

                case 3:{
                    W_TextArea *ta = (W_TextArea*) win->objs[j]->data;

                    GFC_Vector2D position = gfc_vector2d(0,0);
                    position.x = win->position.x + ta->pos.x;
                    position.y = win->position.y + ta->pos.y;
                    GFC_Rect box = gfc_rect(position.x, position.y, ta->dim.x, ta->dim.y);

                    if(ta->sprite){
                        gf2d_sprite_render(
                            ta->sprite,
                            position,
                            &ta->size,
                            NULL,
                            NULL,
                            NULL,
                            &base,
                            NULL,
                            0);
                    }

                    // display text

                    if (clicking) {
                        if (gfc_circle_rect_overlap(click_loc, box)){
                            typingarea = ta;
                            istyping = 1;
                        }
                    }

                    if (strlen(ta->text) > 0){
                        font_draw_test(ta->text, ta->f_size, base, position);
                    } else {
                        font_draw_test(ta->placeholder, ta->f_size, base, position);
                    }

                    break;
                }

                case 4:{
                    update_container(win->objs[j], &click_loc, &clicking, gamestate);
                    break;
                }
                case 5:{
                    W_Image *img = (W_Image*) win->objs[j]->data;

                    // show text at font size at location in regards to size
                    GFC_Vector2D position = gfc_vector2d(0,0);
                    position.x = win->position.x + img->pos.x;
                    position.y = win->position.y + img->pos.y;

                    // background
                    if(img->sprite){
                        gf2d_sprite_render(
                            img->sprite,
                            position,
                            &img->size,
                            NULL,
                            NULL,
                            NULL,
                            &base,
                            NULL,
                            0);
                    }
                break;
                }
            }
        }
    }
}

W_Element *create_image(SJson *j, W_Element *parent){

    SJson *value;
    char gamesprite[50] = "";
    int framewidth, frameheight;
    W_Image *img = (W_Image *) malloc(sizeof(W_Image));
    W_Element *ele = (W_Element *) malloc(sizeof(W_Element));
    ele->data = (W_Image*) img;
    ele->parent = parent;


    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &img->size.x);
    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &img->size.y);

    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &img->pos.x);
    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &img->pos.y);

    strcpy(gamesprite, sj_object_get_string(j, "sprite"));
    sj_object_get_uint8(j, "type", &ele->type);
    sj_object_get_int(j, "sprite_h", &frameheight);
    sj_object_get_int(j, "sprite_w", &framewidth);
    sj_object_get_uint8(j, "active", &ele->active);

    img->dim.x = framewidth;
    img->dim.y = frameheight;

    img->sprite = gf2d_sprite_load_all(
        gamesprite,
        framewidth,
        frameheight,
        1,
        0
    );

    return ele;

}


W_Element *create_button(SJson *j, W_Element *parent){

    SJson *value;
    char gamesprite[50] = "";
    int framewidth, frameheight;
    W_Button *butt /* ha */ = (W_Button *) malloc(sizeof(W_Button));
    W_Element *ele = (W_Element *) malloc(sizeof(W_Element));
    ele->data = (W_Button*) butt;
    ele->parent = parent;


    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &butt->size.x);
    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &butt->size.y);

    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &butt->pos.x);
    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &butt->pos.y);


    strcpy(gamesprite, sj_object_get_string(j, "sprite"));
    sj_object_get_uint8(j, "active", &ele->active);
    sj_object_get_uint8(j, "type", &ele->type);
    sj_object_get_uint8(j, "eventid", &butt->eventId);
    sj_object_get_int(j, "sprite_h", &frameheight);
    sj_object_get_int(j, "sprite_w", &framewidth);

    butt->dim.x = framewidth;
    butt->dim.y = frameheight;

    if(butt->eventId > 13 && butt->eventId < 34){

        butt->sprite = gf2d_sprite_load_all(
            gamesprite,
            framewidth,
            frameheight,
            2,
            0
        );
        return ele;
    }

    butt->sprite = gf2d_sprite_load_all(
        gamesprite,
        framewidth,
        frameheight,
        1,
        0
    );

    return ele;
}



W_Element *create_textarea(SJson *j, W_Element *parent){


    SJson *value;
    char gamesprite[50] = "";
    int framewidth, frameheight;
    W_TextArea *ta /* ha */ = (W_TextArea *) malloc(sizeof(W_TextArea));
    W_Element *ele = (W_Element *) malloc(sizeof(W_Element));
    memset(ta->text, 0, 255);
    memset(ta->placeholder, 0, 255);
    ta->placeholder[0] = '\0';
    ta->text[0] = '\0';
    ele->data = (W_TextArea*) ta;
    ele->parent = parent;


    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &ta->size.x);
    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &ta->size.y);

    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &ta->pos.x);
    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &ta->pos.y);

    strcpy(gamesprite, sj_object_get_string(j, "sprite"));
    strcat(ta->placeholder, sj_object_get_string(j, "placeholder"));
    sj_object_get_uint8(j, "fontsize", &ta->f_size);
    sj_object_get_uint8(j, "type", &ele->type);
    sj_object_get_uint8(j, "id", &ta->id);
    sj_object_get_int(j, "sprite_h", &frameheight);
    sj_object_get_int(j, "sprite_w", &framewidth);
    sj_object_get_uint8(j, "active", &ele->active);

    ta->dim.x = framewidth;
    ta->dim.y = frameheight;

    ta->sprite = gf2d_sprite_load_all(
        gamesprite,
        framewidth,
        frameheight,
        1,
        0
    );

    return ele;
}

void toggle_windows(char *w){

    W_Element *win = (gfc_hashmap_get(hash, w));

    if(win->active) {
        win->active = 0;
    } else {
    win->active = 1;
    }
}

W_Element *create_label(SJson *j, W_Element *parent){


    SJson *value;
    char gamesprite[50] ="";
    int framewidth, frameheight;
    W_Label *la /* ha */ = (W_Label *) malloc(sizeof(W_Label));
    W_Element *ele = (W_Element *) malloc(sizeof(W_Element));
    la->text[0] = '\0';
    ele->data = (W_Label*) la;
    ele->parent = parent;

    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &la->size.x);
    value = sj_object_get_value(j, "size");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &la->size.y);

    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,0);
    sj_get_float_value(value, &la->pos.x);
    value = sj_object_get_value(j, "position");
    value = sj_array_get_nth(value,1);
    sj_get_float_value(value, &la->pos.y);

    strcpy(gamesprite, sj_object_get_string(j, "sprite"));
    if(sj_object_get_string(j, "text")!= NULL)strcpy(la->text, sj_object_get_string(j, "text"));

    sj_object_get_uint8(j, "fontsize", &la->f_size);
    sj_object_get_uint8(j, "type", &ele->type);
    sj_object_get_uint8(j, "id", &la->id);
    sj_object_get_int(j, "sprite_h", &frameheight);
    sj_object_get_int(j, "sprite_w", &framewidth);
    sj_object_get_uint8(j, "active", &ele->active);

    la->dim.x = framewidth;
    la->dim.y = frameheight;

    la->sprite = gf2d_sprite_load_all(
        gamesprite,
        framewidth,
        frameheight,
        1,
        0
    );

    return ele;

}

void free_window(W_Element *w){

    int j;

    if (!w)  return;

    W_Window *win = (W_Window *) w->data;

    if(!win) return;

    for (j = 0; j < win->obj_count; j++){

        if (!win->objs[j]) continue;

        switch (win->objs[j]->type){
            case 1:{
                W_Button *butt = (W_Button*) win->objs[j]->data;
                if(butt->sprite) gf2d_sprite_free(butt->sprite);
                free(win->objs[j]->data);
                free(win->objs[j]);
                break;
            }
            case EL_LABEL:{
                W_Label *la = (W_Label*) win->objs[j]->data;
                if(la->sprite) gf2d_sprite_free(la->sprite);
                free(win->objs[j]->data);
                free(win->objs[j]);
                break;
            }
            case EL_TEXTAREA:{
                W_TextArea *ta = (W_TextArea*) win->objs[j]->data;
                if(ta->sprite) gf2d_sprite_free(ta->sprite);
                free(win->objs[j]->data);
                free(win->objs[j]);
                break;
            }
            case EL_CONTAINER:
                free_window(win->objs[j]);
                break;
            case EL_IMAGE:{
                W_Image *img = (W_Image*) win->objs[j]->data;
                if(img->sprite) gf2d_sprite_free(img->sprite);
                free(win->objs[j]->data);
                free(win->objs[j]);
                break;
            }
            default:
                break;
        }

    }

    free(win->objs);
    free(win);
    free(w);

}

void clean_ui (){
    int i;

    for (i = 0; i < win_count; i++){

        free_window(ui[i]);
    }
    // free everything after game ends
    gfc_hashmap_free(hash);
    free(ui);
}
