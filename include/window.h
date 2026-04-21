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
#define EL_IMAGE 5



typedef struct Element {
    void *data;
    struct Element *parent;
    Uint8 type;
    Uint8 active;
    int zindex;
} W_Element;


typedef struct {
    GFC_Vector2D size;
    GFC_Vector2D position;
    char name[25];
    W_Element **objs;
    Uint8 obj_count;
} W_Window;

typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    Sprite *sprite;
    Uint8 eventId;
} W_Button;


typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    Sprite *sprite;
    char text[255];
    Uint8 f_size;
} W_Label;

typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    Sprite *sprite;
} W_Image;

typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    Sprite *sprite;
    char text[255];
    Uint8 f_size;
    char placeholder[255];
} W_TextArea;


void define_windows();

void toggle_windows(W_Element *w);

void update_container(W_Element *w, GFC_Circle *click_loc, Uint8 *clicking);

void update_windows();

void free_window(W_Element *w);

W_Element *create_container(SJson *j, W_Element *parent);

W_Element *create_image(SJson *j, W_Element *parent);

W_Element *create_button(SJson *j, W_Element *parent);

W_Element *create_textarea(SJson *j, W_Element *parent);

W_Element *create_label(SJson *j, W_Element *parent);


void clean_ui ();


#endif
