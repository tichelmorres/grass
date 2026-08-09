#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

#include "grass.h"

// The maximum time span representable is 584 years.
static uint64_t nanos_since_unspecified_epoch(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

static void nanos_sleep(uint64_t ns) {
    struct timespec ts;
    ts.tv_sec = ns / 1000000000ULL;
    ts.tv_nsec = ns % 1000000000ULL;
    thrd_sleep(&ts, NULL);
}

int main(void) {
    printf("GRASS: GRAphical SimulationS\n");

    Simu sm = simu_init();

    printf("sm.fps            = %d\n", sm.fps);
    printf("sm.display_width  = %d\n", sm.display_width);
    printf("sm.display_height = %d\n", sm.display_height);

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "ERROR: could not open the default display\n");
        exit(1);
    }

    Bool mit_shm = XShmQueryExtension(display);

    Window window = XCreateSimpleWindow(display, XDefaultRootWindow(display),
                                        0, 0, sm.display_width, sm.display_height,
                                        0, 0, 0);

    XStoreName(display, window, "GRASS");

    XWindowAttributes wa = {0};
    XGetWindowAttributes(display, window, &wa);

    XImage *image;
    XShmSegmentInfo shminfo = {0};

    if (mit_shm) {
        shminfo.shmid = shmget(IPC_PRIVATE,
                               sm.display_width * sm.display_height * sizeof(uint32_t),
                               IPC_CREAT | 0777);
        sm.display = shmat(shminfo.shmid, 0, 0);
        shminfo.shmaddr = (char*) sm.display;
        shminfo.readOnly = False;
        XShmAttach(display, &shminfo);
        image = XShmCreateImage(display, wa.visual, wa.depth, ZPixmap,
                                (char*) sm.display, &shminfo,
                                sm.display_width, sm.display_height);
    } else {
        image = XCreateImage(display, wa.visual, wa.depth, ZPixmap, 0,
                             (char*) sm.display,
                             sm.display_width, sm.display_height,
                             32, sm.display_width * sizeof(uint32_t));
    }

    GC gc = XCreateGC(display, window, 0, NULL);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    XSelectInput(display, window, KeyPressMask);

    XMapWindow(display, window);

    uint64_t nanos_fps_cap = 1000000000ULL / sm.fps;

    bool quit = false;
    while (!quit) {
        uint64_t begin = nanos_since_unspecified_epoch();

        while(XPending(display) > 0) {
            XEvent event = {0};
            XNextEvent(display, &event);

            switch(event.type) {
            case KeyPress: {
                switch (XLookupKeysym(&event.xkey, 0)) {
                case 'q':
                    quit = true;
                    break;
                }
            }
            break;

            case ClientMessage: {
                if ((Atom) event.xclient.data.l[0] == wm_delete_window) {
                    quit = true;
                }
            }
            break;

            default: {};
            }
        }

        simu_update(&sm);
        XPutImage(display, window, gc, image,
                  0, 0, 0, 0,
                  sm.display_width, sm.display_height);

        uint64_t end = nanos_since_unspecified_epoch();
        uint64_t frame_duration = end - begin;
        if (frame_duration < nanos_fps_cap) {
            nanos_sleep(nanos_fps_cap - frame_duration);
        }
    }

    XCloseDisplay(display);
    return 0;
}
