#ifndef CIRCLE_H
#define CIRCLE_H

#include "../object.h"

void circle_init(object* object, int id, int x, int y, int color, int radius, void(*on_click)(struct object* object, void* context), void* context);

#endif
