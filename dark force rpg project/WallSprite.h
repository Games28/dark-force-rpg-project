#ifndef WALLSPRITE_H
#define WALLSPRITE_H

#include "olcPixelGameEngine.h"
#include "defs.h"

class WallSprites
{
public:
	WallSprites() {}
	bool init(bool& bSuccess);

public:

	olc::Sprite* sprites[5];
};


#endif // !WALLSPRITE_H



