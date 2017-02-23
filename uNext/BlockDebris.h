#pragma once

#ifndef BLOCKDEBRIS_H
#define BLOCKDEBRIS_H

#include "header.h"
#include "Vector2.h"

class BlockDebris
{
private:
	// ----- 0 = Animation, -1 = Delete
	int debrisState;

	Vector2* vPositionL;
	Vector2* vPositionR;
	Vector2* vPositionL2;
	Vector2* vPositionR2;

	int iFrameID;

	float fSpeedX, fSpeedY;

	bool bRotate;
public:
	BlockDebris(int iXPos, int iYPos);
	~BlockDebris(void);

	void Update();
	void Draw(SDL_Renderer* rR);

	int getDebrisState();

	friend void to_json(JSON& json, const BlockDebris* block);
	friend void from_json(const JSON& json, BlockDebris* block);
	friend void to_json(JSON& json, const std::vector<BlockDebris*>& blocks);
	friend void from_json(const JSON& json, std::vector<BlockDebris*>& blocks);
};

#endif