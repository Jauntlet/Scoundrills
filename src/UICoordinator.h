/* Purpose: Handles UIManagers and the elements within as to keep them better organized.
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */

#pragma once

#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIButtonToggleableElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>

#include "Navigation.h"
#include "drill/DrillManager.h"

class UICoordinator {
public:
	UICoordinator(Jauntlet::Camera2D* hudCamera, Jauntlet::TextRenderer* spriteFont, Jauntlet::InputManager* inputManager, DrillManager* drillManager, Jauntlet::GLSLProgram* textProgram, Jauntlet::GLSLProgram* buttonProgram);
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
	// Color programs
	Jauntlet::GLSLProgram* _colorProgram;

	// Pointers
	Jauntlet::Camera2D* _hudCamera = nullptr;
	Jauntlet::TextRenderer* _textRenderer = nullptr;
	Jauntlet::InputManager* _inputManager = nullptr;

	// UI Manager
	Jauntlet::UIManager _UIManager;
	
	// FPS counter
	bool _debugging = true;
	Jauntlet::Color _fpsColor;
	glm::vec2 _fpsPosition;
	Jauntlet::UITextElement* _fpsCounter;

	// Debug Button
	GLuint _buttonTexture = 0;
	glm::vec2* buttonPos = nullptr;
	Jauntlet::UIButtonToggleableElement* _button = nullptr;
};