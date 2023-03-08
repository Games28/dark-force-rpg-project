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
	return (x >= 0 && x <= MAP_NUM_COLS * TILE_SIZE && y >= 0 && y <= MAP_NUM_ROWS * TILE_SIZE);
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
			
			olc::Pixel p = heightmap[i][j] != 0 ? olc::BLUE : olc::BLACK;


			PGEptr->FillRect(
				tileX * MINIMAP_SCALE_FACTOR,
				tileY * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
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
