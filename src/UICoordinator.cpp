#include <functional>
#include <iostream>
#include <Jauntlet/UI/UIButtonElement.h>

#include "UICoordinator.h"

UICoordinator::UICoordinator() {
	// Empty
}

UICoordinator::~UICoordinator() {
	delete _fpsCounter;
}

void _bruh() {
	std::cout << "bruh" << std::endl;
}


void UICoordinator::init(glm::ivec2 screenSize, Jauntlet::Camera2D* hudCamera, Jauntlet::SpriteFont* spriteFont, Jauntlet::InputManager* inputManager, DrillManager* drillManager) {
	
	_hudCamera = hudCamera;
	_spriteFont = spriteFont;
	_inputManager = inputManager;

	_UIManager = Jauntlet::UIManager(_hudCamera);

	_UIManager.setScale((hudCamera->getCameraSize().y / 1080.0f) * (hudCamera->getCameraSize().x / 1920.0f));
	_UIManager.resolvePositions();

	// TODO: Remove second genNav call
	navigation.genNav(_UIManager, _inputManager, &screenSize.x, &screenSize.y);
	navigation.genNav(_UIManager, _inputManager, &screenSize.x, &screenSize.y);

	_fpsColor = Jauntlet::Color(0, 255, 0, 255);
	_fpsPosition = glm::vec2(0);
	_fpsCounter = new Jauntlet::UITextElement(_spriteFont, &fpsText, &_fpsColor, &_fpsPosition);
	
	_UIManager.addElement(_fpsCounter);

	GLuint _buttonTexture = Jauntlet::ResourceManager::getTexture("Textures/button.png").id;
	glm::vec2* buttonPos = new glm::vec2(10, 10);
	
	// conversion from `void` to `std::function<void ()>` -jk
	std::function<void()> _buttonMethod = std::bind(&DrillManager::toggle, drillManager);
	
	Jauntlet::UIButtonElement* _button = new Jauntlet::UIButtonElement(_inputManager, _buttonMethod, _buttonTexture, buttonPos, glm::vec2(512, 512), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);
		
	_UIManager.addElement(_button);
}

void UICoordinator::draw() {
	_UIManager.draw();
}

void UICoordinator::applyNewScreenSize(glm::ivec2 screenSize) {
	_UIManager.setScale((screenSize.y / 1080.0f) * (screenSize.x / 1920.0f));
	_UIManager.resolvePositions();

	// temporary
	navigation.genNav(_UIManager, _inputManager, &screenSize.x, &screenSize.y);
}