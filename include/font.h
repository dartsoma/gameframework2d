#ifndef __FONT_H__
#define __FONT_H__

#include "gfc_text.h"
#include "gfc_list.h"
#include "gfc_color.h"
#include "gfc_vector.h"

typedef enum {
    FS_small,
    FS_medium = 10,
    FS_large = 20,
    FS_MAX
} FontStyles;


void font_close();

void font_init();

void font_draw_test(const char *text, FontStyles style, GFC_Color color, GFC_Vector2D position);

#endif
