#ifndef POWERS_H
#define POWERS_H
#include <random>
#include "olcPixelGameEngine.h"
#include "Sprite.h"
#include "Player.h"

class Powers
{
public:
	Powers();
	
	void RandomizedLightning(olc::PixelGameEngine* gfx, int x1, int y1, int x2, int y2, int depth, olc::Pixel p);
	void drawLightning(olc::PixelGameEngine* gfx);
	void objectTargeting(olc::PixelGameEngine* gfx, Player& player, Sprite& sprite);

public:
	int range = 0;
	int midchange = 30;
	int maxchange = 60;
	bool inSight = false;
	olc::Sprite sprites[4];
};
#endif // !POWERS_H


