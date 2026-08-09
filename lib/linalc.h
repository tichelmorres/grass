#ifndef LINALC_H_
#define LINALC_H_

#include <stdint.h>

#include "../grass.h"

typedef struct {
    uint32_t x;
    uint32_t y;
} Point;

// Point triangle_determine_min_coordinates(uint32_t x1, uint32_t y1,
//                                          uint32_t x2, uint32_t y2,
//                                          uint32_t x3, uint32_t y3);

// Point triangle_determine_max_coordinates(uint32_t x1, uint32_t y1,
//                                          uint32_t x2, uint32_t y2,
//                                          uint32_t x3, uint32_t y3);

void draw_triangle(uint32_t side, Simu* sm, Color color);

#endif // LINALC_H_
