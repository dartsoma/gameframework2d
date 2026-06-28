#define __UI_CORE_H__
#ifndef __UI_CORE_H__

#include "gfc_hashmap.h"

// Amount of screens active at once
#define MAX_STACK 10
// Amount of screens to be loaded at a time
#define MAX_UI 32


typedef struct UIElement {

    struct UIElement *parent;

    Uint8 visible;
    Uint8 z_index;
    // component bit mask
    int comp_mask;

} UIElement;

typedef struct {

    Uint8 loaded;
    UIElement *root;

    void (* init)(struct UIScreen *);
    void (* destroy)(struct UIScreen *);
    void (* draw_all)(struct UIScreen *);
    void (* update_all)(struct UIScreen *);

} UIScreen;

typedef struct{

    UIScreen *active_screen[MAX_STACK];
    int draw_order[MAX_STACK];
    GFC_HashMap *cache;

} UIManager;


#endif
