#ifndef RAYCAST_H
#define RAYCAST_H


#include "olcPixelGameEngine.h"
#include"defs.h"
#include "Map.h"
#include "Player.h"
#include "WallSprite.h"
#include "RC_Map.h"
#include "Sprite.h"

struct IntersectInfo {
    float fHitX,
        fHitY;
    float fDistance;
    int   nMapCoordX,
        nMapCoordY;
    float fHeight;

    // Adaptation to support multilevel flying and crouching
    // these are y coordinate (projected) values
    int bottom_front;   // on screen projected bottom  of wall slice
    int   ceil_front;   //                     ceiling
    int   ceil_back;    //                     ceiling of wall at back
};



class Raycast
{
public:
    Raycast() = default;
    bool Init(bool& bSuccess, RC_Map& cmap);
    void constrols(olc::PixelGameEngine& pge, float& fElapsedTime);
    bool GetDistancesToWalls(float fRayAngle, std::vector<IntersectInfo>& vHitList, RC_Map& map, Player& player);
    void CalculateWallBottomAndTop(float fCorrectedDistToWall, int nHorHeight, float fWallHeight, int& nWallTop, int& nWallBottom, Player& player, olc::PixelGameEngine& pge);
    void raycasting(olc::PixelGameEngine& pge, Player& player, RC_Map& map, Sprite& sprite);
    olc::Pixel ShadePixel(const olc::Pixel& p, float fDistance);
    olc::Pixel ShadePixel_new(const olc::Pixel& org_pix, float fDistance, const olc::Pixel& shade_pix = SHADE_COLOUR, float fIntensity = 1.5f);

public:
    float fDistToProjPlane;
    float fObjectIntensity = MULTIPLE_LEVELS ? OBJECT_INTENSITY : 0.2f;
    float fIntensityMultiplier = MULTIPLE_LEVELS ? MULTIPLIER_INTENSITY : 10.0f;
    int nMapX = 32;
    int nMapY = 32;
    float fMaxDistance;
    WallSprites wallsprite;
    //std::vector<olc::vf2d> vRayList;
};



#endif // !RAYCAST_H


