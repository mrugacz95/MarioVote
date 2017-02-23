#include "Bubble.h"
#include "Core.h"
#include "CFG.h"

/* ******************************************** */

Bubble::Bubble(int iXPos, int iYPos) {
	this->iXPos = iXPos;
	this->iYPos = iYPos;

	this->bDestoy = false;

	this->iBlockID = 96;
}

Bubble::~Bubble(void) {

}

/* ******************************************** */

void Bubble::Update() {
	if(iYPos < CCFG::GAME_HEIGHT - 12*32 - 8) {
		bDestoy = true;
	} else {
		iYPos -= 1;
	}
}

void Bubble::Draw(SDL_Renderer* rR, CIMG* iIMG) {
	iIMG->Draw(rR,(int)(iXPos + CCore::getMap()->getXPos()), iYPos);
}

/* ******************************************** */

int Bubble::getBlockID() {
	return iBlockID;
}

bool Bubble::getDestroy() {
	return bDestoy;
}

void to_json(JSON& json, const Bubble* bubble) {
	json = JSON({
			bubble->iXPos,
			bubble->iYPos,
			bubble->bDestoy,
			bubble->iBlockID
				});
}

void from_json(const JSON& json, Bubble* bubble) {
	bubble->iXPos = json[0];
	bubble->iYPos = json[1];
	bubble->bDestoy = json[2];
	bubble->iBlockID = json[3];
}

void to_json(JSON& json, const std::vector<Bubble*>& bubbles) {
	std::vector<JSON> bubblesVector;
	for (auto& bubble : bubbles) {
		bubblesVector.push_back(bubble);
	}

	json = bubblesVector;
}

void from_json(const JSON& json, std::vector<Bubble*>& bubbles) {
	for (auto& bubble : bubbles) {
		auto bubbleJSON = json[bubble->iBlockID];
		from_json(bubbleJSON, bubble);
	}
}