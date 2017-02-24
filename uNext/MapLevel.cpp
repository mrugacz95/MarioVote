#include "MapLevel.h"

/* ******************************************** */

MapLevel::MapLevel(void) { }

MapLevel::MapLevel(int iBlockID) {
	this->iBlockID = iBlockID;

	this->iNumOfUse = 0;

	this->blockAnimation = false;
	this->iYPos = 0;
	this->bYDirection = true;

	this->spawnMushroom = false;
	this->spawnStar = false;

	this->powerUP = true;
}

MapLevel::~MapLevel(void) {

}

/* ******************************************** */

void MapLevel::startBlockAnimation() {
	this->blockAnimation = true;
	this->iYPos = 0;
	this->bYDirection = true;
}

int MapLevel::updateYPos() {
	if(blockAnimation) {
		if (bYDirection)
		{
			if (iYPos < 10)
			{
				if (iYPos < 5) {
					iYPos += 2;
				} else {
					 iYPos += 1;
				}
			}
			else {
				bYDirection = false;
			}
		}
		else {
			if (iYPos > 0) {
				if (iYPos > 5) {
					iYPos -= 2;
				} else {
					iYPos -= 1;
				}
			}
			else {
				blockAnimation = false;
			}
		}
	}

	return iYPos;
}

/* ******************************************** */

int MapLevel::getBlockID() {
	return iBlockID;
}

void MapLevel::setBlockID(int iBlockID) {
	this->iBlockID = iBlockID;
}

int MapLevel::getYPos() {
	return iYPos;
}

int MapLevel::getNumOfUse() {
	return iNumOfUse;
}

void MapLevel::setNumOfUse(int iNumOfUse) {
	this->iNumOfUse = iNumOfUse;
}

bool MapLevel::getSpawnMushroom() {
	return spawnMushroom;
}

void MapLevel::setSpawnMushroom(bool spawnMushroom) {
	this->spawnMushroom = spawnMushroom;
}

bool MapLevel::getPowerUP() {
	return powerUP;
}

void MapLevel::setPowerUP(bool powerUP) {
	this->powerUP = powerUP;
}

bool MapLevel::getSpawnStar() {
	return spawnStar;
}

void MapLevel::setSpawnStar(bool spawnStar) {
	this->spawnStar = spawnStar;
}

void to_json(JSON &json, const MapLevel* mapLevel) {
	json = JSON({
						mapLevel->spawnMushroom,
						mapLevel->powerUP,
						mapLevel->spawnStar,
						mapLevel->iNumOfUse,
						mapLevel->blockAnimation,
						mapLevel->iYPos,
						mapLevel->bYDirection
				});
}

void from_json(const JSON &json, MapLevel* mapLevel) {
	mapLevel->spawnMushroom = json[0];
	mapLevel->powerUP = json[1];
	mapLevel->spawnStar = json[2];
	mapLevel->iNumOfUse = json[3];
	mapLevel->blockAnimation = json[4];
	mapLevel->iYPos = json[5];
	mapLevel->bYDirection = json[6];
}

void to_json(JSON &json, const std::vector<MapLevel *> &mapLevels) {
	std::vector<JSON> mapLevelsVector;
	for (auto& mapLevel : mapLevels) {
		mapLevelsVector.push_back(mapLevel);
	}

	json = mapLevelsVector;
}

void from_json(const JSON &json, std::vector<MapLevel *> &mapLevels) {
	for (int i = 0; i < mapLevels.size(); i++) {
		auto mapLevelsJSON = json[i];
		from_json(mapLevelsJSON, mapLevels[i]);
	}
}

void to_json(JSON &json, const std::vector<std::vector<MapLevel *>> &mapLevelsVectors) {
	std::vector<JSON> mapLevelsGrid;
	for (auto& mapLevels : mapLevelsVectors) {
		mapLevelsGrid.push_back(mapLevels);
	}

	json = mapLevelsGrid;
}

void from_json(const JSON &json, std::vector<std::vector<MapLevel *>> &mapLevelsVectors) {
	for (int i = 0; i < mapLevelsVectors.size(); i++) {
		auto mapLevelsJSON = json[i];
		from_json(mapLevelsJSON, mapLevelsVectors[i]);
	}
}
