#include "UICoordinator.h"

UICoordinator::UICoordinator() {
	// Empty
}

UICoordinator::~UICoordinator() {
	delete _fpsCounter;
}

void UICoordinator::init(glm::ivec2 screenSize, Jauntlet::Camera2D* hudCamera, Jauntlet::SpriteFont* spriteFont, Jauntlet::InputManager* inputManager) {
	
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
}

void UICoordinator::draw() {
	_UIManager.draw();
}

void UICoordinator::applyNewScreenSize(glm::ivec2 screenSize) {
	_UIManager.resolvePositions();
	_UIManager.setScale((screenSize.y / 1080.0f) * (screenSize.x / 1920.0f));

	// temporary
	navigation.genNav(_UIManager, _inputManager, &screenSize.x, &screenSize.y);
}