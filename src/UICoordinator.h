/* Purpose: Handles UIManagers and the elements within as to keep them better organized.
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */

#pragma once

#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/UI/UIButtonToggleableElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>

#include "Navigation.h"
#include "drill/DrillManager.h"

class UICoordinator {
public:
	UICoordinator(Jauntlet::Camera2D* hudCamera, Jauntlet::SpriteFont* spriteFont, Jauntlet::InputManager* inputManager, DrillManager* drillManager);
	~UICoordinator();

	void draw();

	void applyNewScreenSize(glm::ivec2 screenSize);

	// toggles debug UI options
	void toggleDebugMode();
	// toggle debug UI options to selected value
	void toggleDebugMode(bool debugging);
	
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
	bool _debugging = true;
	Jauntlet::Color _fpsColor;
	glm::vec2 _fpsPosition;
	Jauntlet::UITextElement* _fpsCounter;

	// Debug Button
	GLuint _buttonTexture;
	glm::vec2* buttonPos;
	Jauntlet::UIButtonToggleableElement* _button;
};