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

	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.addElement(&_startButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_settingsButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_quitButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_titleTextElement, &Jauntlet::TextRenderer::textShader);
	_uiManager.addElement(&_startButtonText, &Jauntlet::TextRenderer::textShader);
	_uiManager.addElement(&_settingsTextElement, &Jauntlet::TextRenderer::textShader);
	_uiManager.addElement(&_quitTextElement, &Jauntlet::TextRenderer::textShader);
	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void MainMenu::gameLoop() {
	_camera.update();
	_uiManager.draw();

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

SaveInfoElement::SaveInfoElement(float yPos) : 
	_position(20, yPos)
{
	// Empty
}
void SaveInfoElement::addToManager(Jauntlet::UIManager& uiManager) {
	uiManager.addElement(&_background, &GlobalContext::normalShader);
}
void SaveInfoElement::setVisibility(bool visible) {
	_background.visible = visible;
}