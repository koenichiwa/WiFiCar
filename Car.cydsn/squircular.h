// Adapted from: http://squircular.blogspot.com/2015/09/mapping-circle-to-square.html
#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "mytypes.h"

fvector2_t circle_to_square(fvector2_t * circle)
{
    assert(fvector_length(circle)<=1.0);
    
    float u2 = circle->x * circle->x;
    float v2 = circle->y * circle->y;
    
    float twosqrt2 = 2.0 * M_SQRT2;
    float subtermx = 2.0 + u2 - v2;
    float subtermy = 2.0 - u2 + v2;
    float termx1 = subtermx + circle->x * twosqrt2;
    float termx2 = subtermx - circle->x * twosqrt2;
    float termy1 = subtermy + circle->y * twosqrt2;
    float termy2 = subtermy - circle->y * twosqrt2;
    
    fvector2_t square;
    square.x = 0.5 * sqrt(termx1) - 0.5 * sqrt(termx2);
    square.y = 0.5 * sqrt(termy1) - 0.5 * sqrt(termy2);
    return square;
}
fvector2_t square_to_circle(fvector2_t * square)
{
    assert(fabsf(square->x) <= 1.0 && fabsf(square->y)<=1.0);
    
    fvector2_t circle;
    circle.x = square->x * sqrt(1.0 - square->y*square->y/2.0);
    circle.y = square->y * sqrt(1.0 - square->x*square->x/2.0);
    return circle;
}
/* [] END OF FILE */
