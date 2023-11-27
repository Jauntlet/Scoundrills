#include "MainMenu.h"
#include "GlobalContext.h"
#include "SceneManager.h"
#include <Jauntlet/Rendering/ResourceManager.h>

#include <iostream>

void MainMenu::startGame() {
	std::cout << "BUTTON CLICKED" << std::endl;
	_sceneManager->switchScene(GameState::MAINGAME);
}

MainMenu::MainMenu(SceneManager* sceneManager)
	:
	_camera(GlobalContext::screenSize.x, GlobalContext::screenSize.y),
	_uiManager(&_camera),
	_startButton(&GlobalContext::inputManager, std::bind(&MainMenu::startGame, this), Jauntlet::ResourceManager::getTexture("Textures/button.png").id, &_startButtonPos, glm::vec2(10), Jauntlet::UIElement::ORIGIN_PIN::CENTER),
	_sceneManager(sceneManager)
{
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(255,255,255,255));

	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);

	_uiManager.addElement(&_startButton, &GlobalContext::normalShader);
	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void MainMenu::gameLoop() {
	_uiManager.draw();
}

void MainMenu::windowResized() {
	_camera.updateCameraSize(GlobalContext::screenSize);
	
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.resolvePositions();
}