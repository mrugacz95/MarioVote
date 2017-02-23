#pragma once

#ifndef BUBBLE_H
#define BUBBlE_H

#include "header.h"
#include "IMG.h"
#include "lib/json.hpp"

using JSON = nlohmann::json;


class Bubble
{
private:
	int iXPos, iYPos;

	bool bDestoy; // -- DELETE BUBBLE
	int iBlockID;
public:
	Bubble(int iXPos, int iYPos);
	~Bubble(void);

	void Update();
	void Draw(SDL_Renderer* rR, CIMG* iIMG);
	
	int getBlockID();
	bool getDestroy();

	friend void to_json(JSON& json, const Bubble* bubble);
	friend void from_json(const JSON& json, Bubble* bubble);
	friend void to_json(JSON& json, const std::vector<Bubble*>& bubbles);
	friend void from_json(const JSON& json, std::vector<Bubble*>& bubbles);
};

#endif