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

// @TODO      Implement base/height versions
// @Refactor  Generalize Point structures.
//               (to always use center is
//                not a good idea.       )

/*
       we start here            
             |  top             
             v   v              
    left ->  +-------+  <- right
             |.|    /           
             |-    /            
             |    /             
             |   /              
             |  /               
             | /                
             |/                 
             +                  
             ^                  
           bottom               
 */
void linalc_draw_ul_triangle_by_side(uint32_t side, Simu* sm, Color color)
{
    Point center = {
        .x = sm->display_width/2,
        .y = sm->display_height/2
    };

    uint32_t half_side = side/2;
    uint32_t left      = center.x - half_side;
    uint32_t right     = center.x + half_side;
    uint32_t top       = center.y - half_side;
    uint32_t bottom    = center.y + half_side;

    for (uint32_t y = top; y < bottom; ++y) {
        // (y - top) => How many pixels to erase from this row?
        for (uint32_t x = left; x < (right - (y - top)); ++x) {
            ((Color*)sm->display)[sm->display_width * y + x] = color;
        }
    }
}

/*
      we start here            
            v                  
           top                 
            v                  
            +                  
            |\                 
            | \                
            |  \               
            |   \              
            |    \             
            |_    \            
            |.|    \           
   left ->  +-------+  <- right
                ^              
              bottom           
 */
void linalc_draw_bl_triangle_by_side(uint32_t side, Simu* sm, Color color)
{
    Point center = {
        .x = sm->display_width/2,
        .y = sm->display_height/2
    };

    uint32_t half_side = side/2;
    uint32_t left      = center.x - half_side;
  //uint32_t right     = never used;
    uint32_t top       = center.y - half_side;
    uint32_t bottom    = center.y + half_side;

    for (uint32_t y = top; y < bottom; ++y) {
        // (y - top + 1) => How many pixels to render this row?
        for (uint32_t x = left; x < (left + (y - top + 1)); ++x) {
            ((Color*)sm->display)[sm->display_width * y + x] = color;
        }
    }
}

/*
       we start here            
             |  top             
             v   v              
    left ->  +-------+  <- right
              \    |.|          
               \    -|          
                \    |          
                 \   |          
                  \  |          
                   \ |          
                    \|          
                     +          
                     ^          
                   bottom       
 */
void linalc_draw_ur_triangle_by_side(uint32_t side, Simu* sm, Color color)
{
    Point center = {
        .x = sm->display_width/2,
        .y = sm->display_height/2
    };

    uint32_t half_side = side/2;
    uint32_t left      = center.x - half_side;
    uint32_t right     = center.x + half_side;
    uint32_t top       = center.y - half_side;
    uint32_t bottom    = center.y + half_side;

    for (uint32_t y = top; y < bottom; ++y) {
        // (y - top) => How many pixels to skip this row?
        for (uint32_t x = (left + (y - top)); x < right; ++x) {
            ((Color*)sm->display)[sm->display_width * y + x] = color;
        }
    }
}

/*
              we start here    
                    v          
                   top         
                    v          
                    +          
                   /|          
                  / |          
                 /  |          
                /   |          
               /    |          
              /    _|          
             /    |.|          
   left ->  +-------+  <- right
                ^              
              bottom           
 */
void linalc_draw_br_triangle_by_side(uint32_t side, Simu* sm, Color color)
{
    Point center = {
        .x = sm->display_width/2,
        .y = sm->display_height/2
    };

    uint32_t half_side = side/2;
  //uint32_t left      = never used;
    uint32_t right     = center.x + half_side;
    uint32_t top       = center.y - half_side;
    uint32_t bottom    = center.y + half_side;

    for (uint32_t y = top; y < bottom; ++y) {
        // (y - top) => How many pixels to go back in this row?
        for (uint32_t x = (right - (y - top)); x < right; ++x) {
            ((Color*)sm->display)[sm->display_width * y + x] = color;
        }
    }
}

// @TODO  Lissajous curve rendering?
// The realistic approach, I guess, is to
//     provide translation functions to
//     map logical pixel positions to
//     geometrical coordinates as would
//     be visually expected.
