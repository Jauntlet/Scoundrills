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
	UICoordinator(Jauntlet::Camera2D* hudCamera, Jauntlet::TextRenderer* spriteFont, DrillManager* drillManager, PlayerResources* resources);
	~UICoordinator();

	void draw();

	void applyNewScreenSize(glm::ivec2 screenSize);
	
	Navigation* navigation;
	std::string waterIconText = "0", foodIconText = "0", partsIconText = "0";
private:
	// Pointers
	Jauntlet::Camera2D* _hudCamera;
	Jauntlet::TextRenderer* _textRenderer;
	PlayerResources* _playerResources;

	// UI Manager(s)
	Jauntlet::UIManager _UIManager;
	Jauntlet::UIManager* _NavManager;

	// Text handling
	Jauntlet::Color _textColor = Jauntlet::Color(); // default to 255, 255, 255, 255
	glm::vec2 _waterIconTextPosition = glm::vec2(65, 5);
	Jauntlet::UITextElement _waterIconTextElement;
	glm::vec2 _foodIconTextPosition = glm::vec2(265, 5);
	Jauntlet::UITextElement _foodIconTextElement;
	glm::vec2 _partsIconTextPosition = glm::vec2(465, 5);
	Jauntlet::UITextElement _partsIconTextElement;

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