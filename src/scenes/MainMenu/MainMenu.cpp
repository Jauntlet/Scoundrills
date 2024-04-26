#include "MainMenu.h"
#include "../GlobalContext.h"
#include "Jauntlet/Inputs/InputManager.h"
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
	_settingsButton.setButtons(nullptr, nullptr, &_quitButton, &_startButton);
	_uiManager.addElement(&_quitButton, &GlobalContext::normalShader);
	_quitButton.setButtons(nullptr, nullptr, &_startButton, &_settingsButton);
	_uiManager.addElement(&_noButton, &GlobalContext::normalShader);
	_noButton.setButtons(&_yesButton, &_yesButton, nullptr, nullptr);
	_noButton.visible = false;
	_uiManager.addElement(&_yesButton, &GlobalContext::normalShader);
	_yesButton.setButtons(&_noButton, &_noButton, nullptr, nullptr);
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
	_startButton.setButtons(nullptr, nullptr, &_settingsButton, &_quitButton);
	_uiManager.addElement(&_startButtonText, &TextRenderer::textShader);

	_uiManager.optimize();
	_uiManager.resolvePositions();

	_selectedButton = &_startButton;
	_startButton.highlighted = true;
}

void MainMenu::gameLoop() {
	_camera.update();
	_uiManager.draw();
	_music.update();

	if ((GlobalContext::inputManager.isKeyPressed(SDLK_ESCAPE) || GlobalContext::inputManager.isKeyPressed(CONTROLLER_FACE_SOUTH)) && GlobalContext::pauseMenu->inSettings()) {
		toggleSettingsMenu();
	}
	if ((GlobalContext::inputManager.isKeyPressed(SDLK_ESCAPE) || GlobalContext::inputManager.isKeyPressed(CONTROLLER_FACE_SOUTH)) && !GlobalContext::pauseMenu->isPaused() && _startButton.visible == false) {
		toggleSavesMenu();
	}

	glm::vec2 controllerAxis = GlobalContext::inputManager.getControllerAxis(Axis::LeftStick);

	if (abs(controllerAxis.x) > 0.5 || abs(controllerAxis.y) > 0.5) {
		if (_moving) {
			return;
		}
		if (_selectedButton == nullptr) {
			if (!GlobalContext::pauseMenu->isPaused()) {
				_selectedButton = &_startButton;
				_moving = true;
			}
			return;
		}
		if (controllerAxis.x > 0.5) {
			_selectedButton = _selectedButton->selectRightButton();
		} else if (controllerAxis.x < -0.5) {
			_selectedButton = _selectedButton->selectLeftButton();
		}
		if (controllerAxis.y > 0.5) {
			_selectedButton = _selectedButton->selectDownButton();
		} else if (controllerAxis.y < -0.5) {
			_selectedButton = _selectedButton->selectUpButton();
		}
		_moving = true;
	} else {
		_moving = false;
	}

	if (GlobalContext::inputManager.isKeyPressed(CONTROLLER_FACE_EAST)) {
		if (_selectedButton != nullptr) {
			_selectedButton->click();
		}
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

	if (_selectedButton != nullptr) {
		_selectedButton->highlighted = false;
	}
	_yesButton.select();
	_selectedButton = &_yesButton;
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
	GlobalContext::pauseMenu->toggleSettingsMenu();

	if (!_startButton.visible) {
		_uiManager.hideAllElements();
		_startButton.visible = true;
		_startButtonText.visible = true;
		_settingsButton.visible = true;
		_settingsTextElement.visible = true;
		_quitButton.visible = true;
		_quitTextElement.visible = true;
		_titleTextElement.visible = true;
		
		if (_selectedButton != nullptr) {
			_selectedButton->highlighted = false;
		}
		_settingsButton.select();
		_selectedButton = &_settingsButton;

	} else {
		_uiManager.hideAllElements();
		if (_selectedButton != nullptr) {
		 	_selectedButton->highlighted = false;
		 	_selectedButton = nullptr;
		} 
	}

}

void MainMenu::toggleSavesMenu() {
	if (_startButton.visible) {
		_uiManager.hideAllElements();
		for (int i = 0; i < 3; ++i) {
			_saveInfoElements[i].setVisibility(true);
		}
		_selectedButton = _saveInfoElements[0].select();
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

		if (_selectedButton != nullptr) {
			_selectedButton->highlighted = false;
		}
		_selectedButton = &_startButton;
		_startButton.select();
	}
}