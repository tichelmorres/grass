#include <stdio.h>

#include "linalc.h"

// Point triangle_determine_max_coordinates(uint32_t x1, uint32_t y1,
//                                          uint32_t x2, uint32_t y2,
//                                          uint32_t x3, uint32_t y3)
// {
//     uint32_t mx = 0;
//     if (x1 > mx) mx = x1;
//     if (x2 > mx) mx = x2;
//     if (x3 > mx) mx = x3;

//     uint32_t my = 0;
//     if (y1 > my) my = y1;
//     if (y2 > my) my = y2;
//     if (y3 > my) my = y3;

//     return (Point) {
//         .x = mx,
//         .y = my
//     };
// }

// Point triangle_determine_min_coordinates(uint32_t x1, uint32_t y1,
//                                          uint32_t x2, uint32_t y2,
//                                          uint32_t x3, uint32_t y3)
// {
//     uint32_t mx = 0;
//     if (x1 < mx) mx = x1;
//     if (x2 < mx) mx = x2;
//     if (x3 < mx) mx = x3;

//     uint32_t my = 0;
//     if (y1 < my) my = y1;
//     if (y2 < my) my = y2;
//     if (y3 < my) my = y3;

//     return (Point) {
//         .x = mx,
//         .y = my
//     };
// }

// TODO: implement base/height version
void draw_triangle(uint32_t side, Simu* sm, Color color)
{
    Point center = {
        .x = sm->display_width/2,
        .y = sm->display_height/2
    };
    uint32_t lim = 1;
    for (uint32_t y = (center.y - side); y < (center.y + side); ++y) {
        for (uint32_t x = (center.x - side); x < (center.x - side + lim); ++x) {
            ((Color*)sm->display)[sm->display_width * y + x] = color;
        }
        // lim = ((center.x - side + lim) == (center.x + side)) ? 0 : ++lim;
        if (center.x - side + lim == center.x + side) {
            lim = 0;
        } else {
            ++lim;
        }
    }
}
