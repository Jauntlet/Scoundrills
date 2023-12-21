#include "SceneManager.h"
#include <Jauntlet/Time.h>
#include "GlobalContext.h"
#include "MainGame.h"
#include "PauseMenu.h"

SceneManager::SceneManager() {
    Jauntlet::init();
    Jauntlet::ResourceManager::setMissingTexture("Textures/missing.png");
    
    GlobalContext::initContext();
    GlobalContext::pauseMenu = new PauseMenu(this);
    queuedSwitchScene();
    // Start at specified scene
#if NDEBUG
    // do NOT change, release builds always build to main menu.
    switchScene(GameState::MAINMENU);
#else
    // Feel free to change this to whatever you need for debugging. This will not compile on release builds.
    switchScene(GameState::MAINMENU);
    //switchScene(GameState::MAINGAME);
#endif
    gameLoop();
}

void SceneManager::gameLoop() {
    Jauntlet::Time::setMaxFPS(-1);

    // This is the loop that plays every frame in the game.
    while (_gameState != GameState::QUITTING) {
        Jauntlet::Time::beginFrame();
        GlobalContext::window.clearScreen();

        if (_gameState != _queuedState) {
            queuedSwitchScene();
        }

        GlobalContext::inputManager.processInput();

        if (GlobalContext::inputManager.quitGameCalled()) {
            break;
        }

        if (GlobalContext::inputManager.windowResized()) {
            GlobalContext::window.resolveWindowSize();
            GlobalContext::screenSize = GlobalContext::window.getWindowSize();
            
            if (_gameState == GameState::MAINGAME) {
                // run maingame window resize event.
                _mainGame->windowResized();
            } else if (_gameState == GameState::MAINMENU) {
                _mainMenu->windowResized();
            }

            GlobalContext::pauseMenu->windowResized();
        }

        if (_gameState == GameState::MAINGAME) {
            _mainGame->gameLoop();
        } else if (_gameState == GameState::MAINMENU) {
            _mainMenu->gameLoop();
        }
        GlobalContext::pauseMenu->update();
        GlobalContext::pauseMenu->draw();

        GlobalContext::window.swapBuffer();
        Jauntlet::Time::endFrame();
    }
    // Game loop is over, destroy leftover data
    GlobalContext::destroyContext();
}

void SceneManager::switchScene(GameState newState) {
    _queuedState = newState;
}
void SceneManager::loadGame(int ID) {
    _queuedState = GameState::MAINGAME;
    _queuedID = ID;
}

void SceneManager::quitGame() {
    _gameState = GameState::QUITTING;
}

void SceneManager::queuedSwitchScene() {
    _gameState = _queuedState;

    GlobalContext::pauseMenu->hideAll();

    // Toggle state of MainGame
    if (_gameState == GameState::MAINGAME) {
        if (_mainGame == nullptr) {
            if (_queuedID != 0) {
                _mainGame = new MainGame(_queuedID);
                _queuedID = 0;
            } else {
                _mainGame = new MainGame();
            }
        }
    } else if (_mainGame != nullptr) {
        delete _mainGame;
        _mainGame = nullptr;
    }

    if (_gameState == GameState::MAINMENU) {
        if (_mainMenu == nullptr) {
            _mainMenu = new MainMenu(this);
        }
    } else if (_mainMenu != nullptr) {
        delete _mainMenu;
        _mainMenu = nullptr;
    }
}