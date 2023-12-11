#include "MainMenu.h"
#include "GlobalContext.h"
#include "Jauntlet/Time.h"
#include "SceneManager.h"
#include <Jauntlet/Rendering/Textures/ResourceManager.h>

#include <iostream>

void MainMenu::startGame() {
	std::cout << "BUTTON CLICKED" << std::endl;
	_sceneManager->switchScene(GameState::MAINGAME);
}

MainMenu::MainMenu(SceneManager* sceneManager) :
	_camera(GlobalContext::screenSize.x, GlobalContext::screenSize.y),
	_uiManager(&_camera),
	_startButton(&GlobalContext::inputManager, std::bind(&MainMenu::startGame, this), Jauntlet::ResourceManager::getTexture("Textures/Stat button.png").id, &_startButtonPos, glm::vec2(100,100), Jauntlet::UIElement::ORIGIN_PIN::CENTER),
	_sceneManager(sceneManager)
{
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(0,0,255,255));

	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.addElement(&_startButton, &GlobalContext::normalShader);
	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void MainMenu::gameLoop() {
	if (GlobalContext::inputManager.isKeyDown(SDLK_a)) {
		_camera.translate(glm::vec2(10 * Jauntlet::Time::getDeltaTime(),0));
		_uiManager.resolvePositions();
	}
	if (GlobalContext::inputManager.isKeyDown(SDLK_d)) {
		_camera.translate(glm::vec2(-10 * Jauntlet::Time::getDeltaTime(),0));
		_uiManager.resolvePositions();
	}
	if (GlobalContext::inputManager.isKeyDown(SDLK_s)) {
		_camera.translate(glm::vec2(0,10 * Jauntlet::Time::getDeltaTime()));
		_uiManager.resolvePositions();
	}
	if (GlobalContext::inputManager.isKeyDown(SDLK_w)) {
		_camera.translate(glm::vec2(0,-10 * Jauntlet::Time::getDeltaTime()));
		_uiManager.resolvePositions();
	}
	if (GlobalContext::inputManager.isKeyDown(SDLK_r)) {
		_camera.setPosition(glm::vec2(0,0));
		_uiManager.resolvePositions();
	}
	_camera.update();
	_uiManager.draw();
}

void MainMenu::windowResized() {
	_camera.updateCameraSize(GlobalContext::screenSize);
	
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.resolvePositions();
}