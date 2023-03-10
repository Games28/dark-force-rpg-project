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
			*angle = TWO_PI + *angle;
		}
	};

	auto distanceBetweenPoints = [=](float x1, float y1, float x2, float y2)
	{
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	};

	rays[stripID].listinfo.clear();
	rays[stripID].endlist.clear();
	
	normalizeAngle(&rayAngle);
	
	//turn in to functions bool below
	int isRayFacingDown = rayAngle > 0 && rayAngle < PI;
	int isRayFacingUp = !isRayFacingDown;

	int isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
	int isRayFacingLeft = !isRayFacingRight;

	float xintercept, yintercept;
	float xstep, ystep;

	///////////////////////////////////////////
	// HORIZONTAL RAY-GRID INTERSECTION CODE
	///////////////////////////////////////////
	bool foundHorzWallHit = false;
	float horzWallHitX = 0;
	float horzWallHitY = 0;
	int horzWallContent = 0;

	// Find the y-coordinate of the closest horizontal grid intersection
	yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown ? TILE_SIZE : 0;

	// Find the x-coordinate of the closest horizontal grid intersection

	xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

	// Calculate the increment xstep and ystep
	ystep = TILE_SIZE;
	ystep *= isRayFacingUp ? -1 : 1;

	xstep = TILE_SIZE / tan(rayAngle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float nextHorzTouchX = xintercept;
	float nextHorzTouchY = yintercept;
	if (nextHorzTouchY == 0)
	{
		int i = 0;
	}
	float horzxstep = xstep;
	float horzystep = ystep;
	int hitcount = 0;
	// Increment xstep and ystep until we find a wall
	while (map.isInsideMap(nextHorzTouchX, nextHorzTouchY))
	{
		float xToCheck = nextHorzTouchX;
		float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

		if (map.mapHasWallAt(xToCheck, yToCheck)) {
			// found a wall hit
			intersectInfo info;
			info.wallHitX = nextHorzTouchX;
			info.wallHitY = nextHorzTouchY;
			info.mapX = (int)floor(nextHorzTouchX / TILE_SIZE);
			info.mapY = (int)floor(nextHorzTouchY / TILE_SIZE);
			
			info.height = map.getFromHeightMap((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
			
			//code for all textures related to each level of a height thats more then 1 level
			for (int i = 1; i <= info.height; i++)
			{
				int texture = map.getTextureMap((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE), i);
				info.textures.push_back(texture);
			}
			info.texture = map.getTextureMap((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE), info.height);

			info.wasHitVertical = false;
			info.distance = distanceBetweenPoints(player.x, player.y, xToCheck, yToCheck);

			nextHorzTouchX += horzxstep;
			nextHorzTouchY += horzystep;
			rays[stripID].listinfo.push_back(info);
			hitcount++;
		}
		else {
			nextHorzTouchX += horzxstep;
			nextHorzTouchY += horzystep;
		}
	}

	//get hit info of out side map
	if (!map.isInsideMap(nextHorzTouchX, nextHorzTouchY))
	{
		intersectInfo info;
		info.wallHitX = nextHorzTouchX;
		info.wallHitY = nextHorzTouchY;
		info.distance = distanceBetweenPoints(player.x, player.y, nextHorzTouchX, nextHorzTouchY);
		info.height = 0;
		info.wasHitVertical = false;
		rays[stripID].endlist.push_back(info);
		//rays[stripID].listinfo.push_back(info);
		//hitcount++;
	
	}

	///////////////////////////////////////////
	// VERTICAL RAY-GRID INTERSECTION CODE
	///////////////////////////////////////////
	bool foundVertWallHit = false;
	float vertWallHitX = 0;
	float vertWallHitY = 0;
	int vertWallContent = 0;

	// Find the x-coordinate of the closest horizontal grid intersection
	xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;

	// Find the y-coordinate of the closest horizontal grid intersection
	float e = tan(rayAngle);
	float a = xintercept - player.x;
	float b = a * e;
	float c = player.y + b;
	yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

	// Calculate the increment xstep and ystep
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE * tan(rayAngle);
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

	float nextVertTouchX = xintercept;
	float nextVertTouchY = yintercept;
	float vertxstep = xstep;
	float vertystep = ystep;

	// Increment xstep and ystep until we find a wall
	while (map.isInsideMap(nextVertTouchX, nextVertTouchY)) {
		float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float yToCheck = nextVertTouchY;

		if (map.mapHasWallAt(xToCheck, yToCheck)) {
			// found a wall hit

			intersectInfo info;
			info.wallHitX = nextVertTouchX;
			info.wallHitY = nextVertTouchY;
			info.mapX = (int)floor(nextVertTouchX / TILE_SIZE);
			info.mapY = (int)floor(nextVertTouchY / TILE_SIZE);
			
			info.height = map.getFromHeightMap((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
			
			//code for all textures related to each level of a height thats more then 1 level
			for (int i = 1; i <= info.height; i++)
			{
				int texture = map.getTextureMap((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE), i);
				info.textures.push_back(texture);
			}
			info.texture = map.getTextureMap((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE), info.height);

			info.wasHitVertical = true;
			info.distance = distanceBetweenPoints(player.x, player.y, xToCheck, yToCheck);


			nextVertTouchX += vertxstep;
			nextVertTouchY += vertystep;
			rays[stripID].listinfo.push_back(info);
			hitcount++;
		}
		else {
			nextVertTouchX += vertxstep;
			nextVertTouchY += vertystep;
		}
	}

	if (!map.isInsideMap(nextVertTouchX, nextVertTouchY))
	{
		intersectInfo info;
		info.wallHitX = nextVertTouchX;
		info.wallHitY = nextVertTouchY;
		info.distance = distanceBetweenPoints(player.x, player.y, nextVertTouchX, nextVertTouchY);
		info.height = 0;
		info.wasHitVertical = true;
		rays[stripID].endlist.push_back(info);
		//rays[stripID].listinfo.push_back(info);
		//hitcount++;
	}


	for (int i = 0; i < hitcount - 1; i++)
	{
		for (int j = i + 1; j < hitcount; j++)
		{
			if (rays[stripID].listinfo[i].distance > rays[stripID].listinfo[j].distance)
			{
				intersectInfo info = rays[stripID].listinfo[i];
				rays[stripID].listinfo[i] = rays[stripID].listinfo[j];
				rays[stripID].listinfo[j] = info;

			}
		}
	}
	// multiple level fly and couch shortest outofbounds distnace compare
	if (rays[stripID].endlist[0].distance > rays[stripID].endlist[1].distance)
	{
		intersectInfo info = rays[stripID].endlist[1];
		rays[stripID].endlist[1] = rays[stripID].endlist[0];
		rays[stripID].endlist[0] = info;
		rays[stripID].listinfo.push_back(info);
	}
	else
	{
		intersectInfo info = rays[stripID].endlist[0];
		rays[stripID].listinfo.push_back(info);
	}
}

void Raycast::renderMapRays(olc::PixelGameEngine* PGEptr, Player& player)
{
	olc::Pixel p = olc::GREEN;
	for (int i = 0; i < NUM_RAYS; i++) {

		PGEptr->DrawLine(
			player.x * MINIMAP_SCALE_FACTOR,
			player.y * MINIMAP_SCALE_FACTOR,
			rays[i].listinfo[0].wallHitX * MINIMAP_SCALE_FACTOR,
			rays[i].listinfo[0].wallHitY * MINIMAP_SCALE_FACTOR,
			p
		);
	}
}
