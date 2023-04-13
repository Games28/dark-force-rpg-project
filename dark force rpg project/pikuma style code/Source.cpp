#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "defs.h"
#include "Map.h"
#include "Player.h"
#include "Raycast.h"
#include "Wall.h"
#include "Sprite.h"
#include "Saber.h"


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
	Sprite sprite;
	Saber saber;
public:

	bool OnUserCreate() override
	{
		wall.wallTextures();
		sprite.initSpriteinfo();
		sprite.initsprites();
		saber.initSaber();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{


        // for debugging purposes
        if (GetKey( olc::Key::F1 ).bHeld) {
            wall.fTestRay -= 0.1f * fElapsedTime;
            if (wall.fTestRay < 0.0f)
                wall.fTestRay = 0.0f;
            wall.nTestRay = int( (NUM_RAYS - 1) * wall.fTestRay );
        }
        if (GetKey( olc::Key::F2 ).bHeld) {
            wall.fTestRay += 0.1f * fElapsedTime;
            if (wall.fTestRay > 1.0f)
                wall.fTestRay = 1.0f;
            wall.nTestRay = int( (NUM_RAYS - 1) * wall.fTestRay );
        }


		player.processInput(this,fElapsedTime,map);
		player.movePlayer(fElapsedTime, map);
		Clear(RENDER_CEILING ? olc::BLACK : olc::BLUE);
		ray.castAllRays(player, map);

		wall.renderWallProjection(this, player, ray);
		sprite.SpriteProjection(this, ray, player);

		//map.renderMapGrid(this);           // little map
		//player.renderMapPlayer(this);// player in the map
		//sprite.mapSprites(this);
		//ray.renderMapRays(this, player, wall.nTestRay );   // rays in the map
		saber.DrawSaber(this,fElapsedTime);
		DrawString(400, 10, "player x  = " + std::to_string(player.x));
		DrawString(400, 20, "player y  = " + std::to_string(player.y));
		DrawString(400, 30, "player a1 = " + std::to_string(player.rotationAngle));
		DrawString(400, 40, "player a2 = " + std::to_string(player.playerLookAngle));
		DrawString(400, 50, "fLookUp   = " + std::to_string(player.lookupordown));
		DrawString(400, 60, "fPlayerH  = " + std::to_string(player.fPlayerH));

		//DrawLine( wall.nTestRay, 0, wall.nTestRay, ScreenHeight(), olc::MAGENTA );

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
