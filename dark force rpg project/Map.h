#ifndef MAP_H
#define MAP_H
#include "defs.h"
#include "MapEditor.h"
#include "olcPixelGameEngine.h"

class Map
{
public:
	Map() = default;
	bool mapHasWallAt(float x, float y);
	bool isInsideMap(float x, float y);
	bool isOutSideMap(float x, float y);

	void renderMapGrid(olc::PixelGameEngine* PGEptr);
	int getMapAt(int i, int j);
	int getFromHeightMap(int i, int j);
	int getTextureMap(int i, int j, int Height);

};



#endif // !MAP_H


