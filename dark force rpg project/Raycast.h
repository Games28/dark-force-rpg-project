#ifndef RAYCAST_H
#define RAYCAST_H
#include "Player.h"
#include "Map.h"
#include <vector>
#include "olcPixelGameEngine.h"

struct intersectInfo
{
	float wallHitX;
	float wallHitY;
	int mapX;
	int mapY;
	float distance;
	int height;
	bool wasHitVertical;
	float rayAngle;
	int texture;
	std::vector<int> textures;

	//multilevel flying and crouching
	int bottom_front;
	int ceil_front;
	int ceil_back;
};


struct Ray {
	std::vector<intersectInfo> listinfo;
	std::vector<intersectInfo> endlist;
};


class Raycast
{
public:
	Raycast() = default;
	void castAllRays(Player& player, Map& map);
	void castRay(float rayAngle, int stripID, Player& player, Map& map);
	void renderMapRays(olc::PixelGameEngine* PGEptr, Player& player);

public:
	Ray rays[NUM_RAYS];
};
#endif // !RAYCAST_H


