#include "RC_Map.h"



void RC_Map::InitMap(int nSizeX, int nSizeY, const std::string& sUserMap)
{
    if (nSizeX * nSizeY != (int)sUserMap.length()) {
        std::cout << "ERROR: InitMap() -->  mismatch between map dimensions and length of map string" << std::endl;
    }

    std::string sMap = sUserMap;
    nMapX = nSizeX;
    nMapY = nSizeY;
    // Initialise fMap as a 2d array of floats, having the same size as sMap, and containing the height per cell.
    // NOTE - if MULTIPLE_LEVELS is false, the fMap will contain no values > 1
    float* fMap = new float[nMapX * nMapY];
    for (int y = 0; y < nMapY; y++) {
        for (int x = 0; x < nMapX; x++) {
            switch (sMap[y * nMapX + x]) {
            case GRND_FLOOR: fMap[y * nMapX + x] = 0.0f; break;
            case FRST_FLOOR: fMap[y * nMapX + x] = 1.0f; break;
            case SCND_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 2.0f : 1.0f; break;
            case THRD_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 3.0f : 1.0f; break;
            case FRTH_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 4.0f : 1.0f; break;
            case FFTH_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 5.0f : 1.0f; break;
            case SXTH_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 6.0f : 1.0f; break;

            case FLOOR_1QRTR: fMap[y * nMapX + x] = 0.25f; break;
            case FLOOR_HALVE: fMap[y * nMapX + x] = 0.50f; break;
            case FLOOR_3QRTR: fMap[y * nMapX + x] = 0.75f; break;

            case         '1': fMap[y * nMapX + x] = 0.10f; break;
            case         '2': fMap[y * nMapX + x] = 0.20f; break;
            case         '3': fMap[y * nMapX + x] = 0.30f; break;
            case         '4': fMap[y * nMapX + x] = 0.40f; break;
            case         '5': fMap[y * nMapX + x] = 0.50f; break;
            case         '6': fMap[y * nMapX + x] = 0.60f; break;
            case         '7': fMap[y * nMapX + x] = 0.70f; break;
            case         '8': fMap[y * nMapX + x] = 0.80f; break;
            case         '9': fMap[y * nMapX + x] = 0.90f; break;
            }
        }
    }
    sMaps.push_back(sMap);
    fMaps.push_back(fMap);
}

void RC_Map::InitMap(int nSizeX, int nSizeY)
{
    nMapX = nSizeX;
    nMapY = nSizeY;
}

void RC_Map::AddLayer(const std::string& sUserMap)
{
    if (nMapX * nMapY != (int)sUserMap.length()) {
        std::cout << "ERROR: InitMap() -->  mismatch between map dimensions and length of map string" << std::endl;
    }

    std::string sMap = sUserMap;

    // Initialise fMap as a 2d array of floats, having the same size as sMap, and containing the height per cell.
    // NOTE - if MULTIPLE_LEVELS is false, the fMap will contain no values > 1
    float* fMap = new float[nMapX * nMapY];
    for (int y = 0; y < nMapY; y++) {
        for (int x = 0; x < nMapX; x++) {
            switch (sMap[y * nMapX + x]) {
            case GRND_FLOOR: fMap[y * nMapX + x] = 0.0f; break;
            case FRST_FLOOR: fMap[y * nMapX + x] = 1.0f; break;

            case FLOOR_1QRTR: fMap[y * nMapX + x] = 0.25f; break;
            case FLOOR_HALVE: fMap[y * nMapX + x] = 0.50f; break;
            case FLOOR_3QRTR: fMap[y * nMapX + x] = 0.75f; break;

            case         '1': fMap[y * nMapX + x] = 0.10f; break;
            case         '2': fMap[y * nMapX + x] = 0.20f; break;
            case         '3': fMap[y * nMapX + x] = 0.30f; break;
            case         '4': fMap[y * nMapX + x] = 0.40f; break;
            case         '5': fMap[y * nMapX + x] = 0.50f; break;
            case         '6': fMap[y * nMapX + x] = 0.60f; break;
            case         '7': fMap[y * nMapX + x] = 0.70f; break;
            case         '8': fMap[y * nMapX + x] = 0.80f; break;
            case         '9': fMap[y * nMapX + x] = 0.90f; break;

            default: std::cout << "ERROR: AddLayer() --> unknown sMap value: " << sMap[y * nMapX + x] << std::endl;
            }
        }
    }
    sMaps.push_back(sMap);
    fMaps.push_back(fMap);
}

void RC_Map::FinalizeMap()
{
    for (auto& elt : fMaps) {
        delete elt;
    }
    sMaps.clear();
    fMaps.clear();
}

int RC_Map::Width()
{
    return nMapX;
}

int RC_Map::Height()
{
    return nMapY;
}

float RC_Map::CellHeight(int x, int y)
{
    float result = -1.0f;
    if (x < 0 || x >= nMapX || y < 0 || y >= nMapY)
    {
        std::cout << "ERROR: CellHeight() -----> error in mapindices" << std::endl;
    }
    else
    {
        result = 0.0f;
        for (int i = 0; i < (int)fMaps.size(); i++)
        {
            result += fMaps[i][y * nMapX + x];
        }
    }
    return result;
}


float RC_Map::CellHeightAt(int x, int y, int level)
{
    float result = -1.0f;
    if (x < 0 || x >= nMapX || y < 0 || y >= nMapY) {
        std::cout << "ERROR: CellHeightAt() --> error in mapindices" << std::endl;
    }
    else if (level >= (int)fMaps.size()) {
        std::cout << "ERROR: CellHeightAt() --> level argument out of range" << std::endl;
    }
    else {
        result = fMaps[level][y * nMapX + x];
    }
    return result;
}

char RC_Map::CellValueAt(int x, int y, int level)
{
    char result = ' ';
    if (x < 0 || x >= nMapX || y < 0 || y >= nMapY) {
        std::cout << "ERROR: CellValueAt() --> error in mapindices" << std::endl;
    }
    else if (level >= (int)fMaps.size()) {
        std::cout << "ERROR: CellValueAt() --> level argument out of range" << std::endl;
    }
    else {
        result = sMaps[level][y * nMapX + x];
    }
    return result;
}

float RC_Map::DiagonalLength()
{
    return sqrt(nMapX * nMapX + nMapY * nMapY);
}

int RC_Map::NrOfLayers()
{
    return (int)fMaps.size();
}

bool RC_Map::IsInBounds(int x,int y)
{
    return (x >= 0 && x < nMapX&&
        y >= 0 && y < nMapY);
}


