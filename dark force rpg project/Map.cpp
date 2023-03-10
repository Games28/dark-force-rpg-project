#include "Map.h"

bool Map::mapHasWallAt(float x, float y)
{
	if (x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS * TILE_SIZE) {
		return true;
	}
	int mapGridIndexX = floor(x / TILE_SIZE);
	int mapGridIndexY = floor(y / TILE_SIZE);
	return heightmap[mapGridIndexY][mapGridIndexX] != 0;
}

bool Map::isInsideMap(float x, float y)
{
	return (x > 0 && x <= MAP_NUM_COLS * TILE_SIZE && y > 0 && y <= MAP_NUM_ROWS * TILE_SIZE);
}

bool Map::isOutSideMap(float x, float y)
{
	return (x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS * TILE_SIZE);
}

void Map::renderMapGrid(olc::PixelGameEngine* PGEptr)
{
	for (int i = 0; i < MAP_NUM_ROWS; i++) {
		for (int j = 0; j < MAP_NUM_COLS; j++) {
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			
			//olc::Pixel p = heightmap[i][j] != 0 ? olc::BLUE : olc::BLACK;


			olc::Pixel p;
			switch (heightmap[i][j]) {
			case 0:  p = olc::VERY_DARK_GREEN; break;
			case 1:  p = olc::WHITE;           break;
			case 2:  p = olc::GREY;            break;
			case 3:  p = olc::DARK_GREY;       break;
			case 4:  p = olc::VERY_DARK_GREY;  break;
			default: p = olc::BLUE;  break;
			}

			PGEptr->FillRect(
				tileX * MINIMAP_SCALE_FACTOR + 1,
				tileY * MINIMAP_SCALE_FACTOR + 1,
				TILE_SIZE * MINIMAP_SCALE_FACTOR - 1,
				TILE_SIZE * MINIMAP_SCALE_FACTOR - 1,
				p
			);

			

		}
	}
}

int Map::getMapAt(int i, int j)
{
	return heightmap[i][j];
}

int Map::getFromHeightMap(int i, int j)
{
	if (i < 0 || i >= MAP_NUM_ROWS || j < 0 || j >= MAP_NUM_COLS)
		return 0;
	else
		return heightmap[i][j];
}

int Map::getTextureMap(int i, int j, int Height)
{
	int textureid = 0;

	switch (Height)
	{
	case 1:
		textureid = texturemapOne[i][j];
		break;
	case 2:
		textureid = texturemapTwo[i][j];
		break;
	case 3:
		textureid = texturemapThree[i][j];
		break;
	case 4:
		textureid = texturemapFour[i][j];
		break;
	}
	return textureid;
}
