#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

#include "grass.h"
#include "lib/linalc.h"

#define FPS 60
#define DISPLAY_WIDTH 1920
#define DISPLAY_HEIGHT 1080

static Color display_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

Simu simu_init(void) {
    int shmid = shmget(IPC_PRIVATE,
                       DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t),
                       IPC_CREAT | 0777);

    uint32_t *pixels = NULL;
    if (shmid >= 0) {
        pixels = shmat(shmid, 0, 0);
        if (pixels == (void*)-1) {
            pixels = NULL;
            shmid  = -1;
        }
    }

    if (!pixels) {
        pixels = mmap(NULL,
                      DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t),
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }

    return (Simu) {
        .fps            = FPS,
        .display        = pixels,
        .display_width  = DISPLAY_WIDTH,
        .display_height = DISPLAY_HEIGHT,
        .shmid          = shmid,
    };
}

void simu_update(Simu* sm) {
    // reset whole frame to black background
    for(size_t i = 0; i < ARRAY_LEN(display_buffer); ++i) {
        display_buffer[i] = (Color) {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = 255
        };
    }

    Color triangle_color = {
        .r = 255,
        .g = 0,
        .b = 0,
        .a = 255
    };
    // uint32_t cx = DISPLAY_WIDTH/2;
    // uint32_t cy = DISPLAY_HEIGHT/2;
    // draw_triangle(cx - DISPLAY_WIDTH/4, cy + DISPLAY_HEIGHT/4,
    //               cx + DISPLAY_WIDTH/4, cy + DISPLAY_HEIGHT/4,
    //               cx,                   cy - DISPLAY_HEIGHT/4,
    //               sm, triangle_color);
    draw_triangle(DISPLAY_HEIGHT/5, sm, triangle_color);
}
