/**
 * Created by Koen van Wel on 10/11/2020.
 *
 * Header file defining util structs and functions and functions used in multiple different files
 * This file CAN depend on standard library files, and it MUST NOT depend on project files.
 */
#pragma once
#include <math.h>

typedef struct fvector2 {
    float x;
    float y;
} fvector2_t;

typedef struct vector2 {
    double x;
    double y;
} vector2_t;

fvector2_t fvector_rotate(fvector2_t *vector, float radians) {
    fvector2_t out;
    out.x = vector->x * cosf(radians) - vector->y * sinf(radians);
    out.y = vector->x * sinf(radians) + vector->y * cosf(radians);
    return out;
}

vector2_t vector_rotate(vector2_t *vector, double radians) {
    vector2_t out;
    out.x = vector->x * cos(radians) - vector->y * sin(radians);
    out.y = vector->x * sin(radians) + vector->y * cos(radians);
    return out;
}

float fvector_length(fvector2_t *vector){
    return vector->x*vector->x + vector->y*vector->y;
}

float vector_length(vector2_t *vector){
    return vector->x*vector->x + vector->y*vector->y;
}
