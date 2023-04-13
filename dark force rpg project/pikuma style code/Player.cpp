#include "Player.h"

Player::Player()
{
	x = WINDOW_WIDTH / 2;
	y = WINDOW_HEIGHT / 2;
	width = 5;
	height = 5;
	turnDirection = 0;
	walkDirection = 0;
	rotationAngle = 0;
	walkSpeed = 100;
	turnSpeed = 45 * (PI/ 180.0f);
	islookingupdown = 0;
	lookUpDown = 45 * (PI / 180.0f);
	playerLookAngle = 0;
	strafeLeft = false;
	strafeRight = false;
	strafedirection = 0;
	run = 1;
	lookspeed = 200.0f;
	lookupordown = 0;
	vertlook = 0;
	strafeupspeed = 1.0f;
	fPlayerH = 0.5f;
	lookvert = false;
	movevert = false;
}

Player::~Player()
{
}

void Player::processInput(olc::PixelGameEngine* PGEptr, float deltatime, Map& map)
{

	if (PGEptr->GetKey(olc::W).bHeld)
	{
		walkDirection = +1;
	}

	if (PGEptr->GetKey(olc::S).bHeld)
	{
		walkDirection = -1;
	}

	if (PGEptr->GetKey(olc::Q).bHeld)
	{
		strafeLeft = true;
		strafedirection = +1;
	}

	if (PGEptr->GetKey(olc::E).bHeld)
	{
		strafeRight = true;
		strafedirection = -1;
	}

	if (PGEptr->GetKey(olc::D).bHeld)
	{
		turnDirection = +1;
	}

	if (PGEptr->GetKey(olc::A).bHeld)
	{
		turnDirection = -1;
	}

	if (PGEptr->GetKey(olc::SHIFT).bHeld)
	{
		run = 3;
	}

	if (PGEptr->GetKey(olc::UP).bHeld)
	{
		lookvert = true;
		lookupordown += lookspeed * deltatime;
	}
	if (PGEptr->GetKey(olc::DOWN).bHeld)
	{
		lookvert = true;
		lookupordown  -= lookspeed * deltatime;
	}

	if (PGEptr->GetKey(olc::W).bReleased)
	{
		walkDirection = 0;
	}

	if (PGEptr->GetKey(olc::S).bReleased)
	{
		walkDirection = 0;
	}

	if (PGEptr->GetKey(olc::Q).bReleased)
	{
		strafedirection = 0;
	}

	if (PGEptr->GetKey(olc::E).bReleased)
	{
		strafedirection = 0;
	}

	if (PGEptr->GetKey(olc::D).bReleased)
	{
		turnDirection = 0;
	}

	if (PGEptr->GetKey(olc::A).bReleased)
	{
		turnDirection = 0;
	}
	if (PGEptr->GetKey(olc::SHIFT).bReleased)
	{
		run = 1;
	}
	if (PGEptr->GetKey(olc::UP).bReleased)
	{
		lookvert = false;
		
	}
	if (PGEptr->GetKey(olc::DOWN).bReleased)
	{
		lookvert = false;
		
	}

	//NOTE - for multi level rendering there's only clamping to keep fPlayerH > 0.0f, there's no upper limit.

		// cache current height of horizon, so that you can compensate for changes in it via the look up value
	float fCachHorHeight = float(WINDOW_HEIGHT) * fPlayerH + lookupordown;

	if (MULTIPLE_LEVELS)
	{
		// if the player height is adapted, keep horizon height stable by compensating with look up value
		if (PGEptr->GetKey(olc::PGUP).bHeld)
		{
			movevert = true;
			fPlayerH += strafeupspeed * run * deltatime;
			lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
		}
		if (PGEptr->GetKey(olc::PGDN).bHeld)
		{
			movevert = true;
			float fNewHeight = fPlayerH - strafeupspeed * run * deltatime;
			if (fNewHeight > 0.0f && map.getFromHeightMap(int(x),int(y)) < fNewHeight)
			{
				fPlayerH = fNewHeight;
				lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
			}
		}

	}
	else
	{
		if (PGEptr->GetKey(olc::PGUP).bHeld)
		{
			movevert = true;
			float fNewHeight = fPlayerH + strafeupspeed * run * deltatime;
			if (fNewHeight < 1.0f)
			{
				fPlayerH += fNewHeight;
				lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
			}
		}
		if (PGEptr->GetKey(olc::PGDN).bHeld)
		{
			movevert = true;
			float fNewHeight = fPlayerH - strafeupspeed * run * deltatime;
			if (fNewHeight > 0.0f)
			{
				fPlayerH = fNewHeight;
				lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
			}
		}
	}

	if(PGEptr->GetKey(olc::PGUP).bReleased)
	{
		movevert = false;
		
	}
	if (PGEptr->GetKey(olc::PGDN).bReleased)
	{
		movevert = false;
		
	}


	// reset height and lookup factor upon pressing R
	if (PGEptr->GetKey(olc::R).bReleased) { fPlayerH = 0.5f; lookupordown = 0.0f; }
}

void Player::movePlayer(float deltatime, Map& map)
{
	auto normalizeAngle = [=](float* angle)
	{
		*angle = remainder(*angle, TWO_PI);
		if (*angle < 0) {
			*angle = TWO_PI + *angle;
		}
	};

	// move forward, backwards, turn right and turn left movment code
	rotationAngle += turnDirection * turnSpeed * deltatime;
	normalizeAngle(&rotationAngle);

	float moveStep = walkDirection * walkSpeed * deltatime;

	float newPlayerX = x + cos(rotationAngle) * moveStep * run;
	float newPlayerY = y + sin(rotationAngle) * moveStep * run;

	//wall collision
	if (!map.mapHasWallAt(newPlayerX, newPlayerY)) {
		x = newPlayerX;
		y = newPlayerY;
	}
	//strafe left and strafe right movement code
	float strafeStep = strafedirection * walkSpeed * deltatime;

	float strafePlayerX = 0;
	float strafePlayerY = 0;
	if (strafeLeft)
	{
		strafePlayerX = x - sin(rotationAngle) * strafeStep;
		strafePlayerY = y + cos(rotationAngle) * strafeStep;
	}
	if(strafeRight)
	{
		strafePlayerX = x + sin(rotationAngle) * strafeStep;
		strafePlayerY = y - cos(rotationAngle) * strafeStep;
	}

	if (!map.mapHasWallAt(strafePlayerX, strafePlayerY))
	{
		x = strafePlayerX;
		y = strafePlayerY;
	}

	//look up and look down cod
}

void Player::renderMapPlayer(olc::PixelGameEngine* PGEptr)
{
	olc::Pixel p = olc::CYAN;
	PGEptr->FillRect(
		x * MINIMAP_SCALE_FACTOR,
		y * MINIMAP_SCALE_FACTOR,
		width * MINIMAP_SCALE_FACTOR,
		height * MINIMAP_SCALE_FACTOR,
		p
	);
}
