#include "Raycast.h"

void Raycast::castAllRays(Player& player, Map& map)
{
	for (int col = 0; col < NUM_RAYS; col++) {
		float rayAngle = player.rotationAngle + (col - NUM_RAYS / 2) / (float)(NUM_RAYS)*FOV_ANGLE;
		castRay(rayAngle, col, player, map);
	}
}

void Raycast::castRay(float rayAngle, int stripID, Player& player, Map& map)
{
	auto normalizeAngle = [=](float* angle)
	{
		*angle = remainder(*angle, TWO_PI);
		if (*angle < 0) {
			*angle += TWO_PI;
		}
	};

	auto distanceBetweenPoints = [=](float x1, float y1, float x2, float y2)
	{
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	};

	// preparation for the DDA algo: clear the hit list for this ray, normalize the angle and set the ray direction booleans
	rays[stripID].listinfo.clear();

	normalizeAngle(&rayAngle);
	float fRayAngleTan = tan( rayAngle );

	//turn in to functions bool below
	bool isRayFacingDn = rayAngle > 0 && rayAngle < PI;
	bool isRayFacingUp = !isRayFacingDn;
	bool isRayFacingRt = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
	bool isRayFacingLt = !isRayFacingRt;

	// these variables will hold the current intersection point and the x and y step values
	float xintercept, yintercept;
	float xstep, ystep;

	///////////////////////////////////////////
	// HORIZONTAL RAY-GRID INTERSECTION CODE
	///////////////////////////////////////////

	// This part of the analysis focuses on the intersections with the horizontal grid lines.
	// This implies that the y-step (in var. ystep) is +/- TILE_SIZE, and the y coordinate (in yintercept)
	// will always be a multiple of TILE_SIZE

	// Find the y-coordinate of the closest horizontal grid intersection
	yintercept = int(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDn ? TILE_SIZE : 0;

	// Find the x-coordinate on the closest horizontal grid intersection
	xintercept = player.x + (yintercept - player.y) / fRayAngleTan;

	// temp fix - tan( angle ) can get very small so that xIntercept gets very big
	// NOTE: this blunt fix may cause glitches in the rendering
	if (xintercept < 0.0f) xintercept = 0.0f;
	if (xintercept > MAP_NUM_COLS * TILE_SIZE) xintercept = MAP_NUM_COLS * TILE_SIZE;

	// Calculate the increments xstep and ystep
	ystep = TILE_SIZE * (isRayFacingUp ? -1 : 1);

	xstep = TILE_SIZE / fRayAngleTan;
	xstep *= (isRayFacingLt && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRt && xstep < 0) ? -1 : 1;

	// start DDA loop - assumption is that the player is inside the map
	if (!map.isInsideMap( xintercept, yintercept)) {
        std::cout << "WARNING: Raycast::CastRay() --> initial location for HORIZONTAL grid line analysis is outside map boundaries..." << std::endl;
	}

	// Increment xstep and ystep until analysis gets out of bounds
	while (map.isInsideMap(xintercept, yintercept))
	{
		// work out grid (tile) coordinates to check the map
		int nXtoCheck = int(xintercept / TILE_SIZE);
		int nYtoCheck = int(yintercept / TILE_SIZE) + (isRayFacingUp ? -1 : 0);

        // determine the height of the next adjacent tile. If there's no next tile
        // because analysis arrived at boundary of the map, set height to 0
        int nextHeight;
        if (map.isOnMapBoundary( xintercept, yintercept )) {
            nextHeight = 0;
        } else {
            nextHeight = map.getFromHeightMap( nXtoCheck, nYtoCheck );
        }

        // just store each grid intersection point in the list - this brute force was necessary to debug the code
        intersectInfo hitInfo;
        hitInfo.wallHitX = xintercept;
        hitInfo.wallHitY = yintercept;
        hitInfo.mapX     = nXtoCheck;
        hitInfo.mapY     = nYtoCheck;
        hitInfo.height   = nextHeight;

        //code for all textures related to each level of a height thats more then 1 level
        for (int i = 1; i <= hitInfo.height; i++)
        {
            int texture = map.getTextureMap(nXtoCheck, nYtoCheck, i);
            hitInfo.textures.push_back(texture);
        }
        hitInfo.texture = map.getTextureMap(nXtoCheck, nYtoCheck, hitInfo.height);

        hitInfo.wasHitVertical = false;
        hitInfo.distance       = distanceBetweenPoints(player.x, player.y, xintercept, yintercept);

        // only needed for debugging
        hitInfo.rayUp = isRayFacingUp;
        hitInfo.rayDn = isRayFacingDn;
        hitInfo.rayLt = isRayFacingLt;
        hitInfo.rayRt = isRayFacingRt;

        rays[stripID].listinfo.push_back(hitInfo);

        // advance to next horizontal grid line
        xintercept += xstep;
        yintercept += ystep;
	}

	///////////////////////////////////////////
	// VERTICAL RAY-GRID INTERSECTION CODE
	///////////////////////////////////////////

	// This part of the analysis focuses on the intersections with the vertical grid lines.
	// This implies that the x-step (in var. xstep) is +/- TILE_SIZE, and the x coordinate (in xintercept)
	// will always be a multiple of TILE_SIZE

	// Find the x-coordinate of the closest vertical grid intersection
	xintercept = int(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRt ? TILE_SIZE : 0;

	// Find the y-coordinate on the closest vertical grid intersection
	yintercept = player.y + (xintercept - player.x) * fRayAngleTan;

	// temp fix - tan( angle ) can get very big so that yIntercept gets very big
	// NOTE: this blunt fix may cause glitches in the rendering
	if (yintercept < 0.0f) yintercept = 0.0f;
	if (yintercept > MAP_NUM_ROWS * TILE_SIZE) yintercept = MAP_NUM_ROWS * TILE_SIZE;

	// Calculate the increments xstep and ystep
	xstep = TILE_SIZE * (isRayFacingLt ? -1 : 1);

	ystep = TILE_SIZE * fRayAngleTan;
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDn && ystep < 0) ? -1 : 1;

	// start DDA loop - assumption is that the player is inside the map
	if (!map.isInsideMap( xintercept, yintercept)) {
        std::cout << "WARNING: Raycast::CastRay() --> initial location for VERTICAL grid line analysis is outside map boundaries..." << std::endl;
	}

	// Increment xstep and ystep until analysis gets out of bounds
	while (map.isInsideMap(xintercept, yintercept))
    {
		// work out grid (tile) coordinates to check the map
		int nXtoCheck = int(xintercept / TILE_SIZE) + (isRayFacingLt ? -1 : 0);
		int nYtoCheck = int(yintercept / TILE_SIZE);

        // determine the height of the next adjacent tile. If there's no next tile
        // because analysis arrived at boundary of the map, set height to 0
        int nextHeight;
        if (map.isOnMapBoundary( xintercept, yintercept )) {
            nextHeight = 0;
        } else {
            nextHeight = map.getFromHeightMap( nXtoCheck, nYtoCheck );
        }

        // just store each grid intersection point in the list - this brute force was necessary to debug the code
        intersectInfo hitInfo;
        hitInfo.wallHitX = xintercept;
        hitInfo.wallHitY = yintercept;
        hitInfo.mapX     = nXtoCheck;
        hitInfo.mapY     = nYtoCheck;

        hitInfo.height   = nextHeight;

        //code for all textures related to each level of a height thats more then 1 level
        for (int i = 1; i <= hitInfo.height; i++)
        {
            int texture = map.getTextureMap( nXtoCheck, nYtoCheck, i);
            hitInfo.textures.push_back(texture);
        }
        hitInfo.texture = map.getTextureMap( nXtoCheck, nYtoCheck, hitInfo.height);

        hitInfo.wasHitVertical = true;
        hitInfo.distance       = distanceBetweenPoints(player.x, player.y, xintercept, yintercept);

        // only needed for debugging
        hitInfo.rayUp = isRayFacingUp;
        hitInfo.rayDn = isRayFacingDn;
        hitInfo.rayLt = isRayFacingLt;
        hitInfo.rayRt = isRayFacingRt;

        rays[stripID].listinfo.push_back(hitInfo);

        // advance to next vertical grid line
        xintercept += xstep;
        yintercept += ystep;
	}

	// sort the hist list in order of increasing distance
    std::sort(
        rays[stripID].listinfo.begin(),
        rays[stripID].listinfo.end(),
        [] ( struct intersectInfo &a, struct intersectInfo &b ) {
            return a.distance < b.distance;
        }
    );

    bool bRunUp = true;
    int nHeightTracker = 0;
    std::vector<struct intersectInfo> tempList(rays[stripID].listinfo);   // copy hit list to a temporary list
    rays[stripID].listinfo.clear();                                         // clear hit list

    for (int i = 0; i < (int)tempList.size(); i++) {
        // this is to remove all unnecessary "hit" points with height 0 at the start of the list
        if (bRunUp) {
            if (tempList[i].height > 0) {
                bRunUp = false;
            }
        }
        // this is to remove all unnecessary in between hit points where the height doesn't change
        if (!bRunUp) {
            if (tempList[i].height != nHeightTracker) {
                nHeightTracker = tempList[i].height;
                // keep only hit points where the height differs from what it was before
                rays[stripID].listinfo.push_back(tempList[i]);
            }
        }
    }
}

void Raycast::renderMapRays(olc::PixelGameEngine* PGEptr, Player& player, int testRay)
{
	olc::Pixel p = olc::GREEN;
	for (int i = 0; i < NUM_RAYS; i++) {

        // get first hitpoint with elevation (height > 0)
        int useIndex = -1;
        for (int j = 0; j < (int)rays[i].listinfo.size() && (useIndex == -1); j++)
            if (rays[i].listinfo[j].height > 0)
                useIndex = j;
        // set to boundary hit if no point was found
        if (useIndex == -1) {
            useIndex = rays[i].listinfo.size() - 1;
        }
        if (useIndex >= 0) {
            PGEptr->DrawLine(
                player.x * MINIMAP_SCALE_FACTOR,
                player.y * MINIMAP_SCALE_FACTOR,
                rays[i].listinfo[useIndex].wallHitX * MINIMAP_SCALE_FACTOR,
                rays[i].listinfo[useIndex].wallHitY * MINIMAP_SCALE_FACTOR,
                p
            );
        }
	}
	// render test ray in distinct colour
	int tstIndex = rays[testRay].listinfo.size() - 1;
	if (tstIndex >= 0) {
        PGEptr->DrawLine(
            player.x * MINIMAP_SCALE_FACTOR,
            player.y * MINIMAP_SCALE_FACTOR,
            rays[testRay].listinfo[tstIndex].wallHitX * MINIMAP_SCALE_FACTOR,
            rays[testRay].listinfo[tstIndex].wallHitY * MINIMAP_SCALE_FACTOR,
            olc::MAGENTA
        );
	}
}
