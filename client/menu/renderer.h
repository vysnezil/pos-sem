#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H

#include "../../libdraw/draw.h"
#include "../graphics/graphics.h"

#define MENU_WIDTH 16
#define MENU_HEIGHT 8
#define MENU_BG 0xfb

void render_frame(menu* menu, int width, int height);

#endif
