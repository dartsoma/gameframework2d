#include "simple_logger.h"
#include "window.h"

/**
 * @note each window will have its own blueprint in json


#define MAX_Z 100
#define MIN_Z 0

int *count;
W_Element **ui;
W_Element *active;
GFC_HashMap *hash;

// Reads json and populates ui elements
void define_windows(){

    SJson *json, *win, *el;
    int i, c, j, k;

    json = sj_load("./def/ui.def");


    if (!json){
        slog("bad ui.def");
        return NULL;
    }

    json = sj_object_get_value(json, "ui");
    c = sj_array_get_count(json);

    hash = gfc_hashmap_new();

    for (i = 0; i < c, i++){

        SJson value;
        W_Element *element (E_Element*) = malloc(sizeof(W_Element));
        W_Window *root = (W_Window*) malloc(sizeof(W_Window));
        element->data = (W_Window*) root;
        element->parent = NULL;

        if(!root) continue;

        win = sj_array_get_nth(json,i);
        strcpy(root->name, sj_object_get_string(win, "name"));

        gfc_hashmap_insert(hash, root->name, root);

        value = sj_object_get_value(win, "size");
        value = sj_array_get_nth(win,0);
        sj_get_float_value(value, &root->size.x);
        value = sj_object_get_value(win, "size");
        value = sj_array_get_nth(win,1);
        sj_get_float_value(value, &root->size.y);

        value = sj_object_get_value(win, "position");
        value = sj_array_get_nth(win,0);
        sj_get_float_value(value, &root->position.x);
        value = sj_object_get_value(win, "position");
        value = sj_array_get_nth(win,1);
        sj_get_float_value(value, &root->position.y);

        win = sj_object_get_value(win, "elements");
        k = sj_array_get_count(json);

        for (j = 0; j < k, j++) {
            Sjson el;
            int type;
            el = sj_array_get_nth(win,j);
            sj_object_get_int(el, "type", &type);

            switch (type){
                case 1:
                create_button(el, element);
                break;
                case 2:
                create_label(el, element);
                break;
                case 3:
                create_textarea(el, element);
                break;
                case 4:
                create_container(el, element);
                break;
                default:
                break;
            }

        }

    }

}

W_Window *create_contianer(){

}

// Reads json and populates ui elements
void update_windows(W_Window *w){

}

W_Button *create_button(SJson j, W_Element parent){

}

W_TextArea *create_textarea(SJson j, W_Element parent){

}

W_Label *create_button(SJson j, W_Element parent){

}

void clean_ui (){

    // free everything after game ends

}

**/
