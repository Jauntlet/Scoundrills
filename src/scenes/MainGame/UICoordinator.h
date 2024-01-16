/* Purpose: Handles UIManagers and the elements within as to keep them better organized.
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */

#pragma once

#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIButtonToggleable.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIText.h>
#include <Jauntlet/UI/UIProgressBar.h>
#include <Jauntlet/UI/UIButtonToggleable.h>

#include "../GlobalContext.h"
#include "src/piloting/Navigation.h"
#include "src/drill/DrillManager.h"

class UICoordinator {
public:
	UICoordinator(Camera2D* hudCamera, DrillManager* drillManager);
	~UICoordinator();

	void draw();

	void applyNewScreenSize(glm::ivec2 screenSize);
	
	void showLoseScreen();

	void toggleButtonDisplay();
	bool isButtonOn();

	Navigation* navigation;
	std::string waterIconText = "0", foodIconText = "0", partsIconText = "0";
protected:
	void restartGame();

	// Icons
	glm::vec2 _waterIconPosition = glm::vec2(30, 15);
	UISprite _waterIcon = UISprite(Jauntlet::ResourceManager::getTexture("Textures/WaterResourceIcon.png").id, &_waterIconPosition, glm::vec2(120), UIElement::ORIGIN_PIN::TOP_LEFT);
	glm::vec2 _foodIconPosition = glm::vec2(230, 15);
	UISprite _foodIcon = UISprite(Jauntlet::ResourceManager::getTexture("Textures/foodIcon.png").id, &_foodIconPosition, glm::vec2(120), UIElement::ORIGIN_PIN::TOP_LEFT);
	glm::vec2 _partsIconPosition = glm::vec2(430, 15);
	UISprite _partsIcon = UISprite(Jauntlet::ResourceManager::getTexture("Textures/ResourceIcon.png").id, &_partsIconPosition, glm::vec2(120), UIElement::ORIGIN_PIN::TOP_LEFT);
	
	// Text handling
	Color _textColor = Color(); // default to 255, 255, 255, 255
	glm::vec2 _waterIconTextPosition = glm::vec2(30, _waterIconPosition.y + (120 - GlobalContext::textRenderer->calculateTextSize(waterIconText, glm::vec2(0.4f)).y) * 0.5);
	UIText _waterIconTextElement = UIText(GlobalContext::textRenderer, &waterIconText, &_textColor, &_waterIconTextPosition, 0.4f);
	glm::vec2 _foodIconTextPosition = glm::vec2(285, _foodIconPosition.y + (120 - GlobalContext::textRenderer->calculateTextSize(foodIconText, glm::vec2(0.4f)).y) * 0.5);
	UIText _foodIconTextElement = UIText(GlobalContext::textRenderer, &foodIconText, &_textColor, &_foodIconTextPosition, 0.4f);
	glm::vec2 _partsIconTextPosition = glm::vec2(485, _partsIconPosition.y + (120 - GlobalContext::textRenderer->calculateTextSize(partsIconText, glm::vec2(0.4f)).y) * 0.5);
	UIText _partsIconTextElement = UIText(GlobalContext::textRenderer, &partsIconText, &_textColor, &_partsIconTextPosition, 0.4f);
	UIProgressBar _tempProgressBar = UIProgressBar("Textures/TempBar.png", glm::vec4(0.5,0,0.5,1), glm::vec4(0,0,0.5,1), glm::vec4(615, 15, 3.125 * 120, 120));
	std::string _drillMovingText = "Drill is not moving!";
	glm::vec2 _drillMovingTextPos = glm::vec2(0, -275);
	UIText _drillMovingElement = UIText(GlobalContext::textRenderer, &_drillMovingText, &_textColor, &_drillMovingTextPos, UIElement::ORIGIN_PIN::BOTTOM_LEFT, 0.2f);
	// Game over screen
	glm::vec2 _loseBcgPos = glm::vec2(0);
	UISprite _lostBcgElemet = UISprite(Jauntlet::ResourceManager::getTexture("Textures/savebcg.png").id, &_loseBcgPos, glm::vec2(1600, 600), UIElement::ORIGIN_PIN::CENTER);
	glm::vec2 _loseTitlePos = glm::vec2(0, -280);
	std::string _loseTitle = "You have Lost!";
	UIText _loseTitleElement = UIText(GlobalContext::textRenderer, &_loseTitle, &_textColor, &_loseTitlePos, UIElement::ORIGIN_PIN::CENTER, 0.5f);
	glm::vec2 _restartButtonPos = glm::vec2(0, 200);
	std::string _restartText = "Restart";
	UIButton _restartButton = UIButton(&GlobalContext::inputManager, std::bind(&UICoordinator::restartGame, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_restartButtonPos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::CENTER);
	UIText _restartTextElement = UIText(GlobalContext::textRenderer, &_restartText, &_textColor, &_restartButtonPos, UIElement::ORIGIN_PIN::CENTER, 0.5f);

	// Pointers
	Camera2D* _hudCamera;
	DrillManager* _drill;

	// UI Manager(s)
	UIManager _UIManager;
	UIManager* _NavManager;
	UIManager* _CavernManager;



	// Drill state Button
	GLuint _buttonTexture = 0;
	glm::vec2* buttonPos = nullptr;
	UIButtonToggleable* _button = nullptr;
};