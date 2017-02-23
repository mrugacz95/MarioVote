#include "LevelText.h"

/* ******************************************** */

LevelText::LevelText(int iXPos, int iYPos, std::string sText) {
	this->iXPos = iXPos;
	this->iYPos = iYPos;
	this->sText = sText;
}

LevelText::~LevelText(void) {

}

/* ******************************************** */

int LevelText::getXPos() {
	return iXPos;
}

int LevelText::getYPos() {
	return iYPos;
}

void LevelText::setText(std::string sText) {
	this->sText = sText;
}

std::string LevelText::getText() {
	return sText;
}

void to_json(JSON& json, const LevelText* text) {
	json = JSON({
			text->iXPos,
			text->iYPos,
			text->sText
				});
}

void from_json(const JSON& json, LevelText* text) {
	text->iXPos = json[0];
	text->iYPos = json[1];
	text->sText = json[2];
}

void to_json(JSON& json, const std::vector<LevelText*>& texts) {
	std::vector<JSON> textsVector;
	for (auto& text : texts) {
		textsVector.push_back(text);
	}

	json = textsVector;
}

void from_json(const JSON& json, std::vector<LevelText*>& texts) {
	for (int i = 0; i < texts.size(); i++) {
		auto textJSON = json[i];
		from_json(textJSON, texts[i]);
	}
}