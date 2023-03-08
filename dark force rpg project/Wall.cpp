#include "Wall.h"

void Wall::wallTextures()
{
	sprites[0] = olc::Sprite("image/sand1.png");
	sprites[1] = olc::Sprite("image/stonewall.png");
	sprites[2] = olc::Sprite("image/Tatooinedoor1.png");
	sprites[3] = olc::Sprite("image/Tatooinedoor2.png");
	sprites[4] = olc::Sprite("image/Tatooinehousewall.png");
	sprites[5] = olc::Sprite("image/tatooinewindow1.png");
	sprites[6] = olc::Sprite("image/tatooinewindow2.png");
}

void Wall::changeColorIntensity(olc::Pixel& p, float factor)
{
}

void Wall::calculateBottomAndTop(float wallDistance, int halfheight, int wallheight, int& wallceil, int& wallfloor, Player &player)
{
	int nsliceHeight = ((TILE_SIZE / wallDistance) * Dist_PROJ_PLANE);
	wallceil = halfheight - (nsliceHeight * (1.0f - player.fPlayerH)) - (wallheight - 1) * nsliceHeight;

	wallfloor = halfheight + (nsliceHeight * player.fPlayerH);
}

void Wall::levelBottomAndTop(float wallDistance, int wallheight, int& wallceil, int& wallfloor)
{
	int nsliceHeight = ((TILE_SIZE / wallDistance) * Dist_PROJ_PLANE);
	wallceil = (WINDOW_HEIGHT / 2) - (nsliceHeight / 2.0f) - (wallheight - 1) * nsliceHeight;

	wallfloor = (WINDOW_HEIGHT / 2) + (nsliceHeight / 2.0f) - (wallheight - 1);
}

void Wall::renderWallProjection(olc::PixelGameEngine* PGEptr, Player& player, Raycast& rays)
{
	int halfscreenwidth = WINDOW_WIDTH / 2;
	int halfscreenheight = WINDOW_HEIGHT * player.fPlayerH + (int)player.lookupordown;
	float anglestep = 60 / float(WINDOW_WIDTH);
	
	bool stretched_texture = false;

	for (int x = 0; x < NUM_RAYS; x++) {



		float fViewangle = float(x - halfscreenwidth) * anglestep;
		float fcurangle = player.rotationAngle + fViewangle;


		int wallTopY, wallBottomY, nWallCeil, nWallCeil2, nWallFloor;
		float rawdistance, correctdistance;
		float fx_hit, fy_hit, rayAngle;
		int nx_hit, ny_hit, colheight, Texture;
		for (int i = 0; i < (int)rays.rays[x].listinfo.size(); i++)
		{
			rays.rays[x].listinfo[i].distance *= cos(fViewangle * PI / 180.0f);
			calculateBottomAndTop(rays.rays[x].listinfo[i].distance, halfscreenheight, rays.rays[x].listinfo[i].height, rays.rays[x].listinfo[i].ceil_front, rays.rays[x].listinfo[i].bottom_front, player);

		}
		for (int i = 0; i < (int)rays.rays[x].listinfo.size(); i++)
		{
			if (i == (int)rays.rays[x].listinfo.size() - 1)
			{
				rays.rays[x].listinfo[i].ceil_back = rays.rays[x].listinfo[i].ceil_front;
			}
			else
			{
				int nDummy;
				calculateBottomAndTop(rays.rays[x].listinfo[i + 1].distance, halfscreenheight, rays.rays[x].listinfo[i].height, rays.rays[x].listinfo[i].ceil_back, nDummy, player);

			}
		}


		fy_hit = rays.rays[x].listinfo[0].wallHitY;
		fx_hit = rays.rays[x].listinfo[0].wallHitX;
		nx_hit = rays.rays[x].listinfo[0].mapX;
		ny_hit = rays.rays[x].listinfo[0].mapY;
		rawdistance = rays.rays[x].listinfo[0].distance;
		colheight = rays.rays[x].listinfo[0].height;
		nWallCeil = rays.rays[x].listinfo[0].ceil_front;
		nWallCeil2 = rays.rays[x].listinfo[0].ceil_back;
		nWallFloor = rays.rays[x].listinfo[0].bottom_front;



		correctdistance = rawdistance * cos(fViewangle * PI / 180.0f);
		int wallheight = ((TILE_SIZE / correctdistance) * Dist_PROJ_PLANE);
		//calculateBottomAndTop(correctdistance,halfscreenheight, colheight, wallTopY, wallBottomY,player);



		float fPlayerH = TILE_SIZE / 2.0f;
		float fFoV = 60.0f;
		float fAngleStep = fFoV / (float)NUM_RAYS;
		float fViewAngle = (float)(x - (NUM_RAYS / 2)) * fAngleStep;
		float fCurAngle = (player.rotationAngle * 180.0f / PI) + fViewAngle;
		float fPlayerX = player.x;
		float fPlayerY = player.y;
		float fCosCurAngle = cos(fCurAngle * PI / 180.0f);
		float fSinCurAngle = sin(fCurAngle * PI / 180.0f);
		float fCosViewAngle = cos(fViewAngle * PI / 180.0f);

#define ROOF_COLOR olc::CYAN

		int hitindex = 0;

		for (int y = WINDOW_HEIGHT - 1; y >= 0; y--)
		{
#define UNKNOWN_DRAW 0
#define FLOOR_DRAW 1
#define WALL_DRAW 2
#define CEIL_DRAW 3
#define ROOF_DRAWING 4

			int drawmode = UNKNOWN_DRAW;
			if (y >= nWallFloor)
			{
				drawmode = (y <= halfscreenheight) ? CEIL_DRAW : FLOOR_DRAW;

			}
			else if (nWallFloor > y && y > nWallCeil)
			{
				drawmode = WALL_DRAW;

			}
			else if (nWallCeil >= y && y > nWallCeil2)
			{
				drawmode = (colheight == 0) ? FLOOR_DRAW : ROOF_DRAWING;
			}
			else
			{
				while (drawmode == UNKNOWN_DRAW)
				{
					if (hitindex < (int)rays.rays[x].listinfo.size() - 1)
					{
						// the y coord is above the current wall and roof slide, but there are still hit points to process
							// so there could be other walls behind current wall sticking out above it
						hitindex += 1;
						fy_hit = rays.rays[x].listinfo[hitindex].wallHitY;
						fx_hit = rays.rays[x].listinfo[hitindex].wallHitX;
						nx_hit = rays.rays[x].listinfo[hitindex].mapX;
						ny_hit = rays.rays[x].listinfo[hitindex].mapY;
						rawdistance = rays.rays[x].listinfo[hitindex].distance;
						colheight = rays.rays[x].listinfo[hitindex].height;
						nWallCeil = rays.rays[x].listinfo[hitindex].ceil_front;
						nWallCeil2 = rays.rays[x].listinfo[hitindex].ceil_back;
						nWallFloor = rays.rays[x].listinfo[hitindex].bottom_front;

						//correctdistance = rawdistance * cos(fViewangle * PI / 180.0f);
						//wallheight = ((TILE_SIZE / correctdistance) * Dist_PROJ_PLANE);
						//int cachewalltopy = wallTopY;
						//calculateBottomAndTop(correctdistance,halfscreenheight, colheight, wallTopY, wallBottomY,player);
						//if (wallTopY < cachewalltopy)
						//{ 
						//	drawmode = WALL_DRAW;
						//   
						//}
						if (y >= nWallFloor)
						{
							drawmode = (y <= halfscreenheight) ? CEIL_DRAW : FLOOR_DRAW;

						}
						else if (nWallFloor > y && y > nWallCeil)
						{
							drawmode = WALL_DRAW;
						}
						else if (nWallCeil >= y && y > nWallCeil2)
						{
							drawmode = ROOF_DRAWING;
						}


					}
					else
					{
						//drawmode = CEIL_DRAW;
						drawmode = (y <= halfscreenheight) ? CEIL_DRAW : FLOOR_DRAW;
					}
				}

			}


			switch (drawmode)
			{
			case CEIL_DRAW:
			{
				if (RENDER_CEILING)
				{
					float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(halfscreenheight - y)) * Dist_PROJ_PLANE) / fCosViewAngle;
					float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
					float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
					int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
					int nSampleY = (int)(fFloorProjY) % TILE_SIZE;

					olc::Pixel p = sprites[0].GetPixel(nSampleX, nSampleY);

					PGEptr->Draw(x, y, p);
					//drawPixel(x, y, 0xff777777);
					//drawPixel(x, y, texelColor);
				}
				break;
			}
			case FLOOR_DRAW:
			{
				float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(y - halfscreenheight)) * Dist_PROJ_PLANE) / fCosViewAngle;
				float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
				float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
				int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
				int nSampleY = (int)(fFloorProjY) % TILE_SIZE;


				olc::Pixel p = sprites[0].GetPixel(nSampleX, nSampleY);

				PGEptr->Draw(x, y, p);
				//drawPixel(x, y, 0xff777777);
				break;

			}
			case ROOF_DRAWING:
			{
				olc::Pixel p = ROOF_COLOR;
				PGEptr->Draw(x, y, p);

				break;
			}

			case WALL_DRAW:
			{
				float fSampleY = 0;
				int nDisplayBlockHeight = 0;


				if (STRETCHED_TEXTURING)
				{
					fSampleY = float(y - wallTopY) / float(wallBottomY - wallTopY);
				}
				else {
					//switch (colheight)
					//{
					//case 1:
					//{
					//	fSampleY = float(y - wallTopY) / float((wallBottomY - wallTopY));
					//	break;
					//}
					//case 2:
					//{
					//	float blocksize = float(wallBottomY - wallTopY) / colheight;
					//	while (relative > blocksize)
					//	{
					//		relative -= blocksize;
					//
					//	}
					//
					//	fSampleY = relative / blocksize;
					//
					//
					//	break;
					//}
					//case 3:
					//{
					//	blocksize = float(wallBottomY - wallTopY) / colheight;
					//
					//	while (relative > blocksize)
					//	{
					//		relative -= blocksize;
					//
					//	}
					//
					//
					//
					//	fSampleY = relative / blocksize;
					//	break;
					//}
					//case 4:
					//{
					//
					//	blocksize = float(wallBottomY - wallTopY) / colheight;
					//	while (relative > blocksize)
					//	{
					//		relative -= blocksize;
					//
					//	}
					//
					//	fSampleY = relative / blocksize;
					//	break;
					//
					//}
					//}
					float blocksize = float(nWallFloor - nWallCeil) / colheight;

					float relative = float(y - nWallCeil);
					nDisplayBlockHeight = colheight;
					while (relative > blocksize)
					{
						relative -= blocksize;
						nDisplayBlockHeight -= 1;
					}

					fSampleY = relative / blocksize;
				}
				float fSampleX;

				if (rays.rays[x].listinfo[hitindex].wasHitVertical) {
					fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitY % TILE_SIZE;

				}
				else
				{
					fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitX % TILE_SIZE;
				}

				//fSampleX = int(fSampleX) % TILE_SIZE;
				fSampleY = fSampleY * TILE_SIZE;
				// having both sample coordinates, get the sample and draw the pixel

				olc::Pixel auxSample = sprites[nDisplayBlockHeight].GetPixel(fSampleX, fSampleY);
				PGEptr->Draw(x, y, auxSample);



				//d
				//if (rays[x].listinfo[hitindex].wasHitVertical)
				//{
				//	changeColorIntensity(&texelColor, 0.7);
				//}
				//
				////drawPixel(x, y, texelColor);


				break;
			}
			}

		}

		PGEptr->DrawString(10, 50, "fLookUp  = " + std::to_string(player.lookupordown));
		PGEptr->DrawString(10, 40, "fPlayerH = " + std::to_string(player.fPlayerH));
	}

		
}

olc::Pixel Wall::wallrender(olc::PixelGameEngine* PGEptr, int x, int y, int hitindex, int texture,int WT,int WB, Raycast& rays)
{
	float fSampleY = float(y - WT) / float((WB - WT));


	float fSampleX;

	if (rays.rays[x].listinfo[hitindex].wasHitVertical) {
		fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitY % TILE_SIZE;

	}
	else
	{
		fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitX % TILE_SIZE;
	}

	//fSampleX = int(fSampleX) % TILE_SIZE;
	fSampleY = fSampleY * TILE_SIZE;

	return sprites[texture].GetPixel(fSampleX, fSampleY);
	
}
