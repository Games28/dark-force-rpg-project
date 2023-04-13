#ifndef SPRITE_H
#define SPRITE_H

#include "olcPixelGameEngine.h"
#include "Player.h"
#include "Map.h"
#include <unordered_map>
#include <algorithm>


//class Object
//{
//public:
//	Object() = default;
//	uint32_t nGenericID = 0;
//
//	olc::vf2d pos;
//	olc::vf2d vel;
//	float fSpeed = 0.0f;
//	float fHeading = 0.0f;
//	float fRadius = 0.5f;
//	bool bVisible = true;
//	bool bRemove = false;
//	bool bCollideWithScenery = true;
//	bool bNotifySceneryCollision = false;
//	bool bCollideWithObjects = false;
//	bool bNotifyObjectCollision = false;
//	bool bCanBeMoved = true;
//	bool bIsActive = true;
//
//	void Walk(const float fWalkSpeed);
//	void Strafe(const float fStrafeSpeed);
//	void Turn(const float fTurnSpeed);
//	void Stop();
//
//};
//
//class Sprite
//{
//public:
//	Sprite() = default;
//	void Init();
//	void drawSprite(olc::PixelGameEngine* gfx,Player& player);
//
//public:
//	std::unordered_map<uint32_t, std::shared_ptr<Object>> mapObjects;
//	std::unique_ptr<float[]> pDepthBuffer;
//};
struct Object
{
	float x;
	float y;
	float vx;
	float vy;
	bool bRemove;
	olc::Sprite* sprite;
};

class Sprite
{
public:
	Sprite() = default;
	void initSprites(float screenwidth);
	void DrawSprites(olc::PixelGameEngine* gfx, int x, float fdisttowall, Player& player, Map& map, float deltatime);
	void newDraw();

public:
	float* fDepthBuffer = nullptr;
	std::list<Object> listObjects;
	olc::Sprite* spritewall;
	olc::Sprite* spritelamp;
	//std::unordered_map<uint32_t, std::shared_ptr<olc::rcw::Object>> mapObjects;
};
#endif // !SPRITE_H








