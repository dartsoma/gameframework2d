#include "simple_logger.h"
#include "gfc_shape.h"
#include "gfc_vector.h"
#include "camera.h"
#include "window.h"


/**
* @note each window will have its own blueprint in json
**/

#define MAX_Z 100
#define MIN_Z 0

int win_count;
W_Element **ui; // stores all window references
Uint8 istyping;
W_TextArea *typingarea;
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
        W_Element *element = (W_Element *) malloc(sizeof(W_Element));
        W_Window *root = (W_Window *) malloc(sizeof(W_Window));
        if(!root) continue;
        element->data = (W_Window *) root;
        root->name[24] = '\0';
        element->active = 1;
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

        win = sj_object_get_value(win, "elements");
        k = sj_array_get_count(win);
        root->objs = (W_Element**) malloc(sizeof(W_Element*)*k);

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


    W_Element *element =  (W_Element*) malloc(sizeof(W_Element));
    W_Window *cont = (W_Window*) malloc(sizeof(W_Window));
    if(!cont) {
        free(element);
        return NULL;
    }
    element->active = 1;
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

    j = sj_object_get_value(j, "elements");
    k = sj_array_get_count(j);
    cont->objs = (W_Element**) malloc(sizeof(W_Element*)*k);
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
        slog("Sub Entity %d complete", i);
    }

    return element;
}



// Reads json and populates ui elements

void update_container(W_Element *w, GFC_Circle *click_loc, Uint8 *clicking, Uint8 *gamestate){

    int i;

    W_Window *win = (W_Window *) w->data;

    for (i = 0; i < win->obj_count; i++) {

        if (!(win->objs[i]->active)) continue;

        switch(win->objs[i]->type){
            case 1:{
                W_Button *butt = (W_Button*) win->objs[i]->data;

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
                        slog("text box clicked");
                    }
                }

                break;
            }

            case 4:{
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

    toggle_windows(gfc_hashmap_get(hash, "mainmenu"));
    // toggle_windows(gfc_hashmap_get(hash, "mapselect"));
    *gamestate = 2;
    break;

    case 3:
    toggle_windows(gfc_hashmap_get(hash, "mainmenu"));
    // toggle_windows(gfc_hashmap_get(hash, "shop"));
    *gamestate = 3;
    break;

    case 4:
    toggle_windows(gfc_hashmap_get(hash, "mainmenu"));
    // toggle_windows(gfc_hashmap_get(hash, "editor"));
    *gamestate = 6;
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

            if (!(win->objs[j]->active)) continue;
            switch(win->objs[j]->type){
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
                            slog("text box clicked");
                        }
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
    ele->active = 1;
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
    char gamesprite[50] ="";
    int framewidth, frameheight;
    W_Button *butt /* ha */ = (W_Button *) malloc(sizeof(W_Button));
    W_Element *ele = (W_Element *) malloc(sizeof(W_Element));
    ele->active = 1;
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
    sj_object_get_uint8(j, "type", &ele->type);
    sj_object_get_uint8(j, "eventid", &butt->eventId);
    sj_object_get_int(j, "sprite_h", &frameheight);
    sj_object_get_int(j, "sprite_w", &framewidth);

    butt->dim.x = framewidth;
    butt->dim.y = frameheight;

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
    ta->text[254] = '\0';
    ele->active = 1;
    ele->data = (W_Button*) ta;
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
    strcpy(ta->placeholder, sj_object_get_string(j, "placeholder"));
    sj_object_get_uint8(j, "fontsize", &ta->f_size);
    sj_object_get_uint8(j, "type", &ele->type);
    sj_object_get_int(j, "sprite_h", &frameheight);
    sj_object_get_int(j, "sprite_w", &framewidth);

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

void toggle_windows(W_Element *w){

    if(w->active) {
        w->active = 0;
    } else {
    w->active = 1;
    }

}

W_Element *create_label(SJson *j, W_Element *parent){


    SJson *value;
    char gamesprite[50] ="";
    int framewidth, frameheight;
    W_Label *la /* ha */ = (W_Label *) malloc(sizeof(W_Label));
    W_Element *ele = (W_Element *) malloc(sizeof(W_Element));
    la->text[254] = '\0';
    ele->active = 1;
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
    strcpy(la->text, sj_object_get_string(j, "text"));
    sj_object_get_uint8(j, "fontsize", &la->f_size);
    sj_object_get_uint8(j, "type", &ele->type);
    sj_object_get_int(j, "sprite_h", &frameheight);
    sj_object_get_int(j, "sprite_w", &framewidth);

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

    W_Window *win = (W_Window *) w->data;

    for (j = 0; j < win->obj_count; j++){

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
