/* Purpose: Handles UIManagers and the elements within as to keep them better organized.
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */

#pragma once

#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIButtonToggleableElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>
#include <Jauntlet/UI/UIProgressBarElement.h>
#include <Jauntlet/UI/UIButtonToggleableElement.h>

#include "src/scenes/GlobalContext.h"
#include "piloting/Navigation.h"
#include "drill/DrillManager.h"

class UICoordinator {
public:
	UICoordinator(Jauntlet::Camera2D* hudCamera, Jauntlet::TextRenderer* spriteFont, DrillManager* drillManager);
	~UICoordinator();

	void draw();

	void applyNewScreenSize(glm::ivec2 screenSize);
	
	void showLoseScreen();

	Navigation* navigation;
	std::string waterIconText = "0", foodIconText = "0", partsIconText = "0";
private:
	// Pointers
	Jauntlet::Camera2D* _hudCamera;
	Jauntlet::TextRenderer* _textRenderer;
	DrillManager* _drill;

	// UI Manager(s)
	Jauntlet::UIManager _UIManager;
	Jauntlet::UIManager* _NavManager;
	Jauntlet::UIManager* _CavernManager;

	// Text handling
	Jauntlet::Color _textColor = Jauntlet::Color(); // default to 255, 255, 255, 255
	glm::vec2 _waterIconTextPosition = glm::vec2(30, _waterIconPosition.y + (120 - GlobalContext::textRenderer->calculateTextSize(waterIconText, glm::vec2(0.4f)).y) * 0.5);
	Jauntlet::UITextElement _waterIconTextElement = Jauntlet::UITextElement(_textRenderer, &waterIconText, &_textColor, &_waterIconTextPosition, 0.4f);
	glm::vec2 _foodIconTextPosition = glm::vec2(285, _foodIconPosition.y + (120 - GlobalContext::textRenderer->calculateTextSize(foodIconText, glm::vec2(0.4f)).y) * 0.5);
	Jauntlet::UITextElement _foodIconTextElement = Jauntlet::UITextElement(_textRenderer, &foodIconText, &_textColor, &_foodIconTextPosition, 0.4f);
	glm::vec2 _partsIconTextPosition = glm::vec2(485, _partsIconPosition.y + (120 - GlobalContext::textRenderer->calculateTextSize(partsIconText, glm::vec2(0.4f)).y) * 0.5);
	Jauntlet::UITextElement _partsIconTextElement = Jauntlet::UITextElement(_textRenderer, &partsIconText, &_textColor, &_partsIconTextPosition, 0.4f);
	Jauntlet::UIProgressBarElement _tempProgressBar = Jauntlet::UIProgressBarElement("Textures/TempBar.png", glm::vec4(0.5,0,0.5,1), glm::vec4(0,0,0.5,1), glm::vec4(615, 15, 3.125 * 120, 120));

	// Icons
	glm::vec2 _waterIconPosition = glm::vec2(30, 15);
	Jauntlet::UISpriteElement _waterIcon = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/WaterResourceIcon.png").id, &_waterIconPosition, glm::vec2(120), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);
	glm::vec2 _foodIconPosition = glm::vec2(230, 15);
	Jauntlet::UISpriteElement _foodIcon = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/foodIcon.png").id, &_foodIconPosition, glm::vec2(120), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);
	glm::vec2 _partsIconPosition = glm::vec2(430, 15);
	Jauntlet::UISpriteElement _partsIcon = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/ResourceIcon.png").id, &_partsIconPosition, glm::vec2(120), Jauntlet::UIElement::ORIGIN_PIN::TOP_LEFT);

	// Game over screen
	glm::vec2 _loseBcgPos = glm::vec2(0);
	Jauntlet::UISpriteElement _lostBcgElemet = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/savebcg.png").id, &_loseBcgPos, glm::vec2(1600, 600), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	glm::vec2 _loseTitlePos = glm::vec2(0, -280);
	std::string _loseTitle = "You have Lost!";
	Jauntlet::UITextElement _loseTitleElement = Jauntlet::UITextElement(_textRenderer, &_loseTitle, &_textColor, &_loseTitlePos, Jauntlet::UIElement::ORIGIN_PIN::CENTER, 0.5f);

	// Drill state Button
	GLuint _buttonTexture = 0;
	glm::vec2* buttonPos = nullptr;
	Jauntlet::UIButtonToggleableElement* _button = nullptr;
};