#include "grass.h"

#define FPS 60
#define DISPLAY_WIDTH 1920
#define DISPLAY_HEIGHT 1080

static Color display_buffer[DISPLAY_WIDTH*DISPLAY_HEIGHT];

Simu simu_init(void) {
    return (Simu) {
        .fps            = FPS,
        .display        = (uint32_t*)display_buffer,
        .display_width  = DISPLAY_WIDTH,
        .display_height = DISPLAY_HEIGHT
    };
}

void simu_update(void) {
    for(size_t i = 0; i < ARRAY_LEN(display_buffer); ++i) {
        display_buffer[i] = (Color) {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = 255
        };
    }
}
