#include "Sprite.h"

void Sprite::initSprites(float screenwidth)
{
	fDepthBuffer = new float[screenwidth];
	spritelamp = new olc::Sprite("trooper.png");

	listObjects = {
		{ 8.5f, 8.5f, 0.0f, 0.0f, false, spritelamp },
			{ 7.5f, 7.5f, 0.0f, 0.0f, false, spritelamp },
			{ 10.5f, 3.5f, 0.0f, 0.0f, false, spritelamp },
	};
}

void Sprite::DrawSprites(olc::PixelGameEngine* gfx,int x, float fdisttowall,Player& player, Map& map, float deltatime)
{
	fDepthBuffer[x] = fdisttowall;
	for (auto object : listObjects)
	{
		object.x += object.vx * deltatime;
		object.y += object.vy * deltatime;

		// Check if object is inside wall - set flag for removal
		if (map.sMap[(int)object.x * map.nMapX + (int)object.y] == '#')
			object.bRemove = true;

		float fVecX = object.x - player.fPlayerX;
		float fVecY = object.y - player.fPlayerY;
		float fDistanceFromPlayer = sqrtf(fVecX * fVecX + fVecY * fVecY);

		float fEyeX = sinf(player.fPlayerA_deg);
		float fEyeY = sinf(player.fPlayerA_deg);
		float fObjectAngle = atan2f(fEyeY, fEyeX) - atan2f(fEyeY, fEyeX);
		if (fObjectAngle < -3.14159f)
			fObjectAngle += 2.0f * 3.14159f;

		if (fObjectAngle > 3.14159f)
			fObjectAngle -= 2.0f * 3.14159f;

		bool bInPlayerFOV = fabs(fObjectAngle) < player.fPlayerFoV_deg / 2.0f;

		if (bInPlayerFOV && fDistanceFromPlayer >= 0.50f && fDistanceFromPlayer < fdisttowall)
		{
			float fObjectCeiling = (float)(gfx->ScreenHeight() / 2.0f) - gfx->ScreenHeight() / ((float)fDistanceFromPlayer);
			float fObjectFloor = gfx->ScreenHeight() - fObjectCeiling;
			float fObjectHeight = fObjectFloor - fObjectCeiling;
			float fObjectAspectRatio = (float)object.sprite->height / (float)object.sprite->width;
			float fObjectWidth = fObjectHeight / fObjectAspectRatio;
			float fMiddleOfObject = (0.5f * (fObjectAngle / (player.fPlayerFoV_deg / 2.0f)) + 0.50f) * (float)gfx->ScreenWidth();
			for (float lx = 0; lx < fObjectWidth; lx++)
			{
				for (float ly = 0; ly < fObjectHeight; ly++)
				{
					float fSampleX = lx / fObjectWidth;
					float fSampleY = ly / fObjectHeight;
					olc::Pixel p = object.sprite->Sample(fSampleX, fSampleY);
					int nObjectColumn = (int)(fMiddleOfObject + lx - (fObjectWidth / 2.0f));
					if (nObjectColumn >= 0 && nObjectColumn < gfx->ScreenWidth())
					{
						if (fDepthBuffer[nObjectColumn] >= fDistanceFromPlayer)
						{
							gfx->Draw(nObjectColumn, nObjectColumn + ly, p);
							fDepthBuffer[nObjectColumn] = fDistanceFromPlayer;
						}
					}
				}
			}
		}
		listObjects.remove_if([](Object& o) {return o.bRemove; });
	}
	
}

//void Object::Walk(const float fWalkSpeed)
//{
//	fSpeed = fWalkSpeed;
//	vel = olc::vf2d(std::cos(fHeading), std::sin(fHeading)) * fSpeed;
//}
//
//void Object::Strafe(const float fStrafeSpeed)
//{
//	fSpeed = fStrafeSpeed;
//	vel = olc::vf2d(std::cos(fHeading), std::sin(fHeading)).perp() * fSpeed;
//}
//
//void Object::Turn(const float fTurnSpeed)
//{
//	fHeading += fTurnSpeed;
//
//	// Wrap heading to sensible angle
//	if (fHeading < -3.14159f) fHeading += 2.0f * 3.14159f;
//	if (fHeading > 3.14159f) fHeading -= 2.0f * 3.14159f;
//}
//
//void Object::Stop()
//{
//	fSpeed = 0;
//	vel = { 0,0 };
//}
//
//void Sprite::Init()
//{
//
//}
//
//void Sprite::drawSprite(olc::PixelGameEngine* gfx,Player& player)
//{
//	auto DepthDraw = [&](int x, int y, float z, olc::Pixel p)
//	{
//		if (z <= pDepthBuffer[y * gfx->ScreenWidth() + x])
//		{
//			gfx->Draw(x, y, p);
//			pDepthBuffer[y * gfx->ScreenWidth() + x] = z;
//		}
//	};
//
//	for (const auto& ob : mapObjects)
//	{
//		const std::shared_ptr<Object> object = ob.second;
//
//		if (!object->bVisible) continue;
//
//		olc::vf2d vCameraPos = { player.fPlayerX, player.fPlayerY };
//		olc::vf2d vObject = object->pos - vCameraPos;
//
//		float fDistanceToObject = vObject.mag();
//
//		float fObjectAngle = atan2f(vObject.y, vObject.x) - player.fPlayerFoV_deg;
//		if (fObjectAngle < -3.14159f) fObjectAngle += 2.0f * 3.14159f;
//		if (fObjectAngle > 3.14159f) fObjectAngle -= 2.0f * 3.14159f;
//
//		
//
//	}
//
//}
