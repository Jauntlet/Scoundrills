#pragma once
#include "Jauntlet/UI/UIElement.h"
#include "Jauntlet/UI/UISpriteElement.h"
#include "Jauntlet/UI/UITextElement.h"
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include "GlobalContext.h"
#include <Jauntlet/Rendering/TextRenderer.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>

class SceneManager;

class SaveInfoElement {
	SaveInfoElement(float yPos);

	void addToManager(Jauntlet::UIManager& uiManager);
	void setVisibility(bool visible);
private:
	glm::vec2 _position;
	Jauntlet::UISpriteElement _background = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/savebcg.png").id, &_position, glm::vec2(300, 800), Jauntlet::UIElement::ORIGIN_PIN::TOP);

};

class MainMenu
{
public:
	MainMenu(SceneManager* sceneManager);

	void gameLoop();

	void windowResized();
private:
	void startGame();
	void quitGame();
	void toggleSettingsMenu();

	bool _switch = false; // true when switching to next scene

	Jauntlet::Camera2D _camera;
	Jauntlet::UIManager _uiManager;

	Jauntlet::Color _textColor = Jauntlet::Color(17,17,17);
	Jauntlet::Color _titleTextColor = Jauntlet::Color(255,255,255);
	
	SceneManager* _sceneManager;

	// Main Menu Elements
	glm::vec2 _startButtonPos = glm::vec2(0,-200);
	Jauntlet::UIButtonElement _startButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&MainMenu::startGame, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_startButtonPos, glm::vec2(600,200), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	std::string _startText = "Start";
	Jauntlet::UITextElement _startButtonText = Jauntlet::UITextElement(GlobalContext::textRenderer, &_startText, &_textColor, &_startButtonPos, Jauntlet::UIButtonElement::ORIGIN_PIN::CENTER, 0.5f);
	
	glm::vec2 _settingsButtonPos = glm::vec2(0, 50);
	Jauntlet::UIButtonElement _settingsButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&MainMenu::toggleSettingsMenu, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_settingsButtonPos, glm::vec2(600,200), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	std::string _settingsText = "Settings";
	Jauntlet::UITextElement _settingsTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_settingsText, &_textColor, &_settingsButtonPos, Jauntlet::UIElement::ORIGIN_PIN::CENTER, 0.5f);

	glm::vec2 _quitButtonPos = glm::vec2(0, 300);
	Jauntlet::UIButtonElement _quitButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&MainMenu::quitGame, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_quitButtonPos, glm::vec2(600,200), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	std::string _quitText = "Quit";
	Jauntlet::UITextElement _quitTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_quitText, &_textColor, &_quitButtonPos, Jauntlet::UIElement::ORIGIN_PIN::CENTER, 0.5f);

	glm::vec2 _titlePosition = glm::vec2(0);
	std::string _titleText = "Scoundrills";
	Jauntlet::UITextElement _titleTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_titleText, &_titleTextColor, &_titlePosition, Jauntlet::UIButtonElement::ORIGIN_PIN::TOP, 0.5f);

};

