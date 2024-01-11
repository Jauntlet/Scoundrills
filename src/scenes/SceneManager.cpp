#include <Jauntlet/Time.h>
#include <iostream>

#include "GlobalContext.h"
#include "Jauntlet/Errors.h"
#include "PauseMenu.h"
#include "SceneManager.h"

SceneManager::SceneManager() {
    Jauntlet::ResourceManager::setMissingTexture("Textures/missing.png");
    
    GlobalContext::pauseMenu = new PauseMenu();
    queuedSwitchScene();

    // Start at specified scene, for builds it should be GameState::MAINMENU
    switchScene(GameState::MAINMENU);
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
            
            switch (_gameState) {
                case (GameState::MAINGAME):
                    _mainGame->windowResized();
                    break;
                case (GameState::MAINMENU):
                    _mainMenu->windowResized();
                    break;
                case (GameState::ROGUEGALLERY):
                    _rogueGallery->windowResized();
                    break;
                case (GameState::TUTORIAL):
                    _tutorial->windowResized();
                    break;
                default:
                    Jauntlet::fatalError("im an error that wont exist created to get clangd off my back");
                    break;
            }

            GlobalContext::pauseMenu->windowResized();
        }

        switch (_gameState) {
            case (GameState::MAINGAME):
                _mainGame->gameLoop();
                break;
            case (GameState::MAINMENU):
                _mainMenu->gameLoop();
                break;
            case (GameState::ROGUEGALLERY):
                _rogueGallery->gameLoop();
                break;
            case (GameState::TUTORIAL):
                _tutorial->gameLoop();
                break;
            default:
                Jauntlet::fatalError("im an error that wont exist created to get clangd off my back");
                break;
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
void SceneManager::startGame(int saveID) {
    _queuedState = GameState::MAINGAME;
    _saveID = saveID;
    _queuedID = _saveID;
}
void SceneManager::startGame(int saveID, const std::vector<uint8_t>& playerIDs) {
    _saveID = saveID;
    _queuedState = GameState::MAINGAME;
    _storedPlayerIDs = playerIDs;
}
void SceneManager::loadTutorial(int saveID, const std::vector<uint8_t>& playerIDs) {
    _saveID = saveID;
    _queuedState = GameState::TUTORIAL;
    _storedPlayerIDs = playerIDs;
}
void SceneManager::loadRoguesGallery(int saveID, bool tutorialMode) {
    _saveID = saveID;
    _queuedState = GameState::ROGUEGALLERY;
    _queuedID = tutorialMode;
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
                _mainGame = new MainGame(_saveID);
                _queuedID = 0;
            }
            else if (!_storedPlayerIDs.empty()) {
                _mainGame = new MainGame(_saveID, _storedPlayerIDs);
                _storedPlayerIDs.clear();
            } else {
                Jauntlet::fatalError("MainGame was loaded in an invalid way! Try startGame()!");
            }
        }
    } else if (_mainGame != nullptr) {
        delete _mainGame;
        _mainGame = nullptr;
    }

    if (_gameState == GameState::MAINMENU) {
        if (_mainMenu == nullptr) {
            _mainMenu = new MainMenu();
        }
    } else if (_mainMenu != nullptr) {
        delete _mainMenu;
        _mainMenu = nullptr;
    }

    if (_gameState == GameState::ROGUEGALLERY) {
        if (_rogueGallery == nullptr) {
            _rogueGallery = new RogueGallery(_saveID, _queuedID);
            _queuedID = 0;
        }
    } else if (_rogueGallery != nullptr) {
        delete _rogueGallery;
        _rogueGallery = nullptr;
    }

    if (_gameState == GameState::TUTORIAL) {
        if (_tutorial == nullptr) {
            if (!_storedPlayerIDs.empty()) {
                _tutorial = new Tutorial(_saveID, _storedPlayerIDs);
                _storedPlayerIDs.clear();
            }
        } else {
            Jauntlet::fatalError("Tutorial was loaded in an invalid way! Try loadTutorial()!");
        }
    } else if (_tutorial != nullptr) {
        delete _tutorial;
        _tutorial = nullptr;
    }
}