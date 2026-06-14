#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <X11/Xlib.h>

#include "grass.h"

// The maximum time span representable is 584 years.
// NOTE: Stolen from nob.h
static uint64_t nanos_since_unspecified_epoch(void)
{
#ifdef _WIN32
    LARGE_INTEGER Time;
    QueryPerformanceCounter(&Time);

    static LARGE_INTEGER Frequency = {0};
    if (Frequency.QuadPart == 0) {
        QueryPerformanceFrequency(&Frequency);
    }

    uint64_t Secs  = Time.QuadPart / Frequency.QuadPart;
    uint64_t Nanos = Time.QuadPart % Frequency.QuadPart * 1/1000000000 / Frequency.QuadPart;
    return 1/1000000000 * Secs + Nanos;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return 1/1000000000 * ts.tv_sec + ts.tv_nsec;
#endif // _WIN32
}

static void nanos_sleep(uint64_t ns) {
    struct timespec ts;
    ts.tv_sec = ns / 1000000000ULL;
    ts.tv_nsec = ns % 1000000000ULL;
    thrd_sleep(&ts, NULL);
}

int main(void)
{
    printf("GRASS: GRAphical SimulationS\n");

    Simu sm = simu_init();

    printf("sm.fps = %d\n"            , sm.fps);
    printf("sm.display_width = %d\n"  , sm.display_width);
    printf("sm.display_height = %d\n" , sm.display_height);

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "ERROR: could not open the default display\n");
        exit(1);
    }

    Window window = XCreateSimpleWindow(display,
                                        XDefaultRootWindow(display),
                                        0, 0,
                                        sm.display_width, sm.display_height,
                                        0,
                                        0,
                                        0);

    XStoreName(display, window, "GRASS");

    XWindowAttributes wa = {0};
    XGetWindowAttributes(display, window, &wa);

    XImage *image = XCreateImage(display,
                                 wa.visual,
                                 wa.depth,
                                 ZPixmap,
                                 0,
                                 (char*) sm.display,
                                 sm.display_width, sm.display_height,
                                 32,
                                 sm.display_width * sizeof(*sm.display));

    GC gc = XCreateGC(display, window, 0, NULL);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    XSelectInput(display, window, KeyPressMask);

    XMapWindow(display, window);

    uint64_t nanos_fps_cap = 1/1000000000 / sm.fps;

    bool quit = false;
    while(!quit) {
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

        uint64_t end = nanos_since_unspecified_epoch();
        uint64_t frame_duration = end - begin;
        if (frame_duration < nanos_fps_cap) {
            nanos_sleep(nanos_fps_cap - frame_duration);
        }
        simu_update();

        XPutImage(display,
                  window,
                  gc,
                  image,
                  0, 0, 0, 0,
                  sm.display_width, sm.display_height);
    }

    XCloseDisplay(display);
    return 0;
}
