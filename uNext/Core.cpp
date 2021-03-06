#include "header.h"
#include "Core.h"
#include "IMG.h"
#include "CFG.h"
#include "Text.h"-
#include "SDL_mixer.h"

/* ******************************************** */

Map* CCore::oMap = new Map();
std::shared_ptr<Server> CCore::server = nullptr;
std::shared_ptr<Client> CCore::client = nullptr;
bool CCore::mouseLeftPressed = false;
bool CCore::mouseRightPressed = false;
int CCore::mouseX = 0;
int CCore::mouseY = 0;
bool CCore::quitGame = false;

bool CCore::movePressed = false;
bool CCore::keyMenuPressed = false;
bool CCore::keyS = false;
bool CCore::keyW = false;
bool CCore::keyA = false;
bool CCore::keyD = false;
bool CCore::keyShift = false;
bool CCore::keyAPressed = false;
bool CCore::keyDPressed = false;

CCore::CCore(void) {
	this->quitGame = false;
	this->iFPS = 0;
	this->iNumOfFPS = 0;
	this->lFPSTime = 0;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
	
	window = SDL_CreateWindow("MarioVote, Marcin Mrugas & Jakub Nurski", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CCFG::GAME_WIDTH, CCFG::GAME_HEIGHT, SDL_WINDOW_SHOWN);

	if(window == NULL) {
		quitGame = true;
	}

	rR = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// ----- ICO
	std::string fileName = "files/images/ico.bmp";
	SDL_Surface* loadedSurface = SDL_LoadBMP(fileName.c_str());
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));

	SDL_SetWindowIcon(window, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	mainEvent = new SDL_Event();
	// ----- ICO
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	
	oMap = new Map(rR);
	CCFG::getMM()->setActiveOption(rR);
	CCFG::getSMBLOGO()->setIMG("super_mario_bros", rR);

	CCFG::getMusic()->PlayMusic();

	this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;

	this->keyAPressed = this->keyDPressed = this->firstDir = false;

	this->mouseX = this->mouseY = 0;

	CCFG::keyIDA = SDLK_a;
	CCFG::keyIDS = SDLK_s;
	CCFG::keyIDD = SDLK_d;
	CCFG::keyIDSpace = SDLK_SPACE;
	CCFG::keyIDShift = SDLK_LSHIFT;
}

CCore::~CCore(void) {
	delete oMap;
	delete mainEvent;
	SDL_DestroyRenderer(rR);
	SDL_DestroyWindow(window);
}

/* ******************************************** */

void CCore::mainLoop() {
	lFPSTime = SDL_GetTicks();

	while(!quitGame && mainEvent->type != SDL_QUIT) {
		frameTime = SDL_GetTicks();
		SDL_PollEvent(mainEvent);
		SDL_RenderClear(rR);

		CCFG::getMM()->setBackgroundColor(rR);
		SDL_RenderFillRect(rR, NULL);

		Input();
		MouseInput();
        if(client && CCFG::getMM()->currentGameState == MenuManager::gameState::eGame){
            JSON clientInput;
            clientInput["direction"] = firstDir;
            clientInput["A"] = keyAPressed;
            clientInput["S"] = keyS;
            clientInput["D"] = keyDPressed;
            clientInput["shift"] = keyShift;
            clientInput["space"] = CCFG::keySpace;
            clientInput["isPaused"] = CCFG::getMM()->currentGameState == MenuManager::gameState::ePause;
            client->sendVoteToServer(clientInput);
        }

		if (server && server->isStarted()) {
            JSON input;
            input["direction"] = firstDir;
            input["A"] = keyAPressed;
            input["S"] = keyS;
            input["D"] = keyDPressed;
            input["shift"] = keyShift;
			input["space"] = CCFG::keySpace;
			input["isPaused"] = CCFG::getMM()->currentGameState == MenuManager::gameState::ePause;

			static auto last_synchronization = std::chrono::high_resolution_clock::now();
			auto now = std::chrono::high_resolution_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_synchronization).count() > 100) {
				JSON mapJSON;
				to_json(mapJSON, oMap);
				mapJSON["isPaused"] = input["isPaused"];
				server->sendToClientsWithStatus(ClientStatus::NOT_SYNCHRONIZED, mapJSON);

				last_synchronization = now;
				std::cout << "Server synchro: " << std::chrono::system_clock::to_time_t(last_synchronization) << "\n";
			}

			server->sendToClientsWithStatus(ClientStatus::SYNCHRONIZED, input);
		}

		if (client && CCFG::getMM()->currentGameState == MenuManager::gameState::eGame) {
			auto response = client->receiveMessage();

			if (!response.empty()) {
				if (client->isSynchronized) {
					firstDir = response["direction"];
					keyAPressed = response["A"];
					keyS = response["S"];
					keyDPressed = response["D"];
					keyShift = response["shift"];
					CCFG::keySpace = response["space"];
				} else {
					auto now = std::chrono::system_clock::now();
					std::cout << "Client: " << std::chrono::system_clock::to_time_t(now) << "\n";
					std::cout << "Synchronization: " << response << " bytes\n";
					from_json(response, CCore::getMap());
					client->isSynchronized = true;
				}
			}
		}

		Update();
		Draw();
		SDL_RenderPresent(rR);
		
		if(SDL_GetTicks() - frameTime < MIN_FRAME_TIME) {
			SDL_Delay(MIN_FRAME_TIME - (SDL_GetTicks () - frameTime));
		}
	}
}

void CCore::Input() {
	switch(CCFG::getMM()->getViewID()) {
		case 2: case 7:
			if(!oMap->getInEvent()) {
				InputPlayer();
			} else {
				resetMove();
			}
			break;
		default:
			InputMenu();
			break;
	}
}

void CCore::InputMenu() {
	if(mainEvent->type == SDL_KEYDOWN) {
		CCFG::getMM()->setKey(mainEvent->key.keysym.sym);

		switch(mainEvent->key.keysym.sym) {
			case SDLK_s: case SDLK_DOWN:
				if(!keyMenuPressed) {
					CCFG::getMM()->keyPressed(SDLK_DOWN); //2
					keyMenuPressed = true;
				}
				break;
			case SDLK_w: case SDLK_UP:
				if(!keyMenuPressed) {
					CCFG::getMM()->keyPressed(SDLK_UP); //0
					keyMenuPressed = true;
				}
				break;
			case SDLK_KP_ENTER: case SDLK_RETURN:
				if(!keyMenuPressed) {
					CCFG::getMM()->enter();
					keyMenuPressed = true;
				}
				break;
			case SDLK_ESCAPE:
				if(!keyMenuPressed) {
					CCFG::getMM()->escape();
					keyMenuPressed = true;
				}
				break;
			case SDLK_LEFT: case SDLK_d:
				if(!keyMenuPressed) {
					CCFG::getMM()->keyPressed(SDLK_LEFT); //3
					keyMenuPressed = true;
				}
				break;
			case SDLK_RIGHT: case SDLK_a:
				if(!keyMenuPressed) {
					CCFG::getMM()->keyPressed(SDLK_RIGHT);//1
					keyMenuPressed = true;
				}
				break;
			default:
				if(mainEvent->key.keysym.mod & KMOD_SHIFT && mainEvent->key.keysym.sym == SDLK_SEMICOLON)
				{
					CCFG::getMM()->keyPressed(SDLK_COLON);
				} else
					CCFG::getMM()->keyPressed(mainEvent->key.keysym.sym);

		}
	}

	if(mainEvent->type == SDL_KEYUP) {
		switch(mainEvent->key.keysym.sym) {
			case SDLK_s: case SDLK_DOWN: case SDLK_w: case SDLK_UP: case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE: case SDLK_a: case SDLK_RIGHT: case SDLK_LEFT: case SDLK_d:
				keyMenuPressed = false;
				break;
			default:
				break;
		}
	}
}

void CCore::InputPlayer() {

	if(mainEvent->type == SDL_KEYUP) {
		if(mainEvent->key.keysym.sym == CCFG::keyIDD) {
				if(firstDir) {
					firstDir = false;
				}

				keyDPressed = false;
			}

			if(mainEvent->key.keysym.sym == CCFG::keyIDS) {
				oMap->getPlayer()->setSquat(false);
				keyS = false;
			}
		
			if(mainEvent->key.keysym.sym == CCFG::keyIDA) {
				if(!firstDir) {
					firstDir = true;
				}

				keyAPressed = false;
			}
		
			if(mainEvent->key.keysym.sym == CCFG::keyIDSpace) {
				CCFG::keySpace = false;
			}
		
			if(mainEvent->key.keysym.sym == CCFG::keyIDShift) {
				if(keyShift) {
					oMap->getPlayer()->resetRun();
					keyShift = false;
				}
			}
		switch(mainEvent->key.keysym.sym) {
			case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE:
				keyMenuPressed = false;
				break;
		}
	}

	if(mainEvent->type == SDL_KEYDOWN) {
		if(mainEvent->key.keysym.sym == CCFG::keyIDD) {
			keyDPressed = true;
			if(!keyAPressed) {
				firstDir = true;
			}
		}

		if(mainEvent->key.keysym.sym == CCFG::keyIDS) {
			if(!keyS) {
				keyS = true;
				if(!oMap->getUnderWater() && !oMap->getPlayer()->getInLevelAnimation()) oMap->getPlayer()->setSquat(true);
			}
		}
		
		if(mainEvent->key.keysym.sym == CCFG::keyIDA) {
			keyAPressed = true;
			if(!keyDPressed) {
				firstDir = false;
			}
		}
		
		if(mainEvent->key.keysym.sym == CCFG::keyIDSpace) {
			if(!CCFG::keySpace) {
				CCFG::keySpace = true;
			}
		}
		
		if(mainEvent->key.keysym.sym == CCFG::keyIDShift) {
			if(!keyShift) {
				keyShift = true;
			}
		}

		switch(mainEvent->key.keysym.sym) {
			case SDLK_KP_ENTER: case SDLK_RETURN:
				if(!keyMenuPressed) {
					CCFG::getMM()->enter();
					keyMenuPressed = true;
				}
			case SDLK_ESCAPE:
				if(!keyMenuPressed && CCFG::getMM()->getViewID() == CCFG::getMM()->eGame) {
					CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePause);
					CCFG::getMM()->setViewID(CCFG::getMM()->ePause);
					CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
					CCFG::getMusic()->PauseMusic();
					keyMenuPressed = true;
				}
				break;
		}
	}

	if(keyAPressed) {
		if(!oMap->getPlayer()->getMove() && firstDir == false && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
			oMap->getPlayer()->startMove();
			oMap->getPlayer()->setMoveDirection(false);
		} else if(!keyDPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
			oMap->getPlayer()->setChangeMoveDirection();
		}
	}

	if(keyDPressed) {
		if(!oMap->getPlayer()->getMove() && firstDir == true && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
			oMap->getPlayer()->startMove();
			oMap->getPlayer()->setMoveDirection(true);
		} else if(!keyAPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
			oMap->getPlayer()->setChangeMoveDirection();
		}
	}

	if (CCFG::keySpace) {
		oMap->getPlayer()->jump();
	}

	if (keyShift) {
		oMap->getPlayer()->startRun();
	}

	if(oMap->getPlayer()->getMove() && !keyAPressed && !keyDPressed) {
		oMap->getPlayer()->resetMove();
	}
}

void CCore::MouseInput() {
	switch(mainEvent->type) {
		case SDL_MOUSEBUTTONDOWN: {
			switch (mainEvent->button.button) {
				case SDL_BUTTON_LEFT:
					mouseLeftPressed = true;
					break;
				case SDL_BUTTON_RIGHT:
					mouseRightPressed = true;
					break;
			}
			break;
		}
		case SDL_MOUSEMOTION: {
			
			SDL_GetMouseState(&mouseX, &mouseY);
			//CCFG::getMM()->getConsole()->print("x:" + std::to_string(mouseX));
			//CCFG::getMM()->getConsole()->print("y:" + std::to_string(mouseY));
			break;
		}
		case SDL_MOUSEBUTTONUP: {
			switch (mainEvent->button.button) {
				case SDL_BUTTON_LEFT:
					mouseLeftPressed = false;
					break;
				case SDL_BUTTON_RIGHT:
					mouseRightPressed = false;
					break;
			}
			break;
		}
		case SDL_MOUSEWHEEL:
			if(mainEvent->wheel.timestamp > SDL_GetTicks() - 2) {
				//CCFG::getMM()->getLE()->mouseWheel(mainEvent->wheel.y);
			}
			break;
	}
}

void CCore::resetKeys() {
	movePressed = keyMenuPressed = keyS = keyW = keyA = keyD = CCFG::keySpace = keyShift = keyAPressed = keyDPressed = false;
}

void CCore::Update() {
	CCFG::getMM()->Update();
}


void CCore::Draw() {
	CCFG::getMM()->Draw(rR);
}

/* ******************************************** */

void CCore::resetMove() {
	this->keyAPressed = this->keyDPressed = false;
}

Map* CCore::getMap() {
	return oMap;
}

void CCore::createServer() {
	server = std::make_shared<Server>();
}

std::shared_ptr<Server> CCore::getServer() {
    return server;
}

void CCore::destroyServer() {
    server.reset();
}

void CCore::createClient(const char *ipv4Address, const uint16_t port) {
	client = std::make_shared<Client>(ipv4Address, port);
}

std::shared_ptr<Client> CCore::getClient() {
	return client;
}
