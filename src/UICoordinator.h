/* Purpose: Handles UIManagers and the elements within as to keep them better organized.
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */

#pragma once

#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIButtonToggleableElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>
#include <Jauntlet/Rendering/ProgressBar.h>

#include "Navigation.h"
#include "drill/DrillManager.h"

class UICoordinator {
public:
	UICoordinator(Jauntlet::Camera2D* hudCamera, Jauntlet::TextRenderer* spriteFont, DrillManager* drillManager);
	~UICoordinator();

	void draw();

	void applyNewScreenSize(glm::ivec2 screenSize);
	
	Navigation* navigation;
	std::string waterIconText = "0", foodIconText = "0", partsIconText = "0";
	std::string tempIconText = "0";
private:
	// Pointers
	Jauntlet::Camera2D* _hudCamera;
	Jauntlet::TextRenderer* _textRenderer;
	DrillManager* _drill;

	// UI Manager(s)
	Jauntlet::UIManager _UIManager;
	Jauntlet::UIManager* _NavManager;

	// Text handling
	Jauntlet::Color _textColor = Jauntlet::Color(); // default to 255, 255, 255, 255
	glm::vec2 _waterIconTextPosition = glm::vec2(85, 5);
	Jauntlet::UITextElement _waterIconTextElement;
	glm::vec2 _foodIconTextPosition = glm::vec2(285, 5);
	Jauntlet::UITextElement _foodIconTextElement;
	glm::vec2 _partsIconTextPosition = glm::vec2(485, 5);
	Jauntlet::UITextElement _partsIconTextElement;
	// TODO: make temperature different from the rest of the UI
	glm::vec2 _tempIconTextPosition = glm::vec2(685, 5);
	Jauntlet::UITextElement _tempIconTextElement;

	Jauntlet::ProgressBar _tempProgressBar = Jauntlet::ProgressBar("Textures/TempBar.png", glm::vec4(0.5,0,0.5,1), glm::vec4(0,0,0.5,1), glm::vec4(0, 0, 3.125 * 60, 60));

	// Icons
	glm::vec2 _waterIconPosition = glm::vec2(15, 15);
	Jauntlet::UISpriteElement _waterIcon = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/WaterResourceIcon.png").id, &_waterIconPosition, glm::vec2(60), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);
	glm::vec2 _foodIconPosition = glm::vec2(215, 15);
	Jauntlet::UISpriteElement _foodIcon = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/foodIcon.png").id, &_foodIconPosition, glm::vec2(60), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);
	glm::vec2 _partsIconPosition = glm::vec2(415, 15);
	Jauntlet::UISpriteElement _partsIcon = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/ResourceIcon.png").id, &_partsIconPosition, glm::vec2(60), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);
	glm::vec2 _tempIconPosition = glm::vec2(615, 15);
	Jauntlet::UISpriteElement _tempIcon = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/TemperatureIcon.png").id, &_tempIconPosition, glm::vec2(60), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);

	// Debug Button
	GLuint _buttonTexture = 0;
	glm::vec2* buttonPos = nullptr;
	Jauntlet::UIButtonToggleableElement* _button = nullptr;
};