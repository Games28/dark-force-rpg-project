#include "Powers.h"

Powers::Powers()
{
	

}

void Powers::RandomizedLightning(olc::PixelGameEngine* gfx, int x1, int y1, int x2, int y2, int depth, olc::Pixel p)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dist(-5,5);
	std::uniform_int_distribution<int> dist1(1, 4);
	int index = dist1(rng);
	int destX = dist(rng);
	int destY = dist(rng);
	olc::vi2d Dest{};

	auto RandAroundZero = [=]() {
		return ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
	};

	if (depth == 0)
	{
		
		gfx->DrawLine(x1 - 2, y1 - 2, x2 - 2, y2 - 2, olc::CYAN.ALPHA);
		gfx->DrawLine(x1 - 1, y1 - 1, x2 - 1, y2 - 1, p);
		gfx->DrawLine(x1, y1, x2, y2, p);
		gfx->DrawLine(x1 +1, y1 +1, x2 + 1, y2+1, olc::CYAN);

		//if (index == 1)
		//{
		//	gfx->DrawLine(x1, y1, x2 + destX, y2 + destY, p);
		//	//gfx->DrawLine(x2 + destX, y2 + destY, x2 + destX + destX, y2 + destY + destY, p);
		//	
		//
		//}
	}
	else if (depth > 0)
	{
		int nDepthX = x2 - x1;
		int nDepthY = y2 - y1;

		float fPercentage = 0.50f;
		float fMinimum = 2.50f;

		float fMarginX = std::max(fMinimum, fPercentage * (float)nDepthX);
		float fMarginY = std::max(fMinimum, fPercentage * (float)nDepthY);

		int x_mid = x1 + nDepthX / 2 + (int)(RandAroundZero() * fMarginX);
		int y_mid = y1 + nDepthY / 2 + (int)(RandAroundZero() * fMarginY);

		RandomizedLightning(gfx, x1, y1, x_mid, y_mid, depth - 1, p);
		RandomizedLightning(gfx, x_mid, y_mid, x2, y2, depth - 1, p);

	}
}

void Powers::drawLightning(olc::PixelGameEngine* gfx)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dist(1, 4);
	int index = dist(rng);
	int index2 = dist(rng);
	olc::vi2d source{0,0};
	

	if (gfx->GetKey(olc::L).bHeld)
	{
		
		
		//switch (index)
		//{
		//case 4:
		//	
		//	RandomizedLightning(gfx, 220, 310, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	break;
		//case 3:
		//	
		//	RandomizedLightning(gfx, 170, 290, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	break;
		//case 2:
		//	
		//	RandomizedLightning(gfx, 140, 300, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	break;
		//case 1:
		//	RandomizedLightning(gfx, 110, 320, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	break;
		//}
		switch (index)
		{
		case 1:
			source = { 110,320 };
			break;
		case 2:
			source = { 140,300 };
			break;

		case 3:
			source = { 170,290 };
			break;
		case 4:
			source = { 220,310 };
			break;
		}
		RandomizedLightning(gfx, source.x, source.y, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);

		switch (index2)
		{
		case 1:
			source = { 110,320 };
			break;
		case 2:
			source = { 140,300 };
			break;

		case 3:
			source = { 170,290 };
			break;
		case 4:
			source = { 220,310 };
			break;
		}
		RandomizedLightning(gfx, source.x, source.y, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);

		//switch (index)
		//{
		//case 4:
		//	RandomizedLightning(gfx, 110, 320, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	RandomizedLightning(gfx, 140, 300, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	RandomizedLightning(gfx, 170, 290, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	RandomizedLightning(gfx, 220, 310, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	break;
		//case 3:
		//	RandomizedLightning(gfx, 110, 320, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	RandomizedLightning(gfx, 140, 300, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	RandomizedLightning(gfx, 170, 290, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	break;
		//case 2:
		//	RandomizedLightning(gfx, 110, 320, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	RandomizedLightning(gfx, 140, 300, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	break;
		//case 1:
		//	RandomizedLightning(gfx, 110, 320, gfx->ScreenWidth() / 2, gfx->ScreenHeight() / 2, 5, olc::BLUE);
		//	break;
		//}
	}

}

void Powers::objectTargeting(olc::PixelGameEngine* gfx, Player& player, Sprite& sprite)
{
}
