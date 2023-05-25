#ifndef RAYCAST_H
#define RAYCAST_H


#include "olcPixelGameEngine.h"
#include"defs.h"
#include "Map.h"
#include "Player.h"
#include "WallSprite.h"
#include "RC_Map.h"
#include "Sprite.h"
#include "Trig.h"

// Joseph21 - changes made here

#define PI 3.1415926535f

// ==============================/  convenience functions for angles  /==============================

float deg2rad(float fAngleDeg);
float rad2deg(float fAngleRad);
float deg_mod2pi(float fAngleDeg);
float rad_mod2pi(float fAngleRad);

// ==============================/  lookup sine and cosine functions  /==============================

#define SIGNIFICANCE 3      // float angle is rounded at three decimal points
#define SIG_POW10    1000

extern float lu_sin_array[360 * SIG_POW10];
extern float lu_cos_array[360 * SIG_POW10];

void init_lu_sin_array();
void init_lu_cos_array();
float lu_sin(float fDegreeAngle);
float lu_cos(float fDegreeAngle);

// Joseph21 - end of hanges made here

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

    //new
    float fDistFrnt,     // distances to front and back faces of hit block
        fDistBack;

    int   nLevel = -1;   // nLevel == 0 --> ground level

    // these are on screen projected values (y coordinate in pixel space)
    int bot_front = -1;    // on screen projected bottom  of wall slice
    int bot_back = -1;    //                     bottom  of wall at back
    int top_front = -1;    //                     ceiling
    int top_back = -1;    //                     ceiling of wall at back
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

    //new
    bool GetDistancesToWallsPerLevel(int level, float fRayAngle, std::vector<IntersectInfo>& vHitList, RC_Map& map, Player& player);
    void CalculateWallBottomAndTop2(float fCorrectedDistToWall, int nHorHeight, int nLevelHeight, float fWallHeight, int& nWallTop, int& nWallBottom, Player& player);
    void raycast(olc::PixelGameEngine& pge, Player& player, RC_Map& map, Sprite& sprite);
public:
    float fDistToProjPlane;
    float fObjectIntensity = MULTIPLE_LEVELS ? OBJECT_INTENSITY : 0.2f;
    float fIntensityMultiplier = MULTIPLE_LEVELS ? MULTIPLIER_INTENSITY : 10.0f;
    int nMapX = 32;
    int nMapY = 32;
    float fMaxDistance;
    WallSprites wallsprite;
    //std::vector<olc::vf2d> vRayList;

public:

};



#endif // !RAYCAST_H


