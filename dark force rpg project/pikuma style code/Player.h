#ifndef PLAYER_H
#define PLAYER_H
#include "Map.h"
#include "olcPixelGameEngine.h"
#include "defs.h"
#include "MapEditor.h"

class Player
{
public:
	Player();
	~Player();
	void processInput(olc::PixelGameEngine* PGEptr,float deltatime, Map& map);
	void movePlayer(float deltatime, Map& map);
	void renderMapPlayer(olc::PixelGameEngine* PGEptr);

public:
	float x;
	float y;
	float width;
	float height;
	int turnDirection; // -1 for left, +1 for right
	int walkDirection; // -1 for back, +1 for front
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
	int islookingupdown;
	float lookUpDown;
	float playerLookAngle;
	bool strafeLeft;
	bool strafeRight;
	int strafedirection;
	int run;
	int lookupordown;
	float lookspeed;
	float vertlook;
	float strafeupspeed;
	float fPlayerH;
	bool movevert;
	bool lookvert;
};

#endif // !PLAYER_H


