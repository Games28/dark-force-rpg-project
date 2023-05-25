#include "Saber.h"










bool Saber::initSaber(bool success)
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

	SsaberAnimation[0] = new olc::Sprite("leftangleswingSheet.png");
	SsaberAnimation[1] = new olc::Sprite("leftangleswingSheetglow.png");
	SsaberAnimation[2] = new olc::Sprite("rightangleswingSheet.png");
	SsaberAnimation[3] = new olc::Sprite("rightangleswingSheetglow.png");

	DsaberAnimation[0] = new olc::Decal(SsaberAnimation[0]);
	DsaberAnimation[1] = new olc::Decal(SsaberAnimation[1]);
	DsaberAnimation[2] = new olc::Decal(SsaberAnimation[2]);
	DsaberAnimation[3] = new olc::Decal(SsaberAnimation[3]);
	Anime[0].SetParams(0.125f,SsaberAnimation[0]->width,SsaberAnimation[0]->height,6,1,6,85,true,true);
	Anime[1].SetParams(0.125f, SsaberAnimation[0]->width, SsaberAnimation[0]->height, 6, 1, 6, 85, false, true);
 	return true;
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

void Saber::DrawSaberAnimation(olc::PixelGameEngine* gfx, bool & reset,int animation, int inner, int outer, float fElapsedTime)
{
	
	
	bool movement = true;
	bool invert = true;
	//AnimationData saberglow = Anime[0].GetInfo(fElapsedTime, Positon.x, reset);
	AnimationData saber = Anime[animation].GetInfo(fElapsedTime, xoffset, reset);
	olc::vf2d size = { 80.0f, 300.0f };
	gfx->SetDecalMode(olc::DecalMode::ADDITIVE);
	gfx->DrawPartialDecal({ Positon.x + xoffset, Positon.y }, size, DsaberAnimation[outer], saber.sourcePos, saber.sourceSize, p);
	gfx->SetDecalMode(olc::DecalMode::NORMAL);
	gfx->DrawPartialDecal({ Positon.x + xoffset, Positon.y }, size, DsaberAnimation[inner], saber.sourcePos, saber.sourceSize);
	
	


}
