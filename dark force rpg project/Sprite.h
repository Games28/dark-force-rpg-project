#ifndef SPRITE_H
#define SPRITE_H

#include "olcPixelGameEngine.h"
#include "Player.h"

#include "Map.h"
#include "RC_Map.h"

#include <unordered_map>
#include <algorithm>

struct Object
{
	float x;
	float y;
	float vx;
	float vy;
	bool bRemove;
	float scale;
	float dist;
	float angle;
	olc::Sprite* sprite;
	bool pickedup;
	bool ishit;
};


class Sprite
{
public:
	Sprite() = default;
	void initSprites(int screenwidth, int screenheight);
	void DrawSprites(olc::PixelGameEngine* gfx, Player& player, Map& map, float deltatime, olc::Pixel maskColour = olc::MAGENTA);
	void DrawIcon(olc::PixelGameEngine* gfx, Player& player, Map& map, float deltatime, olc::Pixel maskColour = olc::MAGENTA);
	void Drawtest(olc::PixelGameEngine* gfx, Object& object, Player& player, RC_Map& map, float deltatime, olc::Pixel maskColour = olc::MAGENTA);
	void DrawDepth(olc::PixelGameEngine& pge, float fDepth, int x, int y, olc::Pixel col);
	//void newDraw();

public:
	float* fDepthBuffer = nullptr;
	std::vector<Object> listObjects;
    std::list<Object> listIcons;
	olc::Sprite* troopericon;
	olc::Sprite* spritetrooper;
	int nMapX = 32;
	int nMapY = 32;
	float fMaxDistance = sqrt(nMapX * nMapX + nMapY * nMapY);
	bool pickup = false;
	//std::unordered_map<uint32_t, std::shared_ptr<olc::rcw::Object>> mapObjects;


};
#endif // !SPRITE_H






