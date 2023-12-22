/* Purpose: Scene holding all of the main menu info and elements, to then move into the maingame afterwards.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once
#include "Jauntlet/UI/UIElement.h"
#include "Jauntlet/UI/UISpriteElement.h"
#include "Jauntlet/UI/UITextElement.h"
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include "../GlobalContext.h"
#include <Jauntlet/Rendering/TextRenderer.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include "SaveInfoElement.h"

class SceneManager;

class MainMenu {
public:
	MainMenu(SceneManager* sceneManager);

	void gameLoop();

	void windowResized();
	
	void promptTutorial();

	void startSavedGame(int ID);
private:
	void startGame();
	void startTutorial();
	void quitGame();
	void toggleSettingsMenu();
	void toggleSavesMenu();

	Jauntlet::Camera2D _camera;
	Jauntlet::AudioSource _music = Jauntlet::AudioSource(glm::vec3(0, 0, 0));
	Jauntlet::UIManager _uiManager;

	Jauntlet::Color _textColor = Jauntlet::Color(17, 17, 17);
	Jauntlet::Color _titleTextColor = Jauntlet::Color(255, 255, 255);
	GLuint _buttonTexture = Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id;

	SceneManager* _sceneManager;

	// Main Menu Elements
	glm::vec2 _startButtonPos = glm::vec2(0, -200);
	Jauntlet::UIButtonElement _startButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&MainMenu::toggleSavesMenu, this), _buttonTexture, &_startButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	std::string _startText = "Start";
	Jauntlet::UITextElement _startButtonText = Jauntlet::UITextElement(GlobalContext::textRenderer, &_startText, &_textColor, &_startButtonPos, Jauntlet::UIElement::ORIGIN_PIN::CENTER, 0.5f);

	glm::vec2 _settingsButtonPos = glm::vec2(0, 50);
	Jauntlet::UIButtonElement _settingsButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&MainMenu::toggleSettingsMenu, this), _buttonTexture, &_settingsButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	std::string _settingsText = "Settings";
	Jauntlet::UITextElement _settingsTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_settingsText, &_textColor, &_settingsButtonPos, Jauntlet::UIElement::ORIGIN_PIN::CENTER, 0.5f);

	glm::vec2 _quitButtonPos = glm::vec2(0, 300);
	Jauntlet::UIButtonElement _quitButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&MainMenu::quitGame, this), _buttonTexture, &_quitButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::CENTER);
	std::string _quitText = "Quit";
	Jauntlet::UITextElement _quitTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_quitText, &_textColor, &_quitButtonPos, Jauntlet::UIElement::ORIGIN_PIN::CENTER, 0.5f);

	glm::vec2 _titlePosition = glm::vec2(0);
	std::string _titleText = "Scoundrills";
	Jauntlet::UITextElement _titleTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_titleText, &_titleTextColor, &_titlePosition, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.5f);

	// Save info
	SaveInfoElement _saveInfoElements[3] = { SaveInfoElement(50.0f, 1, this), SaveInfoElement(400.0f, 2, this), SaveInfoElement(750.0f, 3, this) };

	// Tutorial Prompt
	std::string _tutText = "Would You Like To\n Play The Tutorial?";
	Jauntlet::UITextElement _tutTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_tutText, &_titleTextColor, &_titlePosition, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.4f);
	
	glm::vec2 _noButtonPos = glm::vec2(300, 400);
	Jauntlet::UIButtonElement _noButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&MainMenu::startGame, this), _buttonTexture, &_noButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::TOP);
	std::string _noText = "No";
	Jauntlet::UITextElement _noTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_noText, &_textColor, &_noButtonPos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.6f);
	
	glm::vec2 _yesButtonPos = glm::vec2(-300, 400);
	Jauntlet::UIButtonElement _yesButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&MainMenu::promptTutorial, this), _buttonTexture, &_yesButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::TOP);
	std::string _yesText = "Yes";
	Jauntlet::UITextElement _yesTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_yesText, &_textColor, &_yesButtonPos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.6f);
};