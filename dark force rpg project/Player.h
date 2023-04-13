#ifndef PLAYER_H
#define PLAYER_H
#include "olcPixelGameEngine.h"


struct Player
{

    float fPlayerX = 4.0f;
    float fPlayerY = 7.0f;
    float fPlayerA_deg = 0.0f;      // looking angle is in degrees

    // player: height of eye point and field of view
    float fPlayerH = 0.5f;
    float fPlayerFoV_deg = 60.0f;   // in degrees !!

    // factor for looking up or down - initially 0.0f (in pixel space: float is for smooth movement)
    float fLookUp = 0.0f;
};


#endif // !PLAYER_H


