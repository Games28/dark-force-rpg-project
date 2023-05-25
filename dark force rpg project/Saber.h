#ifndef SABER_H
#define SABER_H
#include "olcPixelGameEngine.h"
#include "Animation.h"

class Saber
{
public:
	Saber() = default;
	
	
	bool initSaber(bool success);
	void DrawSaber(olc::PixelGameEngine* gfx);
	void DrawSaberAnimation(olc::PixelGameEngine* gfx, bool& reset, int animation, int inner, int outer, float fElapsedTime);

public:
	

	olc::vf2d Pos = { -100.0f,-70.0f };
	olc::vf2d APos = { -100.0f,-60.0f };
	olc::vf2d Positon = {0.0f,110.0f };
	olc::Sprite* sprites[5];
	olc::Decal* decals[5];
	float light = 1.0f;
	float Aswitch = 1.0f;
	int SpriteChange = 5;
	float xoffset = 0;
	bool lightswitch = false;
	olc::Pixel p = olc::WHITE;
	olc::Pixel darkinner = olc::BLACK;
	olc::Pixel darkouter = olc::WHITE;
	olc::Pixel lightinner = olc::WHITE;
	olc::Pixel lightouter = olc::BLUE;
	
	//saber innter animation test
	olc::Sprite* SsaberinnerAnimationleft[7];
	olc::Decal* DsaberinnerAnimationleft[7];
	
	//saber outer animation
	olc::Sprite* SSaberouterAnimationleft[7];
	olc::Decal* DsaberouterAnimatiionleft[7];
	
	//new animation test
	SpriteAnimation Anime[2];
	olc::Sprite* SsaberAnimation[4];
	olc::Decal* DsaberAnimation[4];

};


#endif // !SABER_H



