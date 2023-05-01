#ifndef MAP_H
#define MAP_H

#include "olcPixelGameEngine.h"
#include "defs.h"
//#include "Player.h"
//#include "Raycast.h"


class Map
{
public:
    Map() {}
    void initMap();
    void RenderMapGrid(olc::PixelGameEngine* pge);
   // void RenderMapPlayer(olc::PixelGameEngine* pge, Player& player);
   // void RenderMapRays(olc::PixelGameEngine* pge, Player& player, Raycast& ray);

public:
    std::string sMap;     // contains chars that define the type of block per map location
    float* fMap;     // contains floats that represent the height per block
    int nMapX = 32;
    int nMapY = 32;
    float fMaxDistance = sqrt(nMapX * nMapX + nMapY * nMapY);

};
#endif // !MAP_H


