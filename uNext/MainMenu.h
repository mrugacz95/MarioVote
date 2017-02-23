#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.h"
#include <thread>

class MainMenu : public Menu
{
private:
	bool selectWorld;
	bool selectServerAddress;
	int activeWorldID, activeSecondWorldID;
	int lastBackSpace = 0;
	SDL_Rect rSelectWorld;
	SDL_Rect rSelectServerAddr;

	std::string serverAddress = "192.168.1.231:6789";
	std::string getServerAddress();
	uint16_t getServerPort();
public:
	MainMenu(void);
	~MainMenu(void);

	void Update();
	void Draw(SDL_Renderer* rR);

	void enter();
	void escape();

	void updateActiveButton(int iDir);
};

#endif