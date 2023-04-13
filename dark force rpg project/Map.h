#ifndef MAP_H
#define MAP_H

#include "olcPixelGameEngine.h"
struct Map
{
    std::string sMap;     // contains chars that define the type of block per map location
    float* fMap;     // contains floats that represent the height per block
    int nMapX = 32;
    int nMapY = 32;
};

#endif // !MAP_H


