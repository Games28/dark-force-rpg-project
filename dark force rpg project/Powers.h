#ifndef POWERS_H
#define POWERS_H
#include "olcPixelGameEngine.h"
#include "Player.h"
#include "Sprite.h"

class Powers
{
public:
	Powers() = default;
	void initsprite();
	void tkRotation(olc::PixelGameEngine& pge, Object& object, Player& player, float beforeR, float AfterR, int dir);
	void tkMove(Object& object, olc::vf2d before, olc::vf2d after);
	bool isinSight(Object& object, Player& player, float fov, float& angle2player);
	void reticle(olc::PixelGameEngine& pge);
	void reticleGlow(olc::PixelGameEngine& pge);

public:
	olc::Sprite* sprites[2];
	
	olc::vi2d size = { 45,45 };
	olc::vi2d pos = { 100,10 };
};

#endif // !POWERS_H



