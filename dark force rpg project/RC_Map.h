#ifndef RC_MAP_H
#define RC_MAP_H
#include "olcPixelGameEngine.h"
#include "defs.h"

class RC_Map
{
public:
	RC_Map() = default;
	
	void InitMap(int nSizeX, int nSizeY, const std::string& sUserMap);
	void InitMap(int nSizeX, int nSizeY);
	void AddLayer(const std::string& sUserMap);
	void FinalizeMap();
	int Width();
	int Height();
	float CellHeight(int x, int y);
	float CellHeightAt(int x, int y, int level);
	char CellValueAt(int x, int y, int level);
	float DiagonalLength();
	int NrOfLayers();
	bool IsInBounds(int x, int y);

private:
	std::vector<std::string> sMaps;
	std::vector<float*> fMaps;
	int nMapX;
	int nMapY;
	
	//maps
	std::string sMap_level0;
	std::string sMap_level1;
	std::string sMap_level2;
	std::string sMap_level3;
};

#endif // !RC_MAP>H


