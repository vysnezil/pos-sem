#ifndef INPUT_H
#define INPUT_H

void input_init(void (*callback)(int,int,void*), void* context);
void input_destroy();

#endif


