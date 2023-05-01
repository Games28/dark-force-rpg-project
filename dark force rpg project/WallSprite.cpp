#include "WallSprite.h"

bool WallSprites::init(bool& bSuccess)
{
    // lambda expression for loading sprite files with error checking on the existence of sFileName
    auto load_sprite_file = [=](const std::string& sFileName) {
        olc::Sprite* tmp = new olc::Sprite(sFileName);
        if (tmp->width == 0 || tmp->height == 0) {
            std::cout << "ERROR: OnUserCreate() --> can't load file: " << sFileName << std::endl;
            delete tmp;
            tmp = nullptr;
        }
        return tmp;
    };
    // load sprites for texturing walls, floor and ceiling
    //std::string sSpritePath = "../sprites/";
    //wallsprite
    sprites[0] = load_sprite_file("stonewall.png"); bSuccess &= (sprites[0] != nullptr);
    //floor
    sprites[1] = load_sprite_file("sand1.png"); bSuccess &= (sprites[1] != nullptr);
    //ceiling
    sprites[2] = load_sprite_file("sand.png"); bSuccess &= (sprites[2] != nullptr);
    //roof
    sprites[3] = load_sprite_file("Tatooinehousewall.png"); bSuccess &= (sprites[3] != nullptr);

    return bSuccess;
}
