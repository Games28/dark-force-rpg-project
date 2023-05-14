#include "Raycast.h"



bool Raycast::Init(bool& bSuccess, RC_Map& cmap)
{
    wallsprite.init(bSuccess);
    fMaxDistance = cmap.DiagonalLength();
    return bSuccess;
}

void Raycast::constrols(olc::PixelGameEngine& pge, float& fElapsedTime)
{
    if (pge.GetKey(olc::INS).bHeld) fObjectIntensity += INTENSITY_SPEED * fElapsedTime;
    if (pge.GetKey(olc::DEL).bHeld) fObjectIntensity -= INTENSITY_SPEED * fElapsedTime;
    if (pge.GetKey(olc::HOME).bHeld) fIntensityMultiplier += INTENSITY_SPEED * fElapsedTime;
    if (pge.GetKey(olc::END).bHeld) fIntensityMultiplier -= INTENSITY_SPEED * fElapsedTime;
}

bool Raycast::GetDistancesToWalls(float fRayAngle, std::vector<IntersectInfo>& vHitList,RC_Map& map, Player& player)
{
    // The player's position is the "from point"
    float fFromX = player.fPlayerX;
    float fFromY = player.fPlayerY;
    // Calculate the "to point" using the player's angle and fMaxDistance
    float fToX = player.fPlayerX + fMaxDistance * cos(fRayAngle * PI / 180.0f);
    float fToY = player.fPlayerY + fMaxDistance * sin(fRayAngle * PI / 180.0f);
    // work out normalized direction vector (fDX, fDY)
    float fDX = fToX - fFromX;
    float fDY = fToY - fFromY;
    float fRayLen = sqrt(fDX * fDX + fDY * fDY);
    fDX /= fRayLen;
    fDY /= fRayLen;

    // calculate the scaling factors for the ray increments per unit in x resp y direction
    // this calculation takes division by 0.0f into account
    float fSX = (fDX == 0.0f) ? FLT_MAX : sqrt(1.0f + (fDY / fDX) * (fDY / fDX));
    float fSY = (fDY == 0.0f) ? FLT_MAX : sqrt(1.0f + (fDX / fDY) * (fDX / fDY));

    // work out if line is going right or left resp. down or up
    int nGridStepX = (fDX > 0.0f) ? +1 : -1;
    int nGridStepY = (fDY > 0.0f) ? +1 : -1;

    float fLengthPartialRayX = 0.0f;
    float fLengthPartialRayY = 0.0f;

    int nCurX = int(fFromX);
    int nCurY = int(fFromY);

    // work out the first intersections with the grid
    if (nGridStepX < 0) { // ray is going left - get scaled difference between start point and left cell border
        fLengthPartialRayX = (fFromX - float(nCurX)) * fSX;
    }
    else {              // ray is going right - get scaled difference between right cell border and start point
        fLengthPartialRayX = (float(nCurX + 1.0f) - fFromX) * fSX;
    }
    if (nGridStepY < 0) { // ray is going up - get scaled difference between start point and top cell border
        fLengthPartialRayY = (fFromY - float(nCurY)) * fSY;
    }
    else {              // ray is going down - get scaled difference between bottom cell border and start point
        fLengthPartialRayY = (float(nCurY + 1.0f) - fFromY) * fSY;
    }

    // check whether analysis got out of map boundaries
    bool bOutOfBounds = !map.IsInBounds(nCurX, nCurY);
    // did analysis reach the destination cell?
    bool bDestCellFound = (nCurX == int(fToX) && nCurY == int(fToY));

    float fDistIfFound = 0.0f;  // accumulates distance of analysed piece of ray
    float fCurHeight = 0.0f;  // to check on differences in height

    while (!bOutOfBounds && !bDestCellFound && fDistIfFound < fMaxDistance) {

        // advance to next map cell, depending on length of partial ray's
        if (fLengthPartialRayX < fLengthPartialRayY) {
            // continue analysis in x direction
            nCurX += nGridStepX;
            fDistIfFound = fLengthPartialRayX;
            fLengthPartialRayX += fSX;
        }
        else {
            // continue analysis in y direction
            nCurY += nGridStepY;
            fDistIfFound = fLengthPartialRayY;
            fLengthPartialRayY += fSY;
        }

        bOutOfBounds = !map.IsInBounds(nCurX, nCurY);
        if (bOutOfBounds) {
            bDestCellFound = false;

            // If out of bounds, finalize the list with one additional intersection with the map boundary and height 0.
            // (only if the list is not empty!) This additional intersection record is necessary for proper rendering at map boundaries.
            if (fCurHeight != 0.0f && !vHitList.empty()) {

                fCurHeight = 0.0f;  // since we're out of bounds

                // put the collision info in a new IntersectInfo node and push it up the hit list
                IntersectInfo sInfo;
                sInfo.fDistance = fDistIfFound;
                sInfo.fHitX = fFromX + fDistIfFound * fDX;
                sInfo.fHitY = fFromY + fDistIfFound * fDY;
                sInfo.nMapCoordX = nCurX;
                sInfo.nMapCoordY = nCurY;
                sInfo.fHeight = fCurHeight;
                vHitList.push_back(sInfo);
            }
        }
        else {
            // check if there's a difference in height found
            bool bHitFound = (map.CellHeight(nCurX, nCurY) != fCurHeight);
            bDestCellFound = (nCurX == int(fToX) && nCurY == int(fToY));

            if (bHitFound) {

                // reset current height to new value
                fCurHeight = map.CellHeight(nCurX, nCurY);

                // put the collision info in a new IntersectInfo node and push it up the hit list
                IntersectInfo sInfo;
                sInfo.fDistance = fDistIfFound;
                sInfo.fHitX = fFromX + fDistIfFound * fDX;
                sInfo.fHitY = fFromY + fDistIfFound * fDY;
                sInfo.nMapCoordX = nCurX;
                sInfo.nMapCoordY = nCurY;
                sInfo.fHeight = fCurHeight;
                vHitList.push_back(sInfo);
            }
        }
    }

    return (vHitList.size() > 0);
}

void Raycast::CalculateWallBottomAndTop(float fCorrectedDistToWall, int nHorHeight, float fWallHeight, int& nWallTop, int& nWallBottom, Player& player, olc::PixelGameEngine& pge)
{
    int nSliceHeight = int((1.0f / fCorrectedDistToWall) * fDistToProjPlane);
    nWallTop = nHorHeight - (nSliceHeight * (1.0f - player.fPlayerH)) - (fWallHeight - 1) * nSliceHeight;
    nWallBottom = nHorHeight + (nSliceHeight * player.fPlayerH);
}

void Raycast::raycasting(olc::PixelGameEngine& pge, Player& player, RC_Map& map, Sprite& sprite)
{
   
    fDistToProjPlane = ((pge.ScreenWidth() / 2.0f) / sin((player.fPlayerFoV_deg / 2.0f) * PI / 180.0f)) * cos((player.fPlayerFoV_deg / 2.0f) * PI / 180.0f);

    int nHalfScreenWidth = pge.ScreenWidth() / 2;
    int nHorizonHeight = pge.ScreenHeight() * player.fPlayerH + (int)player.fLookUp;
    float fAngleStep = player.fPlayerFoV_deg / float(pge.ScreenWidth());

    // iterate over all screen slices, processing the screen in columns
    for (int x = 0; x < pge.ScreenWidth(); x++) {
        float fViewAngle = float(x - nHalfScreenWidth) * fAngleStep;
        float fCurAngle = player.fPlayerA_deg + fViewAngle;

        float fX_hit, fY_hit;   // to hold exact (float) hit location
        int   nX_hit, nY_hit;   // to hold coords of tile that was hit

        int nWallCeil, nWallCeil2, nWallFloor;   // to store the top and bottom y coord of the wall per column

        // this lambda returns a sample of the ceiling through the pixel at screen coord (px, py)
        auto get_ceil_sample = [=](int px, int py) -> olc::Pixel {
            // work out the distance to the location on the ceiling you are looking at through this pixel
            // (the pixel is given since you know the x and y screen coordinate to draw to)
            float fCeilProjDistance = (((1.0f - player.fPlayerH) / float(nHorizonHeight - py)) * fDistToProjPlane) / cos(fViewAngle * PI / 180.0f);
            // calculate the world ceiling coordinate from the player's position, the distance and the view angle + player angle
            float fCeilProjX = player.fPlayerX + fCeilProjDistance * cos(fCurAngle * PI / 180.0f);
            float fCeilProjY = player.fPlayerY + fCeilProjDistance * sin(fCurAngle * PI / 180.0f);
            // calculate the sample coordinates for that world ceiling coordinate, by subtracting the
            // integer part and only keeping the fractional part. Wrap around if the result < 0
            float fSampleX = fCeilProjX - int(fCeilProjX); if (fSampleX < 0.0f) fSampleX += 1.0f;
            float fSampleY = fCeilProjY - int(fCeilProjY); if (fSampleY < 0.0f) fSampleY += 1.0f;
            // having both sample coordinates, get the sample, shade and return it
            return ShadePixel(wallsprite.sprites[2]->Sample(fSampleX, fSampleY), fCeilProjDistance);
        };

        // this lambda returns a sample of the floor through the pixel at screen coord (px, py)
        auto get_floor_sample = [=](int px, int py) -> olc::Pixel {
            // work out the distance to the location on the floor you are looking at through this pixel
            // (the pixel is given since you know the x and y to draw to)
            float fFloorProjDistance = ((player.fPlayerH / float(py - nHorizonHeight)) * fDistToProjPlane) / cos(fViewAngle * PI / 180.0f);
            // calculate the world floor coordinate from the distance and the view angle + player angle
            float fFloorProjX = player.fPlayerX + fFloorProjDistance * cos(fCurAngle * PI / 180.0f);
            float fFloorProjY = player.fPlayerY + fFloorProjDistance * sin(fCurAngle * PI / 180.0f);
            // calculate the sample coordinates for that world floor coordinate, by subtracting the
            // integer part and only keeping the fractional part. Wrap around if the result < 0
            float fSampleX = fFloorProjX - int(fFloorProjX); if (fSampleX < 0.0f) fSampleX += 1.0f;
            float fSampleY = fFloorProjY - int(fFloorProjY); if (fSampleY < 0.0f) fSampleY += 1.0f;
            // having both sample coordinates, get the sample, shade and return it
            return ShadePixel(wallsprite.sprites[1]->Sample(fSampleX, fSampleY), fFloorProjDistance);
        };

        // this lambda returns a sample of the roof through the pixel at screen coord (px, py)
        auto get_roof_sample = [=](int px, int py, float fHeight) -> olc::Pixel {
            // work out the distance to the location on the roof you are looking at through this pixel
            // (the pixel is given since you know the x and y to draw to)
            float fRoofProjDistance = (((player.fPlayerH - fHeight) / float(py - nHorizonHeight)) * fDistToProjPlane) / cos(fViewAngle * PI / 180.0f);
            // calculate the world floor coordinate from the distance and the view angle + player angle
            float fRoofProjX = player.fPlayerX + fRoofProjDistance * cos(fCurAngle * PI / 180.0f);
            float fRoofProjY = player.fPlayerY + fRoofProjDistance * sin(fCurAngle * PI / 180.0f);
            // calculate the sample coordinates for that world floor coordinate, by subtracting the
            // integer part and only keeping the fractional part. Wrap around if the result < 0
            float fSampleX = fRoofProjX - int(fRoofProjX); if (fSampleX < 0.0f) fSampleX += 1.0f;
            float fSampleY = fRoofProjY - int(fRoofProjY); if (fSampleY < 0.0f) fSampleY += 1.0f;
            // having both sample coordinates, get the sample, shade and return it
            return ShadePixel(wallsprite.sprites[3]->Sample(fSampleX, fSampleY), fRoofProjDistance);
        };

        // prepare the rendering for this screen slice by calculating the list of intersections in this direction
        std::vector<IntersectInfo> vColHitlist;
        float fColHeight = 1.0f;
        float fCurDistance = 0.0f;     // distance var needed for wall shading
        if (GetDistancesToWalls(fCurAngle, vColHitlist,map,player)) {

            // at least one wall / block was hit. Extend the hit list with projected bottom / ceiling info
            for (int i = 0; i < (int)vColHitlist.size(); i++) {
                // make correction for the fish eye effect
                vColHitlist[i].fDistance *= cos(fViewAngle * PI / 180.0f);
                CalculateWallBottomAndTop(vColHitlist[i].fDistance, nHorizonHeight, vColHitlist[i].fHeight, vColHitlist[i].ceil_front, vColHitlist[i].bottom_front,player,pge);
            }
            // Extend the hit list with projected ceiling info for the back of the wall
            for (int i = 0; i < (int)vColHitlist.size(); i++) {
                if (i == (int)vColHitlist.size() - 1) {
                    vColHitlist[i].ceil_back = vColHitlist[i].ceil_front;
                }
                else {
                    int nDummy;
                    CalculateWallBottomAndTop(vColHitlist[i + 1].fDistance, nHorizonHeight, vColHitlist[i].fHeight, vColHitlist[i].ceil_back, nDummy,player,pge);
                }
            }

            // get the info from first hit point
            fX_hit = vColHitlist[0].fHitX;
            fY_hit = vColHitlist[0].fHitY;
            nX_hit = vColHitlist[0].nMapCoordX;
            nY_hit = vColHitlist[0].nMapCoordY;
            fColHeight = vColHitlist[0].fHeight;
            fCurDistance = vColHitlist[0].fDistance;

            nWallCeil = vColHitlist[0].ceil_front;
            nWallCeil2 = vColHitlist[0].ceil_back;
            nWallFloor = vColHitlist[0].bottom_front;

            //sprite.fDepthBuffer[x] = fCurDistance;

        }
        else {
            // no wall was hit - set bottom and top value for wall at the horizon
            nWallCeil = nHorizonHeight;
            nWallCeil2 = nWallCeil;
            nWallFloor = nHorizonHeight;
            fCurDistance = fMaxDistance;

            //sprite.fDepthBuffer[x] - fMaxDistance;
        }

        // now render this slice using the info of the hit list
        int nHitListIndex = 0;
        // note that we are working upwards

        //if (!vColHitlist.empty()) {
        //    olc::vf2d curHitPoint = { vColHitlist[0].fHitX, vColHitlist[0].fHitY };
        //    vRayList.push_back(curHitPoint);
        //}

        for (int y = 0; y < pge.ScreenHeight(); y++) {
            sprite.fDepthBuffer[y * pge.ScreenWidth() + x] = fMaxDistance;
        }
        for (int y = pge.ScreenHeight() - 1; y >= 0; y--) {

            // constants for different types of rendering
#define UNKNOWN_DRAWING 0
#define   FLOOR_DRAWING 1
#define    WALL_DRAWING 2
#define    CEIL_DRAWING 3
#define    ROOF_DRAWING 4

                // determine what type of segment is rendered: floor, wall, roof or ceiling
            int nDrawMode = UNKNOWN_DRAWING;
            if (y >= nWallFloor) {
                nDrawMode = (y <= nHorizonHeight) ? CEIL_DRAWING : FLOOR_DRAWING;
            }
            else if (nWallFloor > y && y > nWallCeil) {
                nDrawMode = WALL_DRAWING;
            }
            else if (nWallCeil >= y && y > nWallCeil2) {
                nDrawMode = (fColHeight == 0.0f) ? FLOOR_DRAWING : ROOF_DRAWING;
            }
            else {
                while (nDrawMode == UNKNOWN_DRAWING) {
                    if (nHitListIndex < (int)vColHitlist.size() - 1) {
                        // the y coord is above the current wall and roof slide, but there are still hit points to process
                        // so there could be other walls behind current wall sticking out above it
                        nHitListIndex += 1;

                        // get the info from next hit point
                        fX_hit = vColHitlist[nHitListIndex].fHitX;
                        fY_hit = vColHitlist[nHitListIndex].fHitY;
                        nX_hit = vColHitlist[nHitListIndex].nMapCoordX;
                        nY_hit = vColHitlist[nHitListIndex].nMapCoordY;
                        fColHeight = vColHitlist[nHitListIndex].fHeight;
                        fCurDistance = vColHitlist[nHitListIndex].fDistance;

                        nWallCeil = vColHitlist[nHitListIndex].ceil_front;
                        nWallCeil2 = vColHitlist[nHitListIndex].ceil_back;
                        nWallFloor = vColHitlist[nHitListIndex].bottom_front;

                        if (y >= nWallFloor) {
                            nDrawMode = (y <= nHorizonHeight) ? CEIL_DRAWING : FLOOR_DRAWING;
                        }
                        else if (nWallFloor > y && y > nWallCeil) {
                            nDrawMode = WALL_DRAWING;
                        }
                        else if (nWallCeil >= y && y > nWallCeil2) {
                            nDrawMode = ROOF_DRAWING;
                        }
                    }
                    else {
                        nDrawMode = (y <= nHorizonHeight) ? CEIL_DRAWING : FLOOR_DRAWING;
                    }
                }
            }

            // now we know what type of segment we're working on, render it
            switch (nDrawMode) {
            case CEIL_DRAWING: {                         // ========== render ceiling ====================
                if (RENDER_CEILING) {
                    olc::Pixel auxSample = get_ceil_sample(x, y);
                    //pge.Draw(x, y, auxSample);
                    sprite.DrawDepth(pge,fCurDistance,x, y, auxSample);
                }
            }
                             break;
            case FLOOR_DRAWING: {                        // ========== render floor   ====================
                olc::Pixel auxSample = get_floor_sample(x, y);
                //pge.Draw(x, y, auxSample);
                sprite.DrawDepth(pge, fCurDistance, x, y, auxSample);
            }
                              break;
            case ROOF_DRAWING: {                        // ========== render roof   ====================
                olc::Pixel auxSample = get_roof_sample(x, y, fColHeight);
                //pge.Draw(x, y, auxSample);
                sprite.DrawDepth(pge, fCurDistance, x, y, auxSample);
            }
                             break;
            case WALL_DRAWING: {                         // ========== render wall    ====================

                float fSampleY;
                if (STRETCHED_TEXTURING) {
                    // original sampling = stretched over full height of wall
                    // the y sample coordinate depends only on the pixel y coord on the screen
                    // in relation to the vertical space the wall is taking up
                    fSampleY = float(y - nWallCeil) / float(nWallFloor - nWallCeil);
                }
                else {
                    // sophisticated sampling = sampling per unit block size
                    float fBlockProjHeight = float(nWallFloor - nWallCeil) / fColHeight;
                    float fRelativeY = float(y - nWallCeil);
                    while (fRelativeY > fBlockProjHeight)
                        fRelativeY -= fBlockProjHeight;
                    fSampleY = fRelativeY / fBlockProjHeight;
                }

                // the x sample coordinate takes more work to figure out. You need to check what side of the
                // block was hit
                float fSampleX;
                float fBlockMidX = (float)nX_hit + 0.5f;   // location of middle of the cell that was hit
                float fBlockMidY = (float)nY_hit + 0.5f;
                // determine in what quadrant the hit location is wrt the block mid point
                float fTestAngle = atan2f((fY_hit - fBlockMidY), (fX_hit - fBlockMidX));
                if (-0.75f * PI <= fTestAngle && fTestAngle < -0.25f * PI) fSampleX = fX_hit - (float)nX_hit;  // south side
                if (-0.25f * PI <= fTestAngle && fTestAngle < 0.25f * PI) fSampleX = fY_hit - (float)nY_hit;  // east  side
                if (0.25f * PI <= fTestAngle && fTestAngle < 0.75f * PI) fSampleX = fX_hit - (float)nX_hit;  // north side
                if (-0.75f * PI > fTestAngle || fTestAngle >= 0.75f * PI) fSampleX = fY_hit - (float)nY_hit;  // west  side

                // having both sample coordinates, get the sample, shade it and draw the pixel
                olc::Pixel auxSample = wallsprite.sprites[0]->Sample(fSampleX, fSampleY);
                //pge.Draw(x, y, ShadePixel(auxSample, fCurDistance));
                sprite.DrawDepth(pge, fCurDistance, x, y, auxSample);
            }
                             break;
            }
        }

    }
}

olc::Pixel Raycast::ShadePixel(const olc::Pixel& p, float fDistance)
{
    if (RENDER_SHADED) {
        float fShadeFactor = std::max(SHADE_FACTOR_MIN, std::min(SHADE_FACTOR_MAX, fObjectIntensity * (fIntensityMultiplier / fDistance)));
        return p * fShadeFactor;
    }
    else
        return p;
}

olc::Pixel Raycast::ShadePixel_new(const olc::Pixel& org_pix, float fDistance, const olc::Pixel& shade_pix, float fIntensity)
{
    return PixelLerp(org_pix, shade_pix, std::min(1.0f, fIntensity * fDistance / fMaxDistance));
}


//new
bool Raycast::GetDistancesToWallsPerLevel(int level, float fRayAngle, std::vector<IntersectInfo>& vHitList, RC_Map& map, Player& player)
{
    int nHitPointsFound = 0;

    // The player's position is the "from point"
    float fFromX = player.fPlayerX;
    float fFromY = player.fPlayerY;
    // Calculate the "to point" using the player's angle and fMaxDistance
    float fToX = player.fPlayerX + fMaxDistance * lu_cos(fRayAngle);
    float fToY = player.fPlayerY + fMaxDistance * lu_sin(fRayAngle);
    // work out normalized direction vector (fDX, fDY)
    float fDX = fToX - fFromX;
    float fDY = fToY - fFromY;
    float fRayLen = sqrt(fDX * fDX + fDY * fDY);
    fDX /= fRayLen;
    fDY /= fRayLen;
    // calculate the scaling factors for the ray increments per unit in x resp y direction
    // this calculation takes division by 0.0f into account
    float fSX = (fDX == 0.0f) ? FLT_MAX : sqrt(1.0f + (fDY / fDX) * (fDY / fDX));
    float fSY = (fDY == 0.0f) ? FLT_MAX : sqrt(1.0f + (fDX / fDY) * (fDX / fDY));
    // work out if line is going right or left resp. down or up
    int nGridStepX = (fDX > 0.0f) ? +1 : -1;
    int nGridStepY = (fDY > 0.0f) ? +1 : -1;

    // init loop variables
    float fLengthPartialRayX = 0.0f;
    float fLengthPartialRayY = 0.0f;

    int nCurX = int(fFromX);
    int nCurY = int(fFromY);

    // work out the first intersections with the grid
    if (nGridStepX < 0) { // ray is going left - get scaled difference between start point and left cell border
        fLengthPartialRayX = (fFromX - float(nCurX)) * fSX;
    }
    else {              // ray is going right - get scaled difference between right cell border and start point
        fLengthPartialRayX = (float(nCurX + 1.0f) - fFromX) * fSX;
    }
    if (nGridStepY < 0) { // ray is going up - get scaled difference between start point and top cell border
        fLengthPartialRayY = (fFromY - float(nCurY)) * fSY;
    }
    else {              // ray is going down - get scaled difference between bottom cell border and start point
        fLengthPartialRayY = (float(nCurY + 1.0f) - fFromY) * fSY;
    }

    // check whether analysis got out of map boundaries
    bool bOutOfBounds = !map.IsInBounds(nCurX, nCurY);
    // did analysis reach the destination cell?
    bool bDestCellFound = (nCurX == int(fToX) && nCurY == int(fToY));

    float fDistIfFound = 0.0f;  // accumulates distance of analysed piece of ray
    float fCurHeight = 0.0f;  // to check on differences in height

    while (!bOutOfBounds && !bDestCellFound && fDistIfFound < fMaxDistance) {

        // advance to next map cell, depending on length of partial ray's
        if (fLengthPartialRayX < fLengthPartialRayY) {
            // continue analysis in x direction
            nCurX += nGridStepX;
            fDistIfFound = fLengthPartialRayX;
            fLengthPartialRayX += fSX;
        }
        else {
            // continue analysis in y direction
            nCurY += nGridStepY;
            fDistIfFound = fLengthPartialRayY;
            fLengthPartialRayY += fSY;
        }

        bOutOfBounds = !map.IsInBounds(nCurX, nCurY);
        if (bOutOfBounds) {
            bDestCellFound = false;

            // If out of bounds, finalize the list with one additional intersection with the map boundary and height 0.
            // (only if the list is not empty!) This additional intersection record is necessary for proper rendering at map boundaries.
            if (fCurHeight != 0.0f && nHitPointsFound > 0) {

                fCurHeight = 0.0f;  // since we're out of bounds
                // put the collision info in a new IntersectInfo node and push it up the hit list
                IntersectInfo sInfo;
                sInfo.fDistFrnt = fDistIfFound;
                sInfo.fHitX = fFromX + fDistIfFound * fDX;
                sInfo.fHitY = fFromY + fDistIfFound * fDY;
                sInfo.nMapCoordX = nCurX;
                sInfo.nMapCoordY = nCurY;
                sInfo.fHeight = fCurHeight;
                sInfo.nLevel = level;
                vHitList.push_back(sInfo);
            }
        }
        else {
            // check if there's a difference in height found
            bool bHitFound = (map.CellHeightAt(nCurX, nCurY, level) != fCurHeight);
            bDestCellFound = (nCurX == int(fToX) && nCurY == int(fToY));

            if (bHitFound) {
                nHitPointsFound += 1;

                // reset current height to new value
                fCurHeight = map.CellHeightAt(nCurX, nCurY, level);
                // put the collision info in a new IntersectInfo node and push it up the hit list
                IntersectInfo sInfo;
                sInfo.fDistFrnt = fDistIfFound;
                sInfo.fHitX = fFromX + fDistIfFound * fDX;
                sInfo.fHitY = fFromY + fDistIfFound * fDY;
                sInfo.nMapCoordX = nCurX;
                sInfo.nMapCoordY = nCurY;
                sInfo.fHeight = fCurHeight;
                sInfo.nLevel = level;
                vHitList.push_back(sInfo);
            }
        }
    }
    // return whether any hitpoints were found on this level
    return (nHitPointsFound > 0);
}

void Raycast::CalculateWallBottomAndTop2(float fCorrectedDistToWall, int nHorHeight, int nLevelHeight, float fWallHeight, int& nWallTop, int& nWallBottom, Player& player)
{
    // calculate slice height for a *unit height* wall
    int nSliceHeight = int((1.0f / fCorrectedDistToWall) * fDistToProjPlane);
    nWallTop = nHorHeight - (nSliceHeight * (1.0f - player.fPlayerH)) - (nLevelHeight + fWallHeight - 1.0f) * nSliceHeight;
    nWallBottom = nWallTop + nSliceHeight * fWallHeight;
}

void Raycast::raycast(olc::PixelGameEngine& pge, Player& player, RC_Map& map, Sprite& sprite)
{
    
    fDistToProjPlane = ((pge.ScreenWidth() / 2.0f) / lu_sin((player.fPlayerFoV_deg / 2.0f) * PI / 180.0f)) * lu_cos((player.fPlayerFoV_deg / 2.0f) * PI / 180.0f);
    int nHorizonHeight = pge.ScreenHeight() * player.fPlayerH + (int)player.fLookUp;
    float fAngleStep = player.fPlayerFoV_deg / float(pge.ScreenWidth());

    // iterate over all screen slices, processing the screen in columns
    for (int x = 0; x < pge.ScreenWidth(); x++) {
        float fViewAngle = float(x - (pge.ScreenWidth() / 2)) * fAngleStep;
        float fCurAngle = player.fPlayerA_deg + fViewAngle;

        float fX_hit, fY_hit;        // to hold exact (float) hit location (world space)
        int   nX_hit, nY_hit;        // to hold coords of tile that was hit (tile space)

        int   nWallTop, nWallTop2;   // to store the top and bottom y coord of the wall per column (screen space)
        int   nWallBot, nWallBot2;   // the ...2 variant represents the back of the current block

        // This lambda performs much of the sampling proces of horizontal surfaces. It can be used for floors, roofs and ceilings etc.
        // fProjDistance is the distance from the player to the hit point on the surface.
        auto generic_sampling = [=](float fProjDistance, olc::Sprite* cTexturePtr) -> olc::Pixel {
            // calculate the world coordinates from the distance and the view angle + player angle
            float fProjX = player.fPlayerX + fProjDistance * lu_cos(fCurAngle);
            float fProjY = player.fPlayerY + fProjDistance * lu_sin(fCurAngle);
            // calculate the sample coordinates for that world coordinate, by subtracting the
            // integer part and only keeping the fractional part. Wrap around if the result < 0 or > 1
            float fSampleX = fProjX - int(fProjX); if (fSampleX < 0.0f) fSampleX += 1.0f; if (fSampleX >= 1.0f) fSampleX -= 1.0f;
            float fSampleY = fProjY - int(fProjY); if (fSampleY < 0.0f) fSampleY += 1.0f; if (fSampleY >= 1.0f) fSampleY -= 1.0f;
            // having both sample coordinates, use the texture pointer to get the sample, and shade and return it
            return ShadePixel(cTexturePtr->Sample(fSampleX, fSampleY), fProjDistance);
        };

        // this lambda returns a sample of the floor through the pixel at screen coord (px, py)
        auto get_floor_sample = [=](int px, int py) -> olc::Pixel {
            // work out the distance to the location on the floor you are looking at through this pixel
            // (the pixel is given since you know the x and y to draw to)
            float fFloorProjDistance = ((player.fPlayerH / float(py - nHorizonHeight)) * fDistToProjPlane) / lu_cos(fViewAngle);
            // call the generic sampler to work out the rest
            return generic_sampling(fFloorProjDistance, wallsprite.sprites[0]);
        };

        // this lambda returns a sample of the roof through the pixel at screen coord (px, py)
        // NOTE: fHeightWithinLevel denotes the height of the hit point on the roof. This is typically the height of the block + its level
        auto get_roof_sample = [=](int px, int py, int nLevel, float fHeightWithinLevel) -> olc::Pixel {
            // work out the distance to the location on the roof you are looking at through this pixel
            // (the pixel is given since you know the x and y to draw to)
            float fRoofProjDistance = (((player.fPlayerH - (float(nLevel) + fHeightWithinLevel)) / float(py - nHorizonHeight)) * fDistToProjPlane) / lu_cos(fViewAngle);
            // call the generic sampler to work out the rest
            return generic_sampling(fRoofProjDistance, wallsprite.sprites[1]);
        };

        // this lambda returns a sample of the ceiling through the pixel at screen coord (px, py)
        // NOTE: fHeightWithinLevel denotes the height of the hit point on the ceiling. This is typically the level of the block, WITHOUT its height!
        auto get_ceil_sample = [=](int px, int py, int nLevel, float fHeightWithinLevel) -> olc::Pixel {
            // work out the distance to the location on the ceiling you are looking at through this pixel
            // (the pixel is given since you know the x and y screen coordinate to draw to)
            float fCeilProjDistance = (((float(nLevel) - player.fPlayerH) / float(nHorizonHeight - py)) * fDistToProjPlane) / lu_cos(fViewAngle);
            // call the generic sampler to work out the rest
            return generic_sampling(fCeilProjDistance, wallsprite.sprites[1]);
        };

        // prepare the rendering for this slice by calculating the list of intersections in this ray's direction
        // for each level, get the list of hitpoints in that level, work out front and back distances and projections
        // on screen, and add to the global vHitPointList
        std::vector<IntersectInfo> vHitPointList;
        for (int k = 0; k < map.NrOfLayers(); k++) {

            std::vector<IntersectInfo> vCurLevelList;
            GetDistancesToWallsPerLevel(k, fCurAngle, vCurLevelList,map, player);

            for (int i = 0; i < (int)vCurLevelList.size(); i++) {
                // make correction for the fish eye effect
                vCurLevelList[i].fDistFrnt *= lu_cos(fViewAngle);
                // calculate values for the on screen projections top_front and top_bottom
                CalculateWallBottomAndTop2(
                    vCurLevelList[i].fDistFrnt,
                    nHorizonHeight,
                    vCurLevelList[i].nLevel,
                    vCurLevelList[i].fHeight,
                    vCurLevelList[i].top_front,
                    vCurLevelList[i].bot_front,
                    player
                );
            }
            // Extend the hit list with projected ceiling info for the back of the wall
            for (int i = 0; i < (int)vCurLevelList.size(); i++) {
                if (i == (int)vCurLevelList.size() - 1) {
                    // last element, has no successor
                    vCurLevelList[i].fDistBack = vCurLevelList[i].fDistFrnt;
                    vCurLevelList[i].top_back = vCurLevelList[i].top_front;
                    vCurLevelList[i].bot_back = vCurLevelList[i].bot_front;
                }
                else {
                    // calculate values for the on screen projections top_front and top_bottom
                    vCurLevelList[i].fDistBack = vCurLevelList[i + 1].fDistFrnt;
                    CalculateWallBottomAndTop2(
                        vCurLevelList[i].fDistBack,
                        nHorizonHeight,
                        vCurLevelList[i].nLevel,
                        vCurLevelList[i].fHeight,
                        vCurLevelList[i].top_back,
                        vCurLevelList[i].bot_back,
                        player
                    );
                }
            }
            // add the hit points for this level list to the combined hit point list
            vHitPointList.insert(vHitPointList.end(), vCurLevelList.begin(), vCurLevelList.end());
        }

        // populate ray list for rendering mini map
        //if (!vHitPointList.empty()) {
        //    olc::vf2d curHitPoint = { vHitPointList[0].fHitX, vHitPointList[0].fHitY };
        //    vRayList.push_back(curHitPoint);
        //}

        // remove all hit points with height 0.0f - they were necessary for calculating the back face projection
        // of blocks, but that part is done now
        vHitPointList.erase(
            std::remove_if(
                vHitPointList.begin(),
                vHitPointList.end(),
                [](IntersectInfo& a) {
                    return a.fHeight == 0.0f;
                }
            ),
            vHitPointList.end()
                    );

        // sort hit points from far away to close by (painters algorithm)
        std::sort(
            vHitPointList.begin(),
            vHitPointList.end(),
            [](IntersectInfo& a, IntersectInfo& b) {
                return (a.fDistFrnt > b.fDistFrnt) ||
                (a.fDistFrnt == b.fDistFrnt && a.nLevel < b.nLevel);
            }
        );

        // start rendering this slice by putting sky and floor in it
        for (int y = pge.ScreenHeight() - 1; y >= 0; y--) {
            // reset depth buffer for this slice
            sprite.fDepthBuffer[y * pge.ScreenWidth() + x] = fMaxDistance;
            // draw floor and horizon
            if (y < nHorizonHeight) {
                olc::Pixel skySample = olc::CYAN;
                sprite.DrawDepth(pge,fMaxDistance, x, y, skySample);
            }
            else {
                olc::Pixel floorSample = get_floor_sample(x, y);   // shading is done in get_floor_sample()
                sprite.DrawDepth(pge,fMaxDistance, x, y, floorSample);
            }
        }

        // now render all hit points back to front
        for (auto& hitRec : vHitPointList) {

            float fBlockElevation = 1.0f;
            int   nBlockLevel = 0;
            float fFrntDistance = 0.0f;     // distance var also used for wall shading
            float fBackDistance = 0.0f;

            // For the distance calculations we needed also points where the height returns to 0.0f (the
            // back faces of the block). For the rendering we must skip these "hit points"
            if (hitRec.fHeight > 0.0f) {
                // load the info from next hit point
                fX_hit = hitRec.fHitX;
                fY_hit = hitRec.fHitY;
                nX_hit = hitRec.nMapCoordX;
                nY_hit = hitRec.nMapCoordY;
                fBlockElevation = hitRec.fHeight;
                nBlockLevel = hitRec.nLevel;
                fFrntDistance = hitRec.fDistFrnt;
                fBackDistance = hitRec.fDistBack;
                // make sure the screen y coordinate is within screen boundaries
                nWallTop = std::clamp(hitRec.top_front, 0, pge.ScreenHeight() - 1);
                nWallTop2 = std::clamp(hitRec.top_back, 0, pge.ScreenHeight() - 1);
                nWallBot = std::clamp(hitRec.bot_front, 0, pge.ScreenHeight() - 1);
                nWallBot2 = std::clamp(hitRec.bot_back, 0, pge.ScreenHeight() - 1);

                // render roof segment if it's visible
                for (int y = nWallTop2; y < nWallTop; y++) {
                    olc::Pixel roofSample = get_roof_sample(x, y, nBlockLevel, fBlockElevation);   // shading is done in get_roof_sample()
                    sprite.DrawDepth(pge,fBackDistance, x, y, roofSample);
                }
                // render wall segment - make sure that computational expensive calculations are only done once
                bool bSampleX_known = false;
                float fSampleX;
                for (int y = nWallTop; y <= nWallBot; y++) {

                    if (!bSampleX_known) {
                        // the x sample coordinate takes more work to figure out. You need to check what side of the
                        // block was hit
                        float fBlockMidX = (float)nX_hit + 0.5f;   // location of middle of the cell that was hit
                        float fBlockMidY = (float)nY_hit + 0.5f;
                        // determine in what quadrant the hit location is wrt the block mid point
                        float fTestAngle = atan2f((fY_hit - fBlockMidY), (fX_hit - fBlockMidX));

                        // I tested several supposedly faster approximations for atan2f(), but the results are really not significant
                        // The major bottleneck here is that this analysis is done for each separate pixel in the slice:
                        // for now I solved this by caching the drawmode and checking if was a wall previous
                        // (note that I have to temper with the cached drawmode when multiple wall segments are behind each other)

                        //   * possible improvement 1: determine the ranges within a slice so that you don't have to repeat the atan2f() call for each pixel
                        //   * possible improvement 2: (after 1) render these slice parts as scaled decals

                        if (-0.75f * PI <= fTestAngle && fTestAngle < -0.25f * PI) fSampleX = fX_hit - (float)nX_hit;  // south side
                        if (-0.25f * PI <= fTestAngle && fTestAngle < 0.25f * PI) fSampleX = fY_hit - (float)nY_hit;  // east  side
                        if (0.25f * PI <= fTestAngle && fTestAngle < 0.75f * PI) fSampleX = fX_hit - (float)nX_hit;  // north side
                        if (-0.75f * PI > fTestAngle || fTestAngle >= 0.75f * PI) fSampleX = fY_hit - (float)nY_hit;  // west  side

                        bSampleX_known = true;
                    }

                    float fSampleY;
                    // the y sample coordinate depends only on the pixel y coord on the screen
                    // in relation to the vertical space the wall is taking up
                    fSampleY = fBlockElevation * float(y - hitRec.top_front) / float(hitRec.bot_front - hitRec.top_front);
                    // having both sample coordinates, get the sample, shade it and draw the pixel
                    olc::Pixel wallSample = ShadePixel(wallsprite.sprites[1]->Sample(fSampleX, fSampleY), fFrntDistance);
                    sprite.DrawDepth(pge,fFrntDistance, x, y, wallSample);
                }
                // render ceiling segment if it's visible
                for (int y = nWallBot + 1; y <= nWallBot2; y++) {
                    olc::Pixel ceilSample = get_ceil_sample(x, y, nBlockLevel, fBlockElevation);   // shading is done in get_ceil_sample()
                    sprite.DrawDepth(pge,fBackDistance, x, y, ceilSample);
                }
            }
        }

        //if (bTestMode && x == nTestSlice) {
        //    PrintHitList(vHitPointList, true);
        //}
    }
    
}

