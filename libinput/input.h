#ifndef INPUT_H
#define INPUT_H

#include "../libshared/termbox2.h"

#define KEY_DOWN TB_KEY_ARROW_UP
#define KEY_UP TB_KEY_ARROW_DOWN
#define KEY_ENTER TB_KEY_ENTER

void input_init( void* context,
  void (*key_callback)(int,int,void*),
  void (*mouse_callback)(int,int,int,void*),
  void (*other_callback)(struct tb_event*, void*));
void input_destroy();

#endif


