#include <stdint.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#ifndef SIMU_H_
#define SIMU_H_

#define TODO(message) do {                                                                    \
                          fprintf(stderr, "%s:%d: TODO: %s \n", __FILE__, __LINE__, message); \
                          abort();                                                            \
                      } while(0)

#define ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))

typedef struct {
    uint8_t b, g, r, a;
} Color;

typedef struct {
    uint8_t   fps;
    uint32_t* display;
    uint16_t  display_width;
    uint16_t  display_height;
    int       shmid;   // -1 if SHM unavailable, main.c uses this to build XShmSegmentInfo
} Simu;

Simu simu_init();
void simu_update(Simu* sm);

#endif // SIMU_H_
