#ifndef SABER_H
#define SABER_H
#include "olcPixelGameEngine.h"

class Saber
{
public:
	Saber() = default;
	void initSaber();
	void DrawSaber(olc::PixelGameEngine* gfx);

public:
	olc::vf2d Pos = { -100.0f,-70.0f };
	olc::Sprite* sprites[5];
	olc::Decal* decals[5];
	float light = 1.0f;
	bool lightswitch = false;
	olc::Pixel p = olc::WHITE;
	olc::Pixel darkinner = olc::BLACK;
	olc::Pixel darkouter = olc::WHITE;
	olc::Pixel lightinner = olc::WHITE;
	olc::Pixel lightouter = olc::BLUE;
};


#endif // !SABER_H



