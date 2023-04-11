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

// Joseph21 - a couple of convenience functions to safely compare floats using an error margin
// Note that prototypes for these functions are not needed, since they are not part of class Map

#define F_EPSILON   0.0001f    // error margin for comparing floats

bool floatEqual(    float a, float b ) { return abs(a - b) < F_EPSILON; }
bool floatGrtEqual( float a, float b ) { return  floatEqual( a, b ) || a > b; }
bool floatLssEqual( float a, float b ) { return  floatEqual( a, b ) || a < b; }
bool floatGreater(  float a, float b ) { return !floatEqual( a, b ) && a > b; }
bool floatLess(     float a, float b ) { return !floatEqual( a, b ) && a < b; }

bool Map::isInsideMap(float x, float y)
{
    // the boundaries of the map are considered to be part of the map
    return floatGrtEqual( x, 0.0f ) && floatLssEqual( x, MAP_NUM_COLS * TILE_SIZE ) &&
           floatGrtEqual( y, 0.0f ) && floatLssEqual( y, MAP_NUM_ROWS * TILE_SIZE );
}

bool Map::isOnMapBoundary(float x, float y)
{
    return floatEqual( x, 0.0f ) || floatEqual( x, MAP_NUM_COLS * TILE_SIZE ) ||
           floatEqual( y, 0.0f ) || floatEqual( y, MAP_NUM_ROWS * TILE_SIZE );
}

bool Map::isOutSideMap(float x, float y)
{
	return !isInsideMap( x, y );
}

void Map::renderMapGrid(olc::PixelGameEngine* PGEptr)
{
    // fill background for minimap
    PGEptr->FillRect( 0, 0, MAP_NUM_COLS * TILE_SIZE * MINIMAP_SCALE_FACTOR, MAP_NUM_ROWS * TILE_SIZE * MINIMAP_SCALE_FACTOR, olc::DARK_YELLOW );

    // draw each tile
	for (int i = 0; i < MAP_NUM_ROWS; i++) {
		for (int j = 0; j < MAP_NUM_COLS; j++) {
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
            // colour different for different heights
			olc::Pixel p;
			switch (heightmap[i][j]) {
			    case 0:  p = olc::VERY_DARK_GREEN; break;
			    case 1:  p = olc::WHITE;           break;
			    case 2:  p = olc::GREY;            break;
			    case 3:  p = olc::DARK_GREY;       break;
			    case 4:  p = olc::VERY_DARK_GREY;  break;
			    default: p = olc::BLUE;  break;
			}
            // render this thile
			PGEptr->FillRect(
				tileX * MINIMAP_SCALE_FACTOR     + 1,
				tileY * MINIMAP_SCALE_FACTOR     + 1,
				TILE_SIZE * MINIMAP_SCALE_FACTOR - 1,
				TILE_SIZE * MINIMAP_SCALE_FACTOR - 1,
				p
			);
		}
	}
}

int Map::getFromHeightMap( int x, int y )
{
    // Joseph21 - if coordinate is out of bounds, return 0, else return value of height map
    if (x < 0 || x >= MAP_NUM_COLS || y < 0 || y >= MAP_NUM_ROWS)
        return 0;
    else
        return heightmap[y][x];
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
