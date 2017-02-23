#pragma once

#ifndef LEVELTEXT_H
#define LEVELTEXT_H

#include <string>

#include "lib/json.hpp"

using JSON = nlohmann::json;


class LevelText
{
private:

	int iXPos, iYPos;

	std::string sText;

public:
	LevelText(int iXPos, int iYPos, std::string sText);
	~LevelText(void);

	int getXPos();
	int getYPos();
	std::string getText();
	void setText(std::string sText);

	friend void to_json(JSON& json, const LevelText* text);
	friend void from_json(const JSON& json, LevelText* text);
	friend void to_json(JSON& json, const std::vector<LevelText*>& texts);
	friend void from_json(const JSON& json, std::vector<LevelText*>& texts);
};

#endif