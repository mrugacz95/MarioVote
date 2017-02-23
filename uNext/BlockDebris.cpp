#include "BlockDebris.h"
#include "CFG.h"
#include "Core.h"

/* ******************************************** */

BlockDebris::BlockDebris(int iXPos, int iYPos) {
	this->debrisState = 0;
	this->bRotate = false;

	this->vPositionL = new Vector2(iXPos, iYPos);
	this->vPositionR = new Vector2(iXPos + 16, iYPos);
	this->vPositionL2 = new Vector2(iXPos, iYPos + 16);
	this->vPositionR2 = new Vector2(iXPos + 16, iYPos + 16);

	this->fSpeedX = 2.15f;
	this->fSpeedY = 1;
}

BlockDebris::~BlockDebris(void) {
	delete vPositionL;
	delete vPositionR;
	delete vPositionL2;
	delete vPositionR2;
}

/* ******************************************** */

void BlockDebris::Update() {
	++iFrameID;

	if(iFrameID > 4) {
		bRotate = !bRotate;
		iFrameID = 0;
	}

	vPositionL->setX((int)(vPositionL->getX() - fSpeedX));
	vPositionL->setY((int)(vPositionL->getY() + (fSpeedY - 3.0f) + (fSpeedY < 3 ? fSpeedY < 2.5f ? -3 : -1.5f : fSpeedY < 3.5f ? 1.5f : 3)));

	vPositionR->setX((int)(vPositionR->getX() + fSpeedX));
	vPositionR->setY((int)(vPositionR->getY() + (fSpeedY - 3.0f) + (fSpeedY < 3 ? fSpeedY < 2.5f ? -3 : -1.5f : fSpeedY < 3.5f ? 1.5f : 3)));

	vPositionL2->setX((int)(vPositionL2->getX() - (fSpeedX - 1.1f)));
	vPositionL2->setY((int)(vPositionL2->getY() + (fSpeedY - 1.5f) + (fSpeedY < 1.5 ? fSpeedY < 1.3f ? -3 : -1 : fSpeedY < 1.8f ? 1 : 3)));

	vPositionR2->setX((int)(vPositionR2->getX() + (fSpeedX - 1.1f)));
	vPositionR2->setY((int)(vPositionR2->getY() + (fSpeedY - 1.5f) + (fSpeedY < 1.5 ? fSpeedY < 1.3f ? -3 : -1 : fSpeedY < 1.8f ? 1 : 3)));

	fSpeedY *= 1.09f;
	fSpeedX *= 1.005f;

	if (vPositionL->getY() >= CCFG::GAME_HEIGHT) {
		debrisState = -1;
	}
}

void BlockDebris::Draw(SDL_Renderer* rR) {
	CCore::getMap()->getBlock(CCore::getMap()->getLevelType() == 0 || CCore::getMap()->getLevelType() == 4 ? 64 : CCore::getMap()->getLevelType() == 1 ? 65 : 66)->getSprite()->getTexture()->Draw(rR, vPositionL->getX() + (int)CCore::getMap()->getXPos(), vPositionL->getY(), bRotate);
	CCore::getMap()->getBlock(CCore::getMap()->getLevelType() == 0 || CCore::getMap()->getLevelType() == 4 ? 64 : CCore::getMap()->getLevelType() == 1 ? 65 : 66)->getSprite()->getTexture()->Draw(rR, vPositionR->getX() + (int)CCore::getMap()->getXPos(), vPositionR->getY(), bRotate);
	CCore::getMap()->getBlock(CCore::getMap()->getLevelType() == 0 || CCore::getMap()->getLevelType() == 4 ? 64 : CCore::getMap()->getLevelType() == 1 ? 65 : 66)->getSprite()->getTexture()->Draw(rR, vPositionL2->getX() + (int)CCore::getMap()->getXPos(), vPositionL2->getY(), bRotate);
	CCore::getMap()->getBlock(CCore::getMap()->getLevelType() == 0 || CCore::getMap()->getLevelType() == 4 ? 64 : CCore::getMap()->getLevelType() == 1 ? 65 : 66)->getSprite()->getTexture()->Draw(rR, vPositionR2->getX() + (int)CCore::getMap()->getXPos(), vPositionR2->getY(), bRotate);
}

/* ******************************************** */

int BlockDebris::getDebrisState() {
	return debrisState;
}

void to_json(JSON &json, const BlockDebris *block) {
	json = JSON({
			block->debrisState,
			block->vPositionL,
			block->vPositionR,
			block->vPositionL2,
			block->vPositionR2,
			block->iFrameID,
			block->fSpeedX,
			block->fSpeedY,
			block->bRotate
				});
}

void from_json(const JSON &json, BlockDebris *block) {
	block->debrisState = json[0];
	from_json(json[1], block->vPositionL);
	from_json(json[2], block->vPositionR);
	from_json(json[3], block->vPositionL2);
	from_json(json[4], block->vPositionR2);
	block->iFrameID = json[5];
	block->fSpeedX = json[6];
	block->fSpeedY = json[7];
	block->bRotate = json[8];
}

void to_json(JSON &json, const std::vector<BlockDebris *> &blocks) {
	std::vector<JSON> blocksVector;
	for (auto& block : blocks) {
		blocksVector.push_back(block);
	}

	json = blocksVector;
}

void from_json(const JSON &json, std::vector<BlockDebris *> &blocks) {
	for (int i = 0; i < blocks.size(); i++) {
		auto blockJSON = json[i];
		from_json(blockJSON, blocks[i]);
	}
}
