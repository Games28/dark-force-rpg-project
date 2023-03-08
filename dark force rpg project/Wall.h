#ifndef WALL_H
#define WALL_H
#include "Player.h"
#include "olcPixelGameEngine.h"
#include "Raycast.h"
#include "defs.h"


class Wall
{
public:
	Wall() = default;
	void wallTextures();
	void changeColorIntensity(olc::Pixel& p, float factor);
	void calculateBottomAndTop(float wallDistance,int halfheight, int wallheight, int& wallceil, int& wallfloor, Player& player);
	void levelBottomAndTop(float wallDistance, int wallheight, int& wallceil, int& wallfloor);
	void renderWallProjection(olc::PixelGameEngine* PGEptr, Player& player, Raycast& rays);
	olc::Pixel wallrender(olc::PixelGameEngine* PGEptr,int x, int y,int hitindex,int texture,int WT,int WB, Raycast& rays);
public:
	olc::Sprite sprites[7];
	int text = 0;
	int time = 0;
};
#endif // !WALL_H


