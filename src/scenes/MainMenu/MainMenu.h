/* Purpose: Scene holding all of the main menu info and elements, to then move into the maingame afterwards.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once

#include <Jauntlet/Rendering/TextRenderer.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include "../../buttons/ControllerButton.h"
#include <Jauntlet/UI/UIManager.h>

#include "../GlobalContext.h"
#include "Jauntlet/UI/UIText.h"
#include "SaveInfoElement.h"

class SceneManager;

class MainMenu {
public:
	MainMenu();

	void gameLoop();

	void windowResized();
	
	void promptTutorial(int saveID);

	void startSavedGame(int ID);
private:
	void startGame();
	void startTutorial();
	void quitGame();
	void toggleSettingsMenu();
	void toggleSavesMenu();

	Camera2D _camera;
	AudioSource _music = AudioSource(glm::vec3(0, 0, 0));
	UIManager _uiManager;

	Color _textColor = Color(17, 17, 17);
	Color _titleTextColor = Color(255, 255, 255);
	GLuint _buttonTexture = Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id;

	// Main Menu Elements
	glm::vec2 _startButtonPos = glm::vec2(0, -200);
	ControllerButton _startButton = ControllerButton(&GlobalContext::inputManager, std::bind(&MainMenu::toggleSavesMenu, this), _buttonTexture, &_startButtonPos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::CENTER);
	std::string _startText = "Start";
	UIText _startButtonText = UIText(GlobalContext::textRenderer, &_startText, &_textColor, &_startButtonPos, UIElement::ORIGIN_PIN::CENTER, 0.5f);

	glm::vec2 _settingsButtonPos = glm::vec2(0, 50);
	ControllerButton _settingsButton = ControllerButton(&GlobalContext::inputManager, std::bind(&MainMenu::toggleSettingsMenu, this), _buttonTexture, &_settingsButtonPos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::CENTER);
	std::string _settingsText = "Settings";
	UIText _settingsTextElement = UIText(GlobalContext::textRenderer, &_settingsText, &_textColor, &_settingsButtonPos, UIElement::ORIGIN_PIN::CENTER, 0.5f);

	glm::vec2 _quitButtonPos = glm::vec2(0, 300);
	ControllerButton _quitButton = ControllerButton(&GlobalContext::inputManager, std::bind(&MainMenu::quitGame, this), _buttonTexture, &_quitButtonPos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::CENTER);
	std::string _quitText = "Quit";
	UIText _quitTextElement = UIText(GlobalContext::textRenderer, &_quitText, &_textColor, &_quitButtonPos, UIElement::ORIGIN_PIN::CENTER, 0.5f);

	glm::vec2 _titlePosition = glm::vec2(0);
	std::string _titleText = "Scoundrills";
	UIText _titleTextElement = UIText(GlobalContext::textRenderer, &_titleText, &_titleTextColor, &_titlePosition, UIElement::ORIGIN_PIN::TOP, 0.5f);

	// Save info
	SaveInfoElement _saveInfoElements[3] = { SaveInfoElement(50.0f, 1, this, &_saveInfoElements[2], &_saveInfoElements[1]), SaveInfoElement(400.0f, 2, this, &_saveInfoElements[0], &_saveInfoElements[2]), SaveInfoElement(750.0f, 3, this, &_saveInfoElements[1], &_saveInfoElements[0]) };

	// Tutorial Prompt
	std::string _tutText = "Would You Like To\n Play The Tutorial?";
	UIText _tutTextElement = UIText(GlobalContext::textRenderer, &_tutText, &_titleTextColor, &_titlePosition, UIElement::ORIGIN_PIN::TOP, 0.4f);
	
	glm::vec2 _noButtonPos = glm::vec2(300, 400);
	ControllerButton _noButton = ControllerButton(&GlobalContext::inputManager, std::bind(&MainMenu::startGame, this), _buttonTexture, &_noButtonPos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::TOP);
	std::string _noText = "No";
	UIText _noTextElement = UIText(GlobalContext::textRenderer, &_noText, &_textColor, &_noButtonPos, UIElement::ORIGIN_PIN::TOP, 0.6f);
	
	glm::vec2 _yesButtonPos = glm::vec2(-300, 400);
	ControllerButton _yesButton = ControllerButton(&GlobalContext::inputManager, std::bind(&MainMenu::startTutorial, this), _buttonTexture, &_yesButtonPos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::TOP);
	std::string _yesText = "Yes";
	UIText _yesTextElement = UIText(GlobalContext::textRenderer, &_yesText, &_textColor, &_yesButtonPos, UIElement::ORIGIN_PIN::TOP, 0.6f);

	int _saveID;

	// Controller Support
	ControllerButton* _selectedButton = nullptr;
	bool _moving = false;
};