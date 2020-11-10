/**
 * Created by Koen van Wel on 10/11/2020.
 */
#pragma once

#include "vector.h"
#define DC_CIRCLE_DOMAIN 'c'
#define DC_SQUARE_DOMAIN 's'

typedef struct drive_command {
    char domain; // 'c' denotes vector is on a circle 's' denotes it's on a square
    fvector2_t moveVector; // x is direction, y is forward
} drive_command_t;
