#include "Saber.h"

void Saber::initSaber()
{
	sprites[0] = new olc::Sprite("dualdarksaberinnerglow.png");
	sprites[1] = new olc::Sprite("dualdarksaberouterglow.png");
	sprites[2] = new olc::Sprite("lightsabermiddleglow.png");
	//sprites[1] = new olc::Sprite("testsaber.png");
	decals[0] = new olc::Decal(sprites[0]);
	decals[1] = new olc::Decal(sprites[1]);
	decals[2] = new olc::Decal(sprites[2]);
}

void Saber::DrawSaber(olc::PixelGameEngine* gfx)
{
	if (light < 0.80f)
	{
		lightswitch = true;
	}


	if (light >= 1.0f)
	{
		lightswitch = false;
	}

	if (lightswitch)
	{
		light += 0.005f;
	}
	else
	{
		light -= 0.005f;
	}


	if (gfx->GetKey(olc::B).bPressed)
	{
		p = olc::BLUE;
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
	//gfx->SetDecalMode(olc::DecalMode::ADDITIVE);
	olc::vf2d size{ 0.45f,0.45f };
	//gfx->DrawDecal({ Pos.x,Pos.y }, decals[1], { 0.90f,0.90f }, p * light);
	//gfx->DrawDecal({ Pos.x,Pos.y }, decals[2], { 0.90f,0.90f }, p );
	gfx->SetDecalMode(olc::DecalMode::ADDITIVE);
	gfx->DrawDecal({ Pos.x,Pos.y }, decals[1], size, darkouter * light);
	gfx->SetDecalMode(olc::DecalMode::NORMAL);
	gfx->DrawDecal({ Pos.x,Pos.y }, decals[1], size, darkouter * light);
	gfx->DrawDecal({ Pos.x,Pos.y }, decals[0], size, darkinner);
}
