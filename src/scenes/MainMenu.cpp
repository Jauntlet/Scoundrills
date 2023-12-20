#include "MainMenu.h"
#include "GlobalContext.h"
#include "Jauntlet/Rendering/TextRenderer.h"
#include "SceneManager.h"
#include "PauseMenu.h"
#include <SDL2/SDL_keycode.h>

MainMenu::MainMenu(SceneManager* sceneManager) :
	_camera(GlobalContext::screenSize.x, GlobalContext::screenSize.y),
	_uiManager(&_camera),
	_sceneManager(sceneManager)
{
	GlobalContext::window.setBackgroundColor(Jauntlet::Color("6B7973"));
	
	GlobalContext::listener.setPosition(glm::vec3(0, 0, 0));
	_music.playWAV("Sounds/Menu Music.wav", true);

	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.addElement(&_settingsButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_quitButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_titleTextElement, &Jauntlet::TextRenderer::textShader);
	_uiManager.addElement(&_settingsTextElement, &Jauntlet::TextRenderer::textShader);
	_uiManager.addElement(&_quitTextElement, &Jauntlet::TextRenderer::textShader);

	for (int i = 0; i < 3; ++i) {
		_saveInfoElements[i].addToManager(_uiManager);
		_saveInfoElements[i].setVisibility(false);
	}
	_uiManager.addElement(&_startButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_startButtonText, &Jauntlet::TextRenderer::textShader);

	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void MainMenu::gameLoop() {
	_camera.update();
	_uiManager.draw();
	_music.update();

	if (_switch) {
		_sceneManager->switchScene(GameState::MAINGAME);
	}
	else if (GlobalContext::inputManager.isKeyPressed(SDLK_ESCAPE) && GlobalContext::pauseMenu->inSettings()) {
		toggleSettingsMenu();
	}
}

void MainMenu::windowResized() {
	_camera.updateCameraSize(GlobalContext::screenSize);
	
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.resolvePositions();
}

void MainMenu::startGame() {
	_switch = true;
}
void MainMenu::quitGame() {
	_sceneManager->quitGame();
}

void MainMenu::toggleSettingsMenu() {
	GlobalContext::pauseMenu->togglePauseMenu();

	if (_startButton.visible) {
		_startButton.visible = false;
		_startButtonText.visible = false;
		_settingsButton.visible = false;
		_settingsTextElement.visible = false;
		_quitButton.visible = false;
		_quitTextElement.visible = false;
		_titleTextElement.visible = false;
		for (int i = 0; i < 3; ++i) {
			_saveInfoElements[i].setVisibility(false);
		}
	} else {
		_startButton.visible = true;
		_startButtonText.visible = true;
		_settingsButton.visible = true;
		_settingsTextElement.visible = true;
		_quitButton.visible = true;
		_quitTextElement.visible = true;
		_titleTextElement.visible = true;
	}
}

void MainMenu::toggleSavesMenu() {
	if (_startButton.visible) {
		_startButton.visible = false;
		_startButtonText.visible = false;
		_settingsButton.visible = false;
		_settingsTextElement.visible = false;
		_quitButton.visible = false;
		_quitTextElement.visible = false;
		_titleTextElement.visible = false;

		for (int i = 0; i < 3; ++i) {
			_saveInfoElements[i].setVisibility(true);
		}
	}
	else {
		_startButton.visible = true;
		_startButtonText.visible = true;
		_settingsButton.visible = true;
		_settingsTextElement.visible = true;
		_quitButton.visible = true;
		_quitTextElement.visible = true;
		_titleTextElement.visible = true;

		for (int i = 0; i < 3; ++i) {
			_saveInfoElements[i].setVisibility(false);
		}
	}
}