#include <SDL2/SDL_keycode.h>

#include "Jauntlet/Inputs/InputManager.h"
#include "Jauntlet/Rendering/Textures/ResourceManager.h"
#include "Jauntlet/Time.h"
#include "Jauntlet/UI/UIElement.h"
#include "PauseMenu.h"
#include "src/scenes/GlobalContext.h"
#include "src/scenes/SceneManager.h"

PauseMenu::PauseMenu() :
	_resumeButton(&GlobalContext::inputManager, std::bind(&PauseMenu::switchState, this, PauseState::HIDDEN), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_resumeButtonPos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::CENTER),
	_settingsButton(&GlobalContext::inputManager, std::bind(&PauseMenu::switchState, this, PauseState::SETTINGS), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_settingsButtonPos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::CENTER),
	_quitButton(&GlobalContext::inputManager, std::bind(&PauseMenu::toMainMenu, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_quitButtonPos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::CENTER),
	_resumeTextElement(GlobalContext::textRenderer, &_resumeText, &_buttonTextColor, &_resumeButtonPos, UIElement::ORIGIN_PIN::CENTER, 0.5f),
	_settingsTextElement(GlobalContext::textRenderer, &_settingsText, &_buttonTextColor, &_settingsButtonPos, UIElement::ORIGIN_PIN::CENTER, 0.5f),
	_quitTextElement(GlobalContext::textRenderer, &_quitText, &_buttonTextColor, &_quitButtonPos, UIElement::ORIGIN_PIN::CENTER, 0.5f),
	_fullscreenButton(&GlobalContext::inputManager, std::bind(&PauseMenu::toggleFullscreen, this), Jauntlet::ResourceManager::getTexture("Textures/CheckBox.png").id, &_fullscreenButtonPos, glm::vec2(180), UIElement::ORIGIN_PIN::TOP_LEFT),
	_fullscreenTextElement(GlobalContext::textRenderer, &_fullscreenText, &_textColor, &_fullscreenTextPos, UIElement::ORIGIN_PIN::TOP_LEFT, 0.5f)
{
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);

	// Pause Menu
	_uiManager.addElement(&_resumeButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_settingsButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_quitButton, &GlobalContext::normalShader);
	
	_resumeButton.setButtons(nullptr, nullptr, &_settingsButton, &_quitButton);
	_settingsButton.setButtons(nullptr, nullptr, &_quitButton, &_resumeButton);
	_quitButton.setButtons(nullptr, nullptr, &_resumeButton, &_settingsButton);

	_uiManager.addElement(&_fullscreenButton, &GlobalContext::normalShader);
	
	_uiManager.addElement(&_resumeTextElement, &TextRenderer::textShader);
	_uiManager.addElement(&_settingsTextElement, &TextRenderer::textShader);
	_uiManager.addElement(&_quitTextElement, &TextRenderer::textShader);
	_uiManager.addElement(&_settingsTitle,&TextRenderer::textShader);
	
	_uiManager.addElement(&_fullscreenTextElement, &TextRenderer::textShader);
	
	_uiManager.resolvePositions();
	_uiManager.optimize();
}

void PauseMenu::update() {
	if (_quitting) {
		GlobalContext::sceneManager->switchScene(GameState::MAINMENU);
		_quitting = false;
	}

	if (_state != PauseState::HIDDEN) {
		glm::vec2 controllerAxis = GlobalContext::inputManager.getControllerAxis(Axis::LeftStick);

		if (abs(controllerAxis.x) > 0.5 || abs(controllerAxis.y) > 0.5) {
			if (_moving) {
				return;
			}
			if (_selectedButton == nullptr) {
				_selectedButton = &_resumeButton;
				_moving = true;
				return;
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

		if (!_moving && GlobalContext::inputManager.isKeyPressed(CONTROLLER_FACE_EAST)) {
			if (_state == PauseState::PAUSED) {
				if (_selectedButton != nullptr) {
					_selectedButton->click();
				}
			} else if (_state == PauseState::SETTINGS) {
				_fullscreenButton.click();
			}
		}
		if (GlobalContext::inputManager.isKeyPressed(CONTROLLER_FACE_SOUTH)) {
			if (_state == PauseState::PAUSED) {
				togglePauseMenu();
			} else if (_state == PauseState::SETTINGS) {
				switchState(PauseState::PAUSED);
			}
		}


	}

	_camera.update();
}
void PauseMenu::draw() {
	_camera.setActive();
	_uiManager.draw();
}

bool PauseMenu::isPaused() const {
	return _state != PauseState::HIDDEN;
}
bool PauseMenu::inSettings() const {
	return _state == PauseState::SETTINGS;
}

void PauseMenu::togglePauseMenu() {
	switchState(_state == PauseState::HIDDEN ? PauseState::PAUSED : PauseState::HIDDEN);
}

void PauseMenu::toggleSettingsMenu() {
	switchState(_state == PauseState::SETTINGS ? PauseState::PAUSED : PauseState::SETTINGS);
}

void PauseMenu::hideAll() {
	_resumeButton.visible = false;
	_resumeTextElement.visible = false;
	_settingsButton.visible = false;
	_settingsTextElement.visible = false;
	_quitButton.visible = false;
	_quitTextElement.visible = false;
	_fullscreenButton.visible = false;
	_fullscreenTextElement.visible = false;
	_settingsTitle.visible = false;
}

void PauseMenu::windowResized() {
	_camera.updateSize(GlobalContext::screenSize);
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.resolvePositions();
}

void PauseMenu::switchState(PauseState state) {
	if (state == _state) {
		return;
	} else {
		_state = state;
	}

	hideAll();

	if (_state == PauseState::PAUSED) {
		_resumeButton.visible = true;
		_resumeTextElement.visible = true;
		_settingsButton.visible = true;
		_settingsTextElement.visible = true;
		_quitButton.visible = true;
		_quitTextElement.visible = true;
		
		_selectedButton = &_resumeButton;
		_resumeButton.select();
	} else if (_state == PauseState::SETTINGS) {
		_fullscreenButton.visible = true;
		_fullscreenTextElement.visible = true;
		_settingsTitle.visible = true;

		_selectedButton = nullptr;
	}
}

void PauseMenu::toMainMenu() {
	_quitting = true;
	Jauntlet::Time::setTimeScale(1.0f);
	switchState(PauseState::HIDDEN);
}
void PauseMenu::toggleFullscreen() {
	GlobalContext::window.toggleFullscreen();
}