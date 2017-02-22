#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include "IMG.h"
#include "Sprite.h"
#include "lib/json.hpp"

using JSON = nlohmann::json;

/* ******************************************** */

class Block
{
private :
	Sprite* sSprite;

	int iBlockID;

	bool bCollision;
	bool bDeath;
	bool bUse;
	bool bVisible;

public:
	Block(void);
	Block(int iBlockID, Sprite* sSprite, bool bCollision, bool bDeath, bool bUse, bool bVisible);
	~Block(void);

	void Draw(SDL_Renderer* rR, int iOffsetX, int iOffsetY);

	/* ----- get & set ----- */
	int getBlockID() const;
	void setBlockID(int iID);

	Sprite* getSprite();
	bool getCollision() const;
	bool getDeath() const;
	bool getUse() const;
	bool getVisible() const;

    friend void to_json(JSON& json, const Block* block);
    friend void from_json(const JSON& json, Block* block);
    friend void to_json(JSON& json, const std::vector<Block*>& blocks);
    friend void from_json(const JSON& json, std::vector<Block*>& blocks);
};

#endif
