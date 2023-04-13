#ifndef SABER_H
#define SABER_H
#include "olcPixelGameEngine.h"


class Saber
{
public:
	Saber() = default;
	void initSaber();
	void DrawSaber(olc::PixelGameEngine* gfx, float deltatime);
	
public:
	olc::vf2d Pos = { -100.0f,0.0f };
	olc::Sprite* sprites[3];
	olc::Decal* decals[3];
	float light = 1.0f;
	bool lightswitch = false;
	olc::Pixel p = olc::WHITE;
};
#endif // !SABER_H



