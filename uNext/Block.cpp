#include "Block.h"

/* ******************************************** */

Block::Block(void) { }

Block::Block(int iBlockID, Sprite* sSprite, bool bCollision, bool bDeath, bool bUse, bool bVisible) {
	this->iBlockID = iBlockID;
	this->sSprite = sSprite;
	this->bCollision = bCollision;
	this->bDeath = bDeath;
	this->bUse = bUse;
	this->bVisible = bVisible;
}

Block::~Block(void) {
	delete sSprite;
}

/* ******************************************** */

void Block::Draw(SDL_Renderer* rR, int iOffsetX, int iOffsetY) {
	sSprite->getTexture()->Draw(rR, iOffsetX, iOffsetY);
}

/* ******************************************** */

int Block::getBlockID() const {
	return iBlockID;
}

void Block::setBlockID(int iBlockID) {
	this->iBlockID = iBlockID;
}

Sprite* Block::getSprite() {
	return sSprite;
}

bool Block::getCollision() const {
	return bCollision;
}

bool Block::getDeath() const {
	return bDeath;
}

bool Block::getUse() const {
	return bUse;
}

bool Block::getVisible() const {
	return bVisible;
}

void to_json(JSON& json, const Block& block) {
    json = JSON({
                        {"id",     block.getBlockID()},
						{"values", {
										   block.getCollision(),
										   block.getDeath(),
										   block.getUse(),
										   block.getVisible()
								   }}
				});
}

void to_json(JSON& json, const std::vector<Block*> blocks) {
    std::vector<JSON> minionsJSON;
    for (auto& minion : blocks) {
        minionsJSON.emplace_back(JSON(*minion));
    }

    json = minionsJSON;
}