#include "libgraphics/graphics.h"
#include "libinput/input.h"

int main ()
{
    graphics_init();
    input_init();

    draw_pixel(10, 10);

    graphics_refresh();

    input_getkey();

    input_destroy();
    graphics_destroy();
    return 0;
}
