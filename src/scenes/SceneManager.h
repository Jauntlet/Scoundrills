/* Purpose: Controls what parts of the game are loaded at what times, as well as manages the global context
 * Main Contributer(s): Xander Mooney
 */
 #pragma once
 #include "MainGame.h"
 #include "MainMenu.h"

enum class GameState { MAINMENU, MAINGAME };

class SceneManager {
public:
    SceneManager();

    void gameLoop();

    void switchScene(GameState newState);
private:
    GameState _gameState;

    MainGame* _mainGame = nullptr;
    MainMenu* _mainMenu = nullptr;
};