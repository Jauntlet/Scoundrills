/* Purpose: Controls what parts of the game are loaded at what times, as well as manages the global context
 * Main Contributer(s): Xander Mooney
 */
 #pragma once
 #include "MainGame.h"
 #include "MainMenu/MainMenu.h"

enum class GameState { MAINMENU, MAINGAME, QUITTING };

class SceneManager {
public:
    SceneManager();

    void gameLoop();

    void switchScene(GameState newState);
    void loadGame(int ID);

    void quitGame();
private:
    void queuedSwitchScene();
    GameState _gameState, _queuedState;
    int _queuedID;

    MainGame* _mainGame = nullptr;
    MainMenu* _mainMenu = nullptr;
};