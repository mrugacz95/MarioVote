#pragma once

#ifndef VECTORR_H
#define VECTORR_H

#include "lib/json.hpp"

using JSON = nlohmann::json;


class Vector2
{
private:
	int X, Y;
	
public:
	Vector2(void);
	Vector2(int X, int Y);
	~Vector2(void);

	int getX();
	void setX(int X);
	int getY();
	void setY(int Y);

	friend void to_json(JSON& json, const Vector2* vector);
	friend void from_json(const JSON& json, Vector2* vector);
};

#endif