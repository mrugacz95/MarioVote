#pragma once

#ifndef CORE_H
#define CORE_H

//STD
#include <memory>

#include "Map.h"
#include "Server.h"
#include "Client.h"
#include "lib/json.hpp"

using JSON = nlohmann::json;

class CCore
{
private:
	SDL_Window* window;
	SDL_Renderer* rR;
	SDL_Event* mainEvent;
	clock_t syncTime = 0;

	// ----- FPS -----

	long frameTime;
	static const int MIN_FRAME_TIME = 16;

	unsigned long lFPSTime;
	int iNumOfFPS, iFPS;

	// ----- FPS -----

	// ----- INPUT
	static bool movePressed, keyMenuPressed, keyS, keyW, keyA, keyD, keyShift;

	static bool keyAPressed, keyDPressed;
	// ----- true = RIGHT, false = LEFT
	bool firstDir;

	// ----- INPUT

	static Map* oMap;
    static std::shared_ptr<Server> server;
	static std::shared_ptr<Client> client;

	// ----- Methods

	void Input();
	void MouseInput();
	void InputPlayer();
	void InputMenu();
public:
	CCore(void);
	~CCore(void);

	static bool quitGame;

	void mainLoop();

	void Update();
	void Draw();

	void resetMove();
	static void resetKeys();

	static bool mouseLeftPressed, mouseRightPressed;
	static int mouseX, mouseY;
	
	/* ----- get & set ----- */
	static Map* getMap();
    static void createServer();
    static std::shared_ptr<Server> getServer();
	static void destroyServer();
	static void createClient(const char *ipv4Address, const uint16_t port);
	static std::shared_ptr<Client> getClient();
};

#endif
