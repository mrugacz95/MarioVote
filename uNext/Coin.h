#pragma once

#ifndef COIN_H
#define COIN_H

#include "header.h"
#include "lib/json.hpp"

using JSON = nlohmann::json;


class Coin
{
private:
	int iXPos, iYPos;
	int iLEFT;

	int iSpriteID;
	int iStepID;

	bool bTOP;

	bool bDelete;
public:
	Coin(int iXPos, int iYPos);
	~Coin(void);

	void Update();
	void Draw(SDL_Renderer* rR);

	int getXPos();
	int getYPos();
	bool getDelete();

	friend void to_json(JSON& json, const Coin* coin);
	friend void from_json(const JSON& json, Coin* coin);
	friend void to_json(JSON& json, const std::vector<Coin*>& coins);
	friend void from_json(const JSON& json, std::vector<Coin*>& coins);
};

#endif