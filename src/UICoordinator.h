#pragma once
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/Vertex.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>

#include "Navigation.h"

class UICoordinator {
public:
	UICoordinator();
	~UICoordinator();

	void init(glm::ivec2 screenSize, Jauntlet::Camera2D* hudCamera, Jauntlet::SpriteFont* spriteFont, Jauntlet::InputManager* inputManager);

	void draw();

	void applyNewScreenSize(glm::ivec2 screenSize);

	Navigation navigation;
	std::string fpsText = "0";
private:
	// Pointers
	Jauntlet::Camera2D* _hudCamera = nullptr;
	Jauntlet::SpriteFont* _spriteFont = nullptr;
	Jauntlet::InputManager* _inputManager = nullptr;

	// UI Manager
	Jauntlet::UIManager _UIManager;
	
	// FPS counter
	Jauntlet::Color _fpsColor;
	glm::vec2 _fpsPosition;
	Jauntlet::UITextElement* _fpsCounter;

	// Debug Button
	//GLuint _buttonTexture = Jauntlet::ResourceManager::getTexture("Textures/button.png").id;
	//glm::vec2* buttonPos = new glm::vec2(10, 10);
	//Jauntlet::UIButtonElement* _button = new Jauntlet::UIButtonElement(_inputManager, _bruh, _buttonTexture, buttonPos, glm::vec2(512, 512), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);
	//_uiManager.addElement(_button);
};