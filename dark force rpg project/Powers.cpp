#include "Powers.h"

void Powers::initsprite()
{
	sprites[0] = new olc::Sprite("newicon.png");
	sprites[1] = new olc::Sprite("newiconglow.png");
}

void Powers::tkRotation(olc::PixelGameEngine& pge,Object& object, Player& player, float beforeR, float AfterR, int dir)
{
	auto Deg2Rad = [=](float angle) { return angle / 180.0f * 3.14159f; };
	
	float difference_x = object.x - player.fPlayerX;
	float difference_y = object.y - player.fPlayerY;

	float distance = sqrtf(difference_x * difference_x + difference_y * difference_y);

	float angle_player_to_object = atan2f(difference_y, difference_x);

	  
	float angle_difference = AfterR - beforeR;
	                    
	float angledifrad = Deg2Rad(angle_difference);
	

	object.x += distance * (cosf(angle_player_to_object + angledifrad) - cosf(angle_player_to_object));
	object.y += distance * (sinf(angle_player_to_object + angledifrad) - sinf(angle_player_to_object));

	
}

void Powers::tkMove(Object& object, olc::vf2d before, olc::vf2d after)
{
	float differencex = after.x - before.x;
	float differencey = after.y - before.y;

	object.x += differencex;
	object.y += differencey;
}

bool Powers::isinSight(Object& object, Player& player, float fov, float& angle2player)
{
	auto Deg2Rad = [=](float angle) { return angle / 180.0f * 3.14159f; };
	auto ModuloTwoPI = [=](float angle)
	{
		float a = angle;
		while (a < 0) a += 2.0f * 3.14159f;
		while (a >= 2.0f * 3.14159f) a -= 2.0f * 3.14159f;
		return a;
	};

	float tempx = object.x - player.fPlayerX;
	float tempy = object.y - player.fPlayerY;

	angle2player = ModuloTwoPI(atan2(tempx, tempy));
	float fAligneda = (2.0f * 3.14159f - Deg2Rad(player.fPlayerA_deg)) - 0.5f * 3.14159f;

	return abs(ModuloTwoPI(fAligneda + 3.14159f) - angle2player) < fov;
}

void Powers::reticle(olc::PixelGameEngine& pge)
{
	
	
		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y; y++)
			{
				int tempx = pos.x + x;
				int tempy = pos.y + y;

				olc::Pixel p;


				p = sprites[0]->GetPixel(x, y);
				

				pge.Draw(tempx, tempy, p);
			}
		}
	
}

void Powers::reticleGlow(olc::PixelGameEngine& pge)
{
	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			int tempx = pos.x + x;
			int tempy = pos.y + y;

			olc::Pixel p;



			p = sprites[1]->GetPixel(x, y);

			if (p != olc::MAGENTA)
			{

				pge.Draw(tempx, tempy, p);
			}
		}
	}
}
