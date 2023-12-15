/* Purpose: A menu for when the game is paused, or when we need to change settings
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UITextElement.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include "GlobalContext.h"

class PauseMenu {
private:
	enum class PauseState { HIDDEN, SETTINGS, PAUSED };
public:
	PauseMenu(Jauntlet::TextureCache& textureCache);

	void update();
	void draw();

	bool isPaused();

	void togglePauseMenu();
	void hideAll();

	void windowResized();
private:
	void switchState(PauseState state);

	PauseState _state = PauseState::HIDDEN;

	// Default UI parameters
	Jauntlet::Camera2D _camera = Jauntlet::Camera2D(GlobalContext::screenSize);
	Jauntlet::UIManager _uiManager = Jauntlet::UIManager(&_camera);
	Jauntlet::Color _textColor = Jauntlet::Color(0, 0, 0);

	// Generic Pause Menu
	glm::vec2 _resumeButtonPos = glm::vec2(0, -200);
	glm::vec2 _settingsButtonPos = glm::vec2(0, 50);
	glm::vec2 _quitButtonPos = glm::vec2(0, 300);
	Jauntlet::UIButtonElement _resumeButton;
	Jauntlet::UIButtonElement _settingsButton;
	Jauntlet::UIButtonElement _quitButton;
	std::string _resumeText = "Resume";
	std::string _settingsText = "Settings";
	std::string _quitText = "Quit";
	Jauntlet::UITextElement _resumeTextElement;
	Jauntlet::UITextElement _settingsTextElement;
	Jauntlet::UITextElement _quitTextElement;
};

