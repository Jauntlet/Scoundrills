#pragma once

#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/Vertex.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>

#include "Navigation.h"
#include "drill/DrillManager.h"

class UICoordinator {
public:
	UICoordinator();
	~UICoordinator();

	void init(glm::ivec2 screenSize, Jauntlet::Camera2D* hudCamera, Jauntlet::SpriteFont* spriteFont, Jauntlet::InputManager* inputManager, DrillManager* drillManager);

	void draw();

	void applyNewScreenSize(glm::ivec2 screenSize);

	Navigation navigation;
	std::string fpsText = "0";
private:
	void _addElements();

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
	GLuint _buttonTexture;
	glm::vec2* buttonPos;
	Jauntlet::UIButtonElement* _button;
};