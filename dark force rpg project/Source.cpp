#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "defs.h"
#include "Map.h"
#include "Player.h"
#include "Raycast.h"
#include "Wall.h"


class darkforce : public olc::PixelGameEngine
{
public:
	darkforce()
	{
		sAppName = "dark force raycaster";
	}
public:
	Map map;
	Player player;
	Raycast ray;
	Wall wall;

public:

	bool OnUserCreate() override
	{
		wall.wallTextures();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		player.processInput(this,fElapsedTime,map);
		player.movePlayer(fElapsedTime, map);
		Clear(RENDER_CEILING ? olc::BLACK : olc::BLUE);
		ray.castAllRays(player, map);
		
		wall.renderWallProjection(this, player, ray);
		map.renderMapGrid(this);
		player.renderMapPlayer(this);
		
		ray.renderMapRays(this, player);
		return true;
	}

	
};

int main()
{
	darkforce game;
	if (game.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 2, 2))
	{
		game.Start();
	}

	return 0;
}