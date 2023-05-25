#ifndef ANIMATION_H
#define ANIMATION_H
#include "olcPixelGameEngine.h"

class AnimationData
{
public:
	AnimationData() = default;
	olc::vf2d sourcePos = { 0.0f,0.0f };
	olc::vf2d sourceSize = { 0.0f,0.0f };

};

class SpriteAnimation
{
public:
	float mInterval = 0.0f;
	int mTotalWidth = 0;
	int mTotalHeight = 0;
	int mTileCountX = 0;
	int mTileCountY = 0;
	int mTotalTileCount = 0;
	float mCurrentTime = 0.0f;
	int mCurrentTile = 0;
	int tileMovement = 0;
	int invertedstart = 0;
	bool movement = false;
	bool inverted = false;

public:
	SpriteAnimation() = default;

	void SetParams(float intervel, int totalwidth, int totalheight, int tilecountx, int tilecounty, int totaltiles, int tilemove, bool invert, bool move)
	{
		mInterval = intervel;
		mTotalWidth = totalwidth;
		mTotalHeight = totalheight;
		mTileCountX = tilecountx;
		mTileCountY = tilecounty;
		mTotalTileCount = totaltiles;
		tileMovement = tilemove;
		invertedstart = tilemove * totaltiles;
		movement = move;
		inverted = invert;
		

	}

	
	AnimationData GetInfo(float fElapsedTime, float& next, bool& reset)
	{

		

		mCurrentTime += fElapsedTime;
		if (inverted)
		{
			if (mCurrentTime >= mInterval)
			{
				mCurrentTime = 0.0f;
				mCurrentTile--;
				if (movement)
				{
					next -= tileMovement;
				}
				
			}

			if (mCurrentTile < 0)
			{
				mCurrentTile = mTotalTileCount - 1;
				if (movement)
				{
					
					next = invertedstart;
				}
				
				reset = false;
			}
		}
		else {
			if (mCurrentTime >= mInterval)
			{
				mCurrentTime = 0.0f;
				mCurrentTile++;
				if (movement)
				{
					next += tileMovement;
				}
			}

			if (mCurrentTile > mTotalTileCount )
			{
				mCurrentTile = 0;
				if (movement)
				{
					
					next = 0;
				}
				
				reset = false;
			}
		}
		AnimationData data;

		data.sourceSize.x = (float)mTotalWidth / (float)mTileCountX;
		data.sourceSize.y = (float)mTotalHeight / (float)mTileCountY;

		data.sourcePos.x = (mCurrentTile % mTileCountX) * data.sourceSize.x;
		data.sourcePos.y = floorf((float)mCurrentTile / (float)mTileCountX) * data.sourceSize.y;
		
		return data;
	}
};
#endif // !ANIMATION_H
