/* Purpose: Controls what parts of the game are loaded at what times, as well as manages the global context
* Main Contributer(s): Xander Mooney
*/
#pragma once
#include "MainGame/MainGame.h"
#include "MainMenu/MainMenu.h"
#include "RogueGallery/RogueGallery.h"

enum class GameState { MAINMENU, MAINGAME, ROGUEGALLERY, QUITTING };

class SceneManager {
public:
    SceneManager();

    void gameLoop();

    void switchScene(GameState newState);
    void loadGame(int ID);
    void loadGame(const std::vector<uint8_t>& playerIDs);

    void quitGame();
private:
    void queuedSwitchScene();
    GameState _gameState, _queuedState;
    int _queuedID;
    std::vector<uint8_t> _storedPlayerIDs;

    MainGame* _mainGame = nullptr;
    MainMenu* _mainMenu = nullptr;
    RogueGallery* _rogueGallery = nullptr;
};