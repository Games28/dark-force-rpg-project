#include "Sprite.h"

void Sprite::initSprites(int screenwidth, int screenheight)
{
	fDepthBuffer = new float[screenwidth * screenheight];
	for (int i = 0; i < screenwidth * screenheight; i++)
		fDepthBuffer[i] = fMaxDistance;
	spritetrooper = new olc::Sprite("trooper.png");
	troopericon = new olc::Sprite("troopericon.png");

	listObjects = {
		 
        
		

		//trooper icon
		{ 8.5f, 4.5f, 0.0f, 0.0f, false, 1.5,-1.0,0.0f, troopericon,false,false },
		{ 15.5f, 16.5f, 0.0f, 0.0f, false,2.0,-1.0,0.0f, troopericon ,false,false},
		{ 10.5f, 3.5f, 0.0f, 0.0f, false,1.0,-1.0,0.0f, troopericon, false, false },

		//trooper sprite
		{ 8.5f, 4.5f, 0.0f, 0.0f, false, 1.5,-1.0,0.0f, spritetrooper,false,false },

		{ 15.5f, 16.5f, 0.0f, 0.0f, false,2.0,-1.0,0.0f, spritetrooper ,false,false},

		{ 10.5f, 3.5f, 0.0f, 0.0f, false,1.0,-1.0,0.0f, spritetrooper, false, false },
	};

	listIcons = {
		{ 8.5f, 4.5f, 0.0f, 0.0f, false, 1.5,-1.0,0.0f, troopericon,false,false },
		{ 15.5f, 16.5f, 0.0f, 0.0f, false,2.0,-1.0,0.0f, troopericon ,false,false},
		{ 10.5f, 3.5f, 0.0f, 0.0f, false,1.0,-1.0,0.0f, troopericon, false, false },
	};
}

// Joseph21 - convenience convertor from degree to radian and vice versa
float deg_to_rad(float fAngleInDeg) { return fAngleInDeg / 180.0f * 3.1415926535f; }
float rad_to_deg(float fAngleInRad) { return fAngleInRad * 180.0f / 3.1415926535f; }

float fCumulatedTime = 0.0f;

void Sprite::DrawSprites(olc::PixelGameEngine* gfx,  Player& player, Map& map, float deltatime, olc::Pixel maskColour)
{

	int nHalfScreenWidth =gfx->ScreenWidth() / 2;
	int nHorizonHeight = gfx->ScreenHeight() * player.fPlayerH + (int)player.fLookUp;
	float fAngleStep = player.fPlayerFoV_deg / float(gfx->ScreenWidth());

	for (auto object : listObjects)
	{
		//object.x += object.vx * deltatime;
		//object.y += object.vy * deltatime;

		// Check if object is inside wall - set flag for removal
		if (map.sMap[(int)object.x * nMapX + (int)object.y] != '.') {
			object.bRemove = true;
		}
		else if (object.x < 0 || object.x >= nMapX || object.y < 0 || object.y >= nMapY)
		{
			object.bRemove = true;
		}
		else
		{

			float fVecX = object.x - player.fPlayerX;
			float fVecY = object.y - player.fPlayerY;
			float fDistanceFromPlayer = sqrtf(fVecX * fVecX + fVecY * fVecY);

			float fPlayerA_rad = deg_to_rad(player.fPlayerA_deg);
			float fEyeX = cosf(fPlayerA_rad);
			float fEyeY = sinf(fPlayerA_rad);
			float fObjectAngle = atan2f(fVecY, fVecX) - atan2f(fEyeY, fEyeX);
			if (fObjectAngle < -3.14159f)
				fObjectAngle += 2.0f * 3.14159f;

			if (fObjectAngle > 3.14159f)
				fObjectAngle -= 2.0f * 3.14159f;

			bool bInPlayerFOV = fabs(fObjectAngle) < deg_to_rad(player.fPlayerFoV_deg) / 2.0f;

			if (bInPlayerFOV && fDistanceFromPlayer >= 0.50f && fDistanceFromPlayer < fMaxDistance)
			{
				//scaling test
				float fCompensatePlayerHeight = player.fPlayerH - 0.5f;

				float fObjHiveSliceHeight = float(gfx->ScreenHeight() / fDistanceFromPlayer);
				float fObjHiveSliveHeightScld = float((gfx->ScreenHeight() * object.scale) / fDistanceFromPlayer);

				float fObjCeilingNormailized;
				float fObjCeilingScaled;
				float fObjectFloor;

				if (object.pickedup)
				{

					fObjCeilingNormailized = float(gfx->ScreenHeight() / 2) - fObjHiveSliceHeight;
					fObjCeilingScaled = float(gfx->ScreenHeight() / 2) - fObjHiveSliveHeightScld;
					fObjectFloor = float(gfx->ScreenHeight() / 2) + fObjHiveSliceHeight;

					
				}
				else {
					fObjCeilingNormailized = float(nHorizonHeight) - fObjHiveSliceHeight;
					fObjCeilingScaled = float(nHorizonHeight) - fObjHiveSliveHeightScld;
					fObjectFloor = float(nHorizonHeight) + fObjHiveSliceHeight;
				}

				float fScalingDifference = fObjCeilingNormailized - fObjCeilingScaled;
				float fObjectCeiling = fObjCeilingNormailized - 2 * fScalingDifference;
				
				float fObjectHeight = fObjectFloor - fObjectCeiling;
				float fObjectAspectRatio = float(object.sprite->height) / float(object.sprite->width);
				float fObjectWidth = fObjectHeight / fObjectAspectRatio;
				float fMiddleOfObject = (0.5f * (fObjectAngle / (deg_to_rad(player.fPlayerFoV_deg) / 2.0f)) + 0.50f) * (float)gfx->ScreenWidth();

				if (!object.pickedup)
				{
					fObjectCeiling += fCompensatePlayerHeight * fObjHiveSliceHeight * 2.0f;
					fObjectFloor += fCompensatePlayerHeight * fObjHiveSliceHeight * 2.0f;
				}


				for (float fx = 0.0f; fx < fObjectWidth; fx++) {
					// get distance across the screen to render
					int nObjColumn = int(fMiddleOfObject + fx - (fObjectWidth / 2.0f));
					// only render this column if it's on the screen
					if (nObjColumn >= 0 && nObjColumn < gfx->ScreenWidth()) {
						for (float fy = 0.0f; fy < fObjectHeight; fy++) {
							// calculate sample coordinates as a percentage of object width and height
							float fSampleX = fx / fObjectWidth;
							float fSampleY = fy / fObjectHeight;
							// sample the pixel and draw it
							olc::Pixel pSample = object.sprite->Sample(fSampleX, fSampleY);
							if (pSample != olc::MAGENTA && fDepthBuffer[nObjColumn] >= fDistanceFromPlayer) {
								gfx->Draw(nObjColumn, fObjectCeiling + fy, pSample);
								fDepthBuffer[nObjColumn] = fDistanceFromPlayer;
							}
						}
					}
				}
				
			}
		}
		//listObjects.remove_if([](Object& o) {return o.bRemove; });
	}
	
}

void Sprite::DrawIcon(olc::PixelGameEngine* gfx, Player& player, Map& map, float deltatime, olc::Pixel maskColour)
{
	int nHalfScreenWidth = gfx->ScreenWidth() / 2;
	int nHorizonHeight = gfx->ScreenHeight() * player.fPlayerH + (int)player.fLookUp;
	float fAngleStep = player.fPlayerFoV_deg / float(gfx->ScreenWidth());

	for (auto object : listIcons)
	{
		//object.x += object.vx * deltatime;
		//object.y += object.vy * deltatime;

		// Check if object is inside wall - set flag for removal
		if (map.sMap[(int)object.x * nMapX + (int)object.y] != '.') {
			object.bRemove = true;
		}
		else if (object.x < 0 || object.x >= nMapX || object.y < 0 || object.y >= nMapY)
		{
			object.bRemove = true;
		}
		else
		{

			float fVecX = object.x - player.fPlayerX;
			float fVecY = object.y - player.fPlayerY;
			float fDistanceFromPlayer = sqrtf(fVecX * fVecX + fVecY * fVecY);

			float fPlayerA_rad = deg_to_rad(player.fPlayerA_deg);
			float fEyeX = cosf(fPlayerA_rad);
			float fEyeY = sinf(fPlayerA_rad);
			float fObjectAngle = atan2f(fVecY, fVecX) - atan2f(fEyeY, fEyeX);
			if (fObjectAngle < -3.14159f)
				fObjectAngle += 2.0f * 3.14159f;

			if (fObjectAngle > 3.14159f)
				fObjectAngle -= 2.0f * 3.14159f;

			bool bInPlayerFOV = fabs(fObjectAngle) < deg_to_rad(player.fPlayerFoV_deg) / 2.0f;

			if (bInPlayerFOV && fDistanceFromPlayer >= 0.50f && fDistanceFromPlayer < fMaxDistance)
			{
				//scaling test
				float fCompensatePlayerHeight = player.fPlayerH - 0.5f;

				float fObjHiveSliceHeight = float(gfx->ScreenHeight() / fDistanceFromPlayer);
				float fObjHiveSliveHeightScld = float((gfx->ScreenHeight() * object.scale) / fDistanceFromPlayer);

				float fObjCeilingNormailized;
				float fObjCeilingScaled;
				float fObjectFloor;

				if (object.pickedup)
				{

					fObjCeilingNormailized = float(gfx->ScreenHeight() / 2) - fObjHiveSliceHeight;
					fObjCeilingScaled = float(gfx->ScreenHeight() / 2) - fObjHiveSliveHeightScld;
					fObjectFloor = float(gfx->ScreenHeight() / 2) + fObjHiveSliceHeight;


				}
				else {
					fObjCeilingNormailized = float(nHorizonHeight) - fObjHiveSliceHeight;
					fObjCeilingScaled = float(nHorizonHeight) - fObjHiveSliveHeightScld;
					fObjectFloor = float(nHorizonHeight) + fObjHiveSliceHeight;
				}

				float fScalingDifference = fObjCeilingNormailized - fObjCeilingScaled;
				float fObjectCeiling = fObjCeilingNormailized - 2 * fScalingDifference;

				float fObjectHeight = fObjectFloor - fObjectCeiling;
				float fObjectAspectRatio = float(object.sprite->height) / float(object.sprite->width);
				float fObjectWidth = fObjectHeight / fObjectAspectRatio;
				float fMiddleOfObject = (0.5f * (fObjectAngle / (deg_to_rad(player.fPlayerFoV_deg) / 2.0f)) + 0.50f) * (float)gfx->ScreenWidth();

				if (!object.pickedup)
				{
					fObjectCeiling += fCompensatePlayerHeight * fObjHiveSliceHeight * 2.0f;
					fObjectFloor += fCompensatePlayerHeight * fObjHiveSliceHeight * 2.0f;
				}


				for (float fx = 0.0f; fx < fObjectWidth; fx++) {
					// get distance across the screen to render
					int nObjColumn = int(fMiddleOfObject + fx - (fObjectWidth / 2.0f));
					// only render this column if it's on the screen
					if (nObjColumn >= 0 && nObjColumn < gfx->ScreenWidth()) {
						for (float fy = 0.0f; fy < fObjectHeight; fy++) {
							// calculate sample coordinates as a percentage of object width and height
							float fSampleX = fx / fObjectWidth;
							float fSampleY = fy / fObjectHeight;
							// sample the pixel and draw it
							olc::Pixel pSample = object.sprite->Sample(fSampleX, fSampleY);
							if (pSample != olc::MAGENTA && fDepthBuffer[nObjColumn] >= fDistanceFromPlayer) {
								gfx->Draw(nObjColumn, fObjectCeiling + fy, pSample);
								fDepthBuffer[nObjColumn] = fDistanceFromPlayer;
							}
						}
					}
				}

			}
		}
		listIcons.remove_if([](Object& o) {return o.bRemove; });
	}
}

void Sprite::Drawtest(olc::PixelGameEngine* gfx,Object& object, Player& player, RC_Map& map, float deltatime, olc::Pixel maskColour)
{
	int nHalfScreenWidth = gfx->ScreenWidth() / 2;
	int nHorizonHeight = gfx->ScreenHeight() * player.fPlayerH + (int)player.fLookUp;
	float fAngleStep = player.fPlayerFoV_deg / float(gfx->ScreenWidth());

	
		

		// Check if object is inside wall - set flag for removal
		
		if (object.x < 0 || object.x >= map.Width() || object.y < 0 || object.y >= map.Height())
		{
			object.bRemove = true;
		}
		else
		{

			float fVecX = object.x - player.fPlayerX;
			float fVecY = object.y - player.fPlayerY;
			float fDistanceFromPlayer = sqrtf(fVecX * fVecX + fVecY * fVecY);

			float fPlayerA_rad = deg_to_rad(player.fPlayerA_deg);
			float fEyeX = cosf(fPlayerA_rad);
			float fEyeY = sinf(fPlayerA_rad);
			float fObjectAngle = atan2f(fVecY, fVecX) - atan2f(fEyeY, fEyeX);
			if (fObjectAngle < -3.14159f)
				fObjectAngle += 2.0f * 3.14159f;

			if (fObjectAngle > 3.14159f)
				fObjectAngle -= 2.0f * 3.14159f;

			bool bInPlayerFOV = fabs(fObjectAngle) < deg_to_rad(player.fPlayerFoV_deg) / 2.0f;

			if (bInPlayerFOV && fDistanceFromPlayer >= 0.50f && fDistanceFromPlayer < fMaxDistance)
			{
				//scaling test
				float fCompensatePlayerHeight = player.fPlayerH - 0.5f;

				float fObjHiveSliceHeight = float(gfx->ScreenHeight() / fDistanceFromPlayer);
				float fObjHiveSliveHeightScld = float((gfx->ScreenHeight() * object.scale) / fDistanceFromPlayer);

				float fObjCeilingNormailized;
				float fObjCeilingScaled;
				float fObjectFloor;

				if (object.pickedup)
				{

					fObjCeilingNormailized = float(gfx->ScreenHeight() / 2) - fObjHiveSliceHeight;
					fObjCeilingScaled = float(gfx->ScreenHeight() / 2) - fObjHiveSliveHeightScld;
					fObjectFloor = float(gfx->ScreenHeight() / 2) + fObjHiveSliceHeight;


				}
				else {
					fObjCeilingNormailized = float(nHorizonHeight) - fObjHiveSliceHeight;
					fObjCeilingScaled = float(nHorizonHeight) - fObjHiveSliveHeightScld;
					fObjectFloor = float(nHorizonHeight) + fObjHiveSliceHeight;
				}

				float fScalingDifference = fObjCeilingNormailized - fObjCeilingScaled;
				float fObjectCeiling = fObjCeilingNormailized - 2 * fScalingDifference;

				float fObjectHeight = fObjectFloor - fObjectCeiling;
				float fObjectAspectRatio = float(object.sprite->height) / float(object.sprite->width);
				float fObjectWidth = fObjectHeight / fObjectAspectRatio;
				float fMiddleOfObject = (0.5f * (fObjectAngle / (deg_to_rad(player.fPlayerFoV_deg) / 2.0f)) + 0.50f) * (float)gfx->ScreenWidth();

				if (!object.pickedup)
				{
					fObjectCeiling += fCompensatePlayerHeight * fObjHiveSliceHeight * 2.0f;
					fObjectFloor += fCompensatePlayerHeight * fObjHiveSliceHeight * 2.0f;
				}


				for (float fx = 0.0f; fx < fObjectWidth; fx++) {
					// get distance across the screen to render
					int nObjColumn = int(fMiddleOfObject + fx - (fObjectWidth / 2.0f));
					// only render this column if it's on the screen
					if (nObjColumn >= 0 && nObjColumn < gfx->ScreenWidth()) {
						for (float fy = 0.0f; fy < fObjectHeight; fy++) {
							// calculate sample coordinates as a percentage of object width and height
							float fSampleX = fx / fObjectWidth;
							float fSampleY = fy / fObjectHeight;
							// sample the pixel and draw it
							olc::Pixel pSample = object.sprite->Sample(fSampleX, fSampleY);
							if (pSample != olc::MAGENTA) {
							    //gfx->Draw(nObjColumn, fObjectCeiling + fy, pSample);
								//fDepthBuffer[nObjColumn] = fDistanceFromPlayer;
								DrawDepth(*gfx, fDistanceFromPlayer, nObjColumn, fObjectCeiling + fy, pSample);
							}
						}
					}
				}

			}
		
		
	}
}

void Sprite::DrawDepth(olc::PixelGameEngine& pge,float fDepth, int x, int y, olc::Pixel col)
{
	if (x >= 0 && x < pge.ScreenWidth() &&
		y >= 0 && y < pge.ScreenHeight())
	{
		if (fDepth <= fDepthBuffer[y * pge.ScreenWidth() + x])
		{
			fDepthBuffer[y * pge.ScreenWidth() + x] = fDepth;
			pge.Draw(x, y, col);
		}
	}

}



