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

void to_json(JSON& json, const Block* block) {
    json = JSON({
                         block->getCollision(),
                         block->getDeath(),
                         block->getUse(),
                         block->getVisible()
                 });
}

void from_json(const JSON& json, Block* block) {
    block->bCollision = json[0];
    block->bDeath = json[1];
    block->bUse = json[2];
    block->bVisible = json[3];
}

void to_json(JSON& json, const std::vector<Block*>& blocks) {
    std::vector<JSON> blocksVector;
    for (auto& block : blocks) {
        blocksVector.push_back(block);
    }

    json = blocksVector;
}

void from_json(const JSON &json, std::vector<Block*>& blocks) {
    for (auto& block : blocks) {
        auto blockJSON = json[block->iBlockID];
        from_json(blockJSON, block);
    }
}
