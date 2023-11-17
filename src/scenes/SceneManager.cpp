#include "SceneManager.h"
#include <Jauntlet/Time.h>
#include "GlobalContext.h"
#include "MainGame.h"

SceneManager::SceneManager() {
    Jauntlet::init();
    Jauntlet::ResourceManager::setMissingTexture("Textures/missing.png");
    
    GlobalContext::initContext();
    
    // Start at specified scene; for final builds it should be GameState::MAINMENU
    switchScene(GameState::MAINGAME);

    gameLoop();
}

void SceneManager::gameLoop() {
    Jauntlet::Time::setMaxFPS(60);

    while (true) {
        Jauntlet::Time::beginFrame();
        
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
            }
        }

        if (_gameState == GameState::MAINGAME) {
            _mainGame->gameLoop();
        }

        Jauntlet::Time::endFrame();
    }
}

void SceneManager::switchScene(GameState newState) {
    _gameState = newState;

    // Toggle state of MainGame
    if (_gameState == GameState::MAINGAME) {
        if (_mainGame == nullptr) {
                _mainGame = new MainGame();
        }
    } else if (_mainGame != nullptr) {
            delete _mainGame;
    }
}