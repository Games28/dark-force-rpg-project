#include "Saber.h"

void Saber::initSaber()
{
	sprites[0] = new olc::Sprite("layeredduallightsaberinner.png");
	sprites[1] = new olc::Sprite("layeredduallightsaberouter1.png");
	sprites[2] = new olc::Sprite("layeredduallightsaberouter2.png");
	sprites[3] = new olc::Sprite("layeredduallightsaberouter3.png");
	sprites[4] = new olc::Sprite("layeredduallightsaberouter4.png");
	//sprites[1] = new olc::Sprite("testsaber.png");
	decals[0] = new olc::Decal(sprites[0]);
	decals[1] = new olc::Decal(sprites[1]);
	decals[2] = new olc::Decal(sprites[2]);
	decals[3] = new olc::Decal(sprites[3]);
	decals[4] = new olc::Decal(sprites[4]);

	
}

void Saber::DrawSaber(olc::PixelGameEngine* gfx)
{
	int random = 0;


	if (light >= 1.0f)
	{
		lightswitch = true;
		light = 0.70f;
		random = rand() % 3;
	}
	else
	{
		lightswitch = false;

	}
	light += 0.5f;



	if (gfx->GetKey(olc::B).bPressed)
	{
		p = olc::DARK_CYAN;
	}
	if (gfx->GetKey(olc::R).bPressed)
	{
		p = olc::RED;
	}
	if (gfx->GetKey(olc::G).bPressed)
	{
		p = olc::GREEN;
	}
	if (gfx->GetKey(olc::P).bPressed)
	{
		p = olc::MAGENTA;
	}

	if (gfx->GetKey(olc::I).bHeld)
	{
		Pos.y -= 1;
	}
	if (gfx->GetKey(olc::K).bHeld)
	{
		Pos.y += 1;
	}
	if (gfx->GetKey(olc::J).bHeld)
	{
		Pos.x -= 1;
	}
	if (gfx->GetKey(olc::L).bHeld)
	{
		Pos.x += 1;
	}

	olc::vf2d size{ 0.40f,0.40f };
	gfx->SetDecalMode(olc::DecalMode::NORMAL);
	gfx->DrawDecal({ Pos.x,Pos.y }, decals[0], size, olc::WHITE);
	gfx->SetDecalMode(olc::DecalMode::ADDITIVE);

	switch (random)
	{
	case 0:
		gfx->DrawDecal({ Pos.x,Pos.y }, decals[1], size, p);
		break;
	case 1:
		gfx->DrawDecal({ Pos.x,Pos.y }, decals[2], size, p);
		break;
	case 2:
		gfx->DrawDecal({ Pos.x,Pos.y }, decals[3], size, p);
		break;
	case 3:
		gfx->DrawDecal({ Pos.x,Pos.y }, decals[4], size, p);
		break;
	}

}
