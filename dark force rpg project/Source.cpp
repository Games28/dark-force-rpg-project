#include <cfloat>       // needed for constant FLT_MAX in the DDA function

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Player.h"
#include "Sprite.h"
#include "Map.h"
#include "defs.h"
#include "Raycast.h"
#include "Menu.h"
#include "Saber.h"
#include "Powers.h"
#include "RC_Map.h"
#include "MapLayout.h"


enum GameState
{
    GAME = 0,
    MENU = 1
};

class MyRayCaster : public olc::PixelGameEngine {

public:
    MyRayCaster() {    // display the screen and pixel dimensions in the window caption
        sAppName = "MyRayCaster - Permadi tutorial - S:(" + std::to_string(SCREEN_X / PIXEL_X) + ", " + std::to_string(SCREEN_Y / PIXEL_Y) + ")" +
            ", P:(" + std::to_string(PIXEL_X) + ", " + std::to_string(PIXEL_Y) + ")";
    }

public:
    Player player;
    Map map;
    Sprite sprite;
    Raycast ray;
    Menu menu;
    Saber saber;
    Powers power;
    bool bMouseControl = MOUSE_CONTROL;
    GameState gamestate = GameState::GAME;
    RC_Map cMap;
    bool resetAniamtion = false;
    // max visible distance - use length of map diagonal to overlook whole map
    int nMapX = 32;
    int nMapY = 32;
    float fMaxDistance;
    bool isHit = false;
    float beforeRotate = 0.0f;
    float afterRotate = 0.0f;
    olc::vf2d playerbefore = { 0.0f, 0.0f };
    olc::vf2d playerafter = { 0.0f, 0.0f };
    int saberinner = 0;
    int saberouter = 0;
    int saberanimation = 0;
    int selectedAnimation = 0;
    // player: position and looking angle

            // constant distance to projection plane - is calculated in OnUserCreate()

    olc::Sprite* pWallSprite = nullptr;    // these pointers are populated in OnUserCreate()
    olc::Sprite* pFloorSprite = nullptr;
    olc::Sprite* pCeilSprite = nullptr;
    olc::Sprite* pRoofSprite = nullptr;

    bool bMinimap = false;    // toggle on mini map rendering (trigger key P)
    bool bMapRays = false;    //                              (trigger key O)
    bool bDebugInfo = false;    //                              (trigger key I)

public:




    bool OnUserCreate() override {
        bool bSuccess = true;

        init_lu_sin_array();
        init_lu_cos_array();

        cMap.InitMap(16, 16);
        cMap.AddLayer(sMap_level0);
        cMap.AddLayer(sMap_level1);
       cMap.AddLayer(sMap_level2);
        cMap.AddLayer(sMap_level3);
        fMaxDistance = cMap.DiagonalLength();

            ray.Init(bSuccess,cMap);
            sprite.initSprites(ScreenWidth(),ScreenHeight());
            saber.initSaber(bSuccess);

            menu.InitMenu(*this);
            power.initsprite();


        return bSuccess;
    }


    bool GetMouseSteering(float& fHorPerc, float& fVerPerc) {
        // grab mouse coordinates
        int nMouseX = GetMouseX();
        int nMouseY = GetMouseY();

        // express mouse coords in -1.0f, 1.0f range
        float fRangeX = (nMouseX - (ScreenWidth() / 2)) / float(ScreenWidth() / 2);
        float fRangeY = (nMouseY - (ScreenHeight() / 2)) / float(ScreenHeight() / 2);

        // the screen width / height is mapped onto [ -1.0, +1.0 ] range
        // the range [ -0.2f, +0.2f ] is the stable (inactive) zone
        fHorPerc = 0.0f;
        fVerPerc = 0.0f;
        // if outside the stable zone, map to [ -1.0f, +1.0f ] again
        if (fRangeX < -0.2f) fHorPerc = (fRangeX + 0.2f) * (1.0f / 0.8f);
        if (fRangeX > 0.2f) fHorPerc = (fRangeX - 0.2f) * (1.0f / 0.8f);
        if (fRangeY < -0.2f) fVerPerc = (fRangeY + 0.2f) * (1.0f / 0.8f);
        if (fRangeY > 0.2f) fVerPerc = (fRangeY - 0.2f) * (1.0f / 0.8f);

        return (fHorPerc != 0.0f || fVerPerc != 0.0f);
    }

    // Shade the pixel p using fDistance as a factor in the shade formula

    bool OnUserUpdate(float fElapsedTime) override {

        // gamestate controls
        if (GetKey(olc::M).bPressed)
        {
            gamestate = GameState::MENU;
        }
        if (GetKey(olc::N).bPressed)
        {
            gamestate = GameState::GAME;
        }
        // ===================
        if (gamestate == GameState::GAME)
        {
            playerbefore = { player.fPlayerX, player.fPlayerY };
            beforeRotate = player.fPlayerA_deg;
            // Rotate - collision detection not necessary. Keep fPlayerA_deg between 0 and 360 degrees
            if (GetKey(olc::D).bHeld) { player.fPlayerA_deg += SPEED_ROTATE * fElapsedTime; if (player.fPlayerA_deg >= 360.0f) player.fPlayerA_deg -= 360.0f; }
            if (GetKey(olc::A).bHeld) { player.fPlayerA_deg -= SPEED_ROTATE * fElapsedTime; if (player.fPlayerA_deg < 0.0f) player.fPlayerA_deg += 360.0f; }

            // variables used for collision detection - work out the new location in a seperate coordinate pair, and only alter
            // the players coordinate if there's no collision
            float fNewX = player.fPlayerX;
            float fNewY = player.fPlayerY;

            // walking forward, backward and strafing left, right
            float fPlayerA_rad = player.fPlayerA_deg * PI / 180.0f;
            if (GetKey(olc::W).bHeld) { fNewX += cos(fPlayerA_rad) * SPEED_MOVE * fElapsedTime; fNewY += sin(fPlayerA_rad) * SPEED_MOVE * fElapsedTime; }   // walk forward
            if (GetKey(olc::S).bHeld) { fNewX -= cos(fPlayerA_rad) * SPEED_MOVE * fElapsedTime; fNewY -= sin(fPlayerA_rad) * SPEED_MOVE * fElapsedTime; }   // walk backwards
            if (GetKey(olc::Q).bHeld) { fNewX += sin(fPlayerA_rad) * SPEED_STRAFE * fElapsedTime; fNewY -= cos(fPlayerA_rad) * SPEED_STRAFE * fElapsedTime; }   // strafe left
            if (GetKey(olc::E).bHeld) { fNewX -= sin(fPlayerA_rad) * SPEED_STRAFE * fElapsedTime; fNewY += cos(fPlayerA_rad) * SPEED_STRAFE * fElapsedTime; }   // strafe right
            // collision detection - check if out of bounds or inside non-empty tile
            // only update position if no collision
            if (cMap.IsInBounds(fNewX, fNewY) &&
                // collision detection criterion - is players height > height of map?
                cMap.CellHeight(int(fNewX), int(fNewY)) < player.fPlayerH) {
                player.fPlayerX = fNewX;
                player.fPlayerY = fNewY;
            }

            // for looking up/down or crouching/flying you can speed up by keeping SHIFT pressed
            float fSpeedUp = GetKey(olc::SHIFT).bHeld ? 4.0f : 1.0f;
            // looking up or down - collision detection not necessary
            // NOTE - there's no clamping to extreme values (yet)
            if (GetKey(olc::UP).bHeld) { player.fLookUp += SPEED_LOOKUP * fSpeedUp * fElapsedTime; }
            if (GetKey(olc::DOWN).bHeld) { player.fLookUp -= SPEED_LOOKUP * fSpeedUp * fElapsedTime; }

            // Mouse control
            if (GetKey(olc::M).bReleased) { bMouseControl = !bMouseControl; }  // toggle on or off
            float fRotFactor, fTiltFactor;
            if (bMouseControl && GetMouseSteering(fRotFactor, fTiltFactor)) {
                player.fPlayerA_deg += SPEED_ROTATE * fRotFactor * fElapsedTime;
                player.fLookUp -= SPEED_LOOKUP * fTiltFactor * fElapsedTime;
            }

            // flying or crouching
            // NOTE - for multi level rendering there's only clamping to keep fPlayerH > 0.0f, there's no upper limit.

            // cache current height of horizon, so that you can compensate for changes in it via the look up value
            float fCacheHorHeight = float(ScreenHeight() * player.fPlayerH) + player.fLookUp;
            if (MULTIPLE_LEVELS) {
                // if the player height is adapted, keep horizon height stable by compensating with look up value
                if (GetKey(olc::PGUP).bHeld) {
                    player.fPlayerH += SPEED_STRAFE_UP * fSpeedUp * fElapsedTime;
                    player.fLookUp = fCacheHorHeight - float(ScreenHeight() * player.fPlayerH);
                }
                if (GetKey(olc::PGDN).bHeld) {
                    float fNewHeight = player.fPlayerH - SPEED_STRAFE_UP * fSpeedUp * fElapsedTime;
                    // prevent negative height, and do CD on the height map
                    if (fNewHeight > 0.0f && cMap.CellHeight(int(player.fPlayerX), int(player.fPlayerY)) < fNewHeight) {
                        player.fPlayerH = fNewHeight;
                        player.fLookUp = fCacheHorHeight - float(ScreenHeight() * player.fPlayerH);
                    }
                }
            }
            else {
                if (GetKey(olc::PGUP).bHeld) {
                    float fNewHeight = player.fPlayerH + SPEED_STRAFE_UP * fSpeedUp * fElapsedTime;
                    if (fNewHeight < 1.0f) {
                        player.fPlayerH = fNewHeight;
                        // compensate look up value so that horizon remains stable
                        player.fLookUp = fCacheHorHeight - float(ScreenHeight() * player.fPlayerH);
                    }
                }
                if (GetKey(olc::PGDN).bHeld) {
                    float fNewHeight = player.fPlayerH - SPEED_STRAFE_UP * fSpeedUp * fElapsedTime;
                    if (fNewHeight > 0.0f) {
                        player.fPlayerH = fNewHeight;
                        // compensate look up value so that horizon remains stable
                        player.fLookUp = fCacheHorHeight - float(ScreenHeight() * player.fPlayerH);
                    }
                }
            }
            //telekinesis toggle




            playerafter = { player.fPlayerX, player.fPlayerY };
            afterRotate = player.fPlayerA_deg;
            // reset look up value and player height on pressing 'R'
            if (GetKey(olc::R).bReleased) { player.fPlayerH = 0.5f; player.fLookUp = 0.0f; }

            // alter object intensity and multiplier
            ray.constrols(*this, fElapsedTime);

            // step 2 - game logic
            // ===================

            // step 3 - render
            // ===============

            Clear(RENDER_CEILING ? olc::BLACK : olc::CYAN);

            //ray.raycasting(*this, player, cMap, sprite);
            ray.raycast(*this, player, cMap, sprite);

            //sprite.DrawSprites(this, player, map, fElapsedTime);
           // sprite.DrawIcon(this, player, map, fElapsedTime);

            
           
            if (resetAniamtion != true)
            {
                //left angle attack
                if (GetKey(olc::J).bPressed) { resetAniamtion = true;  selectedAnimation = 0; }
                //right angle attack
                if (GetKey(olc::L).bPressed) { resetAniamtion = true;  selectedAnimation = 1; }
            }

            if (resetAniamtion != false)
            {
                switch (selectedAnimation)
                {
                case 0:
                    saber.DrawSaberAnimation(this, resetAniamtion, 0, 1, 0, fElapsedTime);
                    break;
                    
                case 1:
                    saber.DrawSaberAnimation(this, resetAniamtion, 1, 3, 2, fElapsedTime);
                    break;

                case 3:
                    saber.DrawSaberAnimation(this, resetAniamtion, saberanimation, saberinner, saberouter, fElapsedTime);
                    break;

                case 4:
                    saber.DrawSaberAnimation(this, resetAniamtion, saberanimation, saberinner, saberouter, fElapsedTime);
                    break;
                }
                
            }
            else
            {
                saber.DrawSaber(this);
            }



            //object selection

            float fObjPlyA;
            for (int i = 0; i < (sprite.listObjects.size() / 2); i++)
            {
                int objlist = sprite.listObjects.size() / 2 + i;

                Object& obj = sprite.listObjects[objlist];
                Object& icon = sprite.listObjects[i];
                if (!GetKey(olc::SPACE).bHeld)
                {
                    obj.ishit = power.isinSight(obj, player, 3.0f * (3.14159f / 180.0f), fObjPlyA);
                    if (obj.ishit)
                    {
                        sprite.Drawtest(this, icon, player, cMap, fElapsedTime);
                    }
                }





                    if (GetKey(olc::SPACE).bHeld && obj.ishit)
                    {
                        obj.pickedup = true;
                    }
                    if (obj.pickedup)
                    {
                        power.tkMove(obj, playerbefore, playerafter);
                        power.tkRotation(*this, obj, player, beforeRotate, afterRotate, -1);
                        power.tkMove(icon, playerbefore, playerafter);
                        power.tkRotation(*this, icon, player, beforeRotate, afterRotate, -1);
                    }


                if (GetKey(olc::SPACE).bReleased) obj.pickedup = false;
                sprite.Drawtest(this,obj,player,cMap,fElapsedTime);


            }




                if (GetKey(olc::SPACE).bReleased) isHit = false;

        }
        if (gamestate == GameState::MENU)
        {
            Clear(olc::BLACK);
            menu.updateMenu(*this,saber);
        }
        return true;
    }
};

int main()
{
    MyRayCaster demo;
    if (demo.Construct(SCREEN_X / PIXEL_X, SCREEN_Y / PIXEL_Y, PIXEL_X, PIXEL_Y))
        demo.Start();

    return 0;
}
