/*
 * Header file defining util structs and functions and functions used in multiple different files
 * This file CAN depend on standard library files, and it MUST NOT depend on project files.
 */
#pragma once
#include <math.h>

typedef struct fvector2 {
    float x;
    float y;
} fvector2_t;

fvector2_t fvector_rotate(fvector2_t *vector, float radians){
    fvector2_t out;
    out.x = vector->x * cosf(radians) - vector->y * sinf(radians);
    out.y = vector->x * sinf(radians) + vector->y * cosf(radians);
    return out;
}

float fvector_length(fvector2_t *vector){
    return vector->x*vector->x + vector->y*vector->y;
}

/* [] END OF FILE */
