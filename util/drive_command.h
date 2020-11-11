/**
 * Created by Koen van Wel on 10/11/2020.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "vector.h"

#define DC_CIRCLE_DOMAIN 'c'
#define DC_SQUARE_DOMAIN 's'

typedef struct driveCommand {
    char domain; // 'c' denotes vector is on a circle 's' denotes it's on a square
    fvector2_t moveVector; // x is direction, y is forward
} driveCommand_t;

void driveCommandToCharArray(driveCommand_t * in, char* out){
    for(int i = 0; i<sizeof(driveCommand_t); i++)
        out[i] = ((unsigned char*) in)[i];
}

#ifdef __cplusplus
}
#endif