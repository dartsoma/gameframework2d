#ifndef __WINDOW_H__
#define __WINDOW_H__
/**
 @note each window will have its own blueprint in json
 **/

#include "gfc_hashmap.h"
#include "gf2d_sprite.h"
#include "simple_json.h"

#define EL_NONE 0
#define EL_BUTTON 1
#define EL_LABEL 2
#define EL_TEXTAREA 3
#define EL_CONTAINER 4

typedef struct Element {
    void *data;
    struct Element *parent;
    Uint8 type;
    Uint8 zindex;
} W_Element;


typedef struct {
    GFC_Vector2D w_size;
    GFC_Vector2D w_position;
    char* name;
    W_Element **objs;
    Uint8 active;
} W_Window;

typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    Sprite *sprite;
    Uint8 EventId;
    Uint8 active;
} W_Button;

typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    Sprite *sprite;
    char* obj;
    Uint8 active;
} W_Label;

typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    char* text;
    char* placeholder;
    Uint8 active;
} W_TextArea;


void define_windows();

void update_windows(W_Element *w);

W_Window *create_contianer(SJson j, W_Element parent);

W_Button *create_button(SJson j, W_Element parent);

W_TextArea *create_textarea(SJson j, W_Element parent);

W_Label *create_label(SJson j, W_Element parent);


void clean_ui ();


#endif
