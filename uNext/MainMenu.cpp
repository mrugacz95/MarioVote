#include "MainMenu.h"
#include "CFG.h"
#include "Core.h"
#include <unistd.h>
/* ******************************************** */

MainMenu::MainMenu(void) {
    std::vector<std::string> menuStrings = {"JOIN", "HOST SERVER", "OPTIONS", "ABOUT", "EXIT"};
    for (int i = 0; i < menuStrings.size(); i++) {
        this->lMO.push_back(new MenuOption(menuStrings[i], 178, 250 + 25 * i));
    }

    this->numOfMenuOptions = (int) lMO.size();

    this->selectWorld = false;

    rSelectWorld.x = 122;
    rSelectWorld.y = 280;
    rSelectWorld.w = 306;
    rSelectWorld.h = 72;

    this->selectSerwerAddr = false;

    rSelectServerAddr.x = 122;
    rSelectServerAddr.y = 280;
    rSelectServerAddr.w = 612;
    rSelectServerAddr.h = 72;

    this->activeWorldID = this->activeSecondWorldID = 0;
}

MainMenu::~MainMenu(void) {

}

/* ******************************************** */

void MainMenu::Update() {
    CCFG::getMusic()->StopMusic();

    Menu::Update();
}

void MainMenu::Draw(SDL_Renderer *rR) {
    CCFG::getSMBLOGO()->Draw(rR, 80, 48);
    Menu::Draw(rR);
    std::string names = "MARCIN MRUGAS I JAKUB NURSKI";
    CCFG::getText()->Draw(rR, names, 4, CCFG::GAME_HEIGHT - 4 - 8, 8, 0, 0, 0);
    CCFG::getText()->Draw(rR, names, 5, CCFG::GAME_HEIGHT - 5 - 8, 8, 255, 255, 255);

    if (selectWorld) {
        SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(rR, 4, 4, 4, 235);
        SDL_RenderFillRect(rR, &rSelectWorld);
        SDL_SetRenderDrawColor(rR, 255, 255, 255, 255);
        rSelectWorld.x += 1;
        rSelectWorld.y += 1;
        rSelectWorld.h -= 2;
        rSelectWorld.w -= 2;
        SDL_RenderDrawRect(rR, &rSelectWorld);
        rSelectWorld.x -= 1;
        rSelectWorld.y -= 1;
        rSelectWorld.h += 2;
        rSelectWorld.w += 2;

        CCFG::getText()->Draw(rR, "SELECT WORLD",
                              rSelectWorld.x + rSelectWorld.w / 2 - CCFG::getText()->getTextWidth("SELECT WORLD") / 2,
                              rSelectWorld.y + 16, 16, 255, 255, 255);

        for (int i = 0, extraX = 0; i < 8; i++) {
            if (i == activeWorldID) {
                CCFG::getText()->Draw(rR, std::to_string(i + 1) + "-" + std::to_string(activeSecondWorldID + 1),
                                      rSelectWorld.x + 16 * (i + 1) + 16 * i + extraX, rSelectWorld.y + 16 + 24, 16,
                                      255, 255, 255);

                extraX = 32;

            } else {
                CCFG::getText()->Draw(rR, std::to_string(i + 1), rSelectWorld.x + 16 * (i + 1) + 16 * i + extraX,
                                      rSelectWorld.y + 16 + 24, 16, 90, 90, 90);
            }
        }

        SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_NONE);
        CCore::getMap()->setBackgroundColor(rR);
    } else if (selectSerwerAddr) {
        SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(rR, 4, 4, 4, 235);
        SDL_RenderFillRect(rR, &rSelectServerAddr);
        SDL_SetRenderDrawColor(rR, 255, 255, 255, 255);
        rSelectServerAddr.x += 1;
        rSelectServerAddr.y += 1;
        rSelectServerAddr.h -= 2;
        rSelectServerAddr.w -= 2;
        SDL_RenderDrawRect(rR, &rSelectServerAddr);
        rSelectServerAddr.x -= 1;
        rSelectServerAddr.y -= 1;
        rSelectServerAddr.h += 2;
        rSelectServerAddr.w += 2;
        int writeAddrPos = rSelectServerAddr.x + rSelectServerAddr.w / 2 -
                           CCFG::getText()->getTextWidth("WRITE SERVER IP/ADDRESS:PORT:") / 2;
        CCFG::getText()->Draw(rR, "WRITE SERVER IP/ADDRESS:PORT:",
                              writeAddrPos,
                              rSelectServerAddr.y + 16, 16, 255, 255, 255);

        std::string serverInput = serverAddr + ((SDL_GetTicks() % 2000 < 1000) ? "<" : "");
        CCFG::getText()->Draw(rR, serverInput,
                              writeAddrPos,
                              rSelectServerAddr.y + 16 + 24, 16, 255, 255, 255);
    }
}

/* ******************************************** */

void MainMenu::enter() {
    switch (activeMenuOption) {
        case 0:
            if (!selectSerwerAddr)
                selectSerwerAddr = true;
            break;
        case 1:
            if (!selectWorld) {
                selectWorld = true;
            } else {
                CCFG::getMM()->getLoadingMenu()->updateTime();
                CCore::getMap()->resetGameData();
                CCore::getMap()->setCurrentLevelID(activeWorldID * 4 + activeSecondWorldID);
                CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
                CCFG::getMM()->getLoadingMenu()->loadingType = true;
                CCore::getMap()->setSpawnPointID(0);
                selectWorld = false;
            }
            break;
        case 2:
            CCFG::getMM()->getOptions()->setEscapeToMainMenu(true);
            CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->eOptions);
            CCFG::getMM()->getOptions()->updateVolumeRect();
            CCFG::getMM()->setViewID(CCFG::getMM()->eOptions);
            break;
        case 3:
            CCFG::getMM()->getAboutMenu()->updateTime();
            CCFG::getMM()->setViewID(CCFG::getMM()->eAbout);
            CCFG::getMusic()->PlayMusic(CCFG::getMusic()->mOVERWORLD);
            break;
        default:
            break;
    }
}

void MainMenu::escape() {
    if (selectWorld || selectSerwerAddr) {
        selectWorld = false;
        selectSerwerAddr = false;
    } else
        CCore::quitGame = true;

}

void MainMenu::updateActiveButton(int iDir) {
    switch (iDir) {
        case SDLK_UP:
        case SDLK_DOWN:
            if (!selectWorld) {
                Menu::updateActiveButton(iDir);
            } else {
                switch (iDir) {
                    case SDLK_UP:
                        if (activeSecondWorldID < 1) {
                            activeSecondWorldID = 3;
                        } else {
                            --activeSecondWorldID;
                        }
                        break;
                    case SDLK_DOWN:
                        if (activeSecondWorldID > 2) {
                            activeSecondWorldID = 0;
                        } else {
                            ++activeSecondWorldID;
                        }
                        break;
                }
            }
            break;
        case SDLK_RIGHT:
            if (selectWorld) {
                if (activeWorldID < 7) {
                    ++activeWorldID;
                } else {
                    activeWorldID = 0;
                }
            }
            break;
        case SDLK_LEFT:
            if (selectWorld) {
                if (activeWorldID > 0) {
                    --activeWorldID;
                } else {
                    activeWorldID = 7;
                }
            }
            break;
        case SDLK_BACKSPACE:
            if (serverAddr.size() > 0 && SDL_GetTicks() - lastBackSpace>100) {
            serverAddr.pop_back();
            lastBackSpace = SDL_GetTicks();
            }
            break;
        default:
            if (iDir >= SDLK_0 && iDir <= SDLK_9)
                serverAddr+=(char)iDir;
            else if (iDir >= SDLK_KP_1 && iDir <= SDLK_KP_0)
                if(iDir == SDLK_KP_0)
                    serverAddr+='0';
                else
                    serverAddr+=iDir - SDLK_KP_1 + '0'+1;
            else if (iDir == SDLK_COLON)
                serverAddr+=':';
            else if(iDir==SDLK_PERIOD){
                serverAddr+='.';
            }
            usleep(150);
            break;
    }
}