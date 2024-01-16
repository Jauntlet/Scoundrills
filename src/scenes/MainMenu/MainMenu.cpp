#include "MainMenu.h"
#include "../GlobalContext.h"
#include "Jauntlet/Rendering/TextRenderer.h"
#include "../SceneManager.h"
#include "../PauseMenu.h"
#include <SDL2/SDL_keycode.h>
#include <iostream>

MainMenu::MainMenu() :
	_camera(GlobalContext::screenSize.x, GlobalContext::screenSize.y),
	_uiManager(&_camera)
{
	GlobalContext::window.setBackgroundColor(Color("6B7973"));
	
	GlobalContext::listener.setPosition(glm::vec3(0, 0, 0));
	_music.playWAV("Sounds/Menu Music.wav", true);

	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.addElement(&_settingsButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_quitButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_noButton, &GlobalContext::normalShader);
	_noButton.visible = false;
	_uiManager.addElement(&_yesButton, &GlobalContext::normalShader);
	_yesButton.visible = false;

	_uiManager.addElement(&_titleTextElement, &TextRenderer::textShader);
	_uiManager.addElement(&_settingsTextElement, &TextRenderer::textShader);
	_uiManager.addElement(&_quitTextElement, &TextRenderer::textShader);
	_uiManager.addElement(&_tutTextElement, &TextRenderer::textShader);
	_tutTextElement.visible = false;
	_uiManager.addElement(&_noTextElement, &TextRenderer::textShader);
	_noTextElement.visible = false;
	_uiManager.addElement(&_yesTextElement, &TextRenderer::textShader);
	_yesTextElement.visible = false;

	for (int i = 0; i < 3; ++i) {
		_saveInfoElements[i].addToManager(_uiManager);
		_saveInfoElements[i].setVisibility(false);
	}
	_uiManager.addElement(&_startButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_startButtonText, &TextRenderer::textShader);

	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void MainMenu::gameLoop() {
	_camera.update();
	_uiManager.draw();
	_music.update();

	if (GlobalContext::inputManager.isKeyPressed(SDLK_ESCAPE) && GlobalContext::pauseMenu->inSettings()) {
		toggleSettingsMenu();
	}
	if (GlobalContext::inputManager.isKeyPressed(SDLK_ESCAPE) && !GlobalContext::pauseMenu->isPaused() && _startButton.visible == false) {
		toggleSavesMenu();
	}
}

void MainMenu::windowResized() {
	_camera.updateSize(GlobalContext::screenSize);
	
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.resolvePositions();
}

void MainMenu::promptTutorial(int saveID) {
	_saveID = saveID;

	_uiManager.hideAllElements();

	_tutTextElement.visible = true;
	_noButton.visible = true;
	_noTextElement.visible = true;
	_yesButton.visible = true;
	_yesTextElement.visible = true;
}

void MainMenu::startSavedGame(int ID) {
	GlobalContext::sceneManager->startGame(ID);
}
void MainMenu::startGame() {
	GlobalContext::sceneManager->loadRoguesGallery(_saveID, false);
}

void MainMenu::startTutorial() {
	GlobalContext::sceneManager->loadRoguesGallery(_saveID, true);
}

void MainMenu::quitGame() {
	GlobalContext::sceneManager->quitGame();
}

void MainMenu::toggleSettingsMenu() {
	GlobalContext::pauseMenu->togglePauseMenu();

	if (!_startButton.visible) {
		_uiManager.hideAllElements();
		_startButton.visible = true;
		_startButtonText.visible = true;
		_settingsButton.visible = true;
		_settingsTextElement.visible = true;
		_quitButton.visible = true;
		_quitTextElement.visible = true;
		_titleTextElement.visible = true;
	} else {
		_uiManager.hideAllElements();
	}

}

void MainMenu::toggleSavesMenu() {
	if (_startButton.visible) {
		_uiManager.hideAllElements();
		for (int i = 0; i < 3; ++i) {
			_saveInfoElements[i].setVisibility(true);
		}
	}
	else {
		_uiManager.hideAllElements();
		_startButton.visible = true;
		_startButtonText.visible = true;
		_settingsButton.visible = true;
		_settingsTextElement.visible = true;
		_quitButton.visible = true;
		_quitTextElement.visible = true;
		_titleTextElement.visible = true;
	}
}