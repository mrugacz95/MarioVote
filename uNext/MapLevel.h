#pragma once

#ifndef MAP_LEVEL_H
#define MAP_LEVEL_H

#include "lib/json.hpp"

using JSON = nlohmann::json;

class MapLevel
{
private:
	int iBlockID;

	bool spawnMushroom;
	bool powerUP; // -- true = powerUP, false = 1UP
	bool spawnStar;
	int iNumOfUse;

	// ----- Animation -----

	bool blockAnimation;
	int iYPos;
	bool bYDirection;	// ----- true TOP, false BOTTOM

	// ----- Animation -----

public:
	MapLevel(void);
	MapLevel(int iBlockID);
	~MapLevel(void);

	void startBlockAnimation();

	int updateYPos();

	/* ----- get & set ----- */

	int getBlockID();
	void setBlockID(int iBlockID);

	int getYPos();

	int getNumOfUse();
	void setNumOfUse(int iNumOfUse);

	bool getSpawnMushroom();
	void setSpawnMushroom(bool spawnMushroom);
	bool getPowerUP();
	void setPowerUP(bool powerUP);
	bool getSpawnStar();
	void setSpawnStar(bool spawnStar);

	friend void to_json(JSON& json, const MapLevel* mapLevel);
	friend void from_json(const JSON& json, MapLevel* mapLevel);
	friend void to_json(JSON& json, const std::vector<MapLevel*>& mapLevels);
	friend void from_json(const JSON& json, std::vector<MapLevel*>& mapLevels);
	friend void to_json(JSON& json, const std::vector<std::vector<MapLevel*>>& mapLevelsVectors);
	friend void from_json(const JSON& json, std::vector<std::vector<MapLevel*>>& mapLevelsVectors);
};

#endif