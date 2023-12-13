/* Purpose: A menu for when the game is paused, or when we need to change settings
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
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

	// Generic Pause Menu
	glm::vec2 _resumeButtonPos = glm::vec2(0);
	glm::vec2 _settingsButtonPos = glm::vec2(0, 200);
	Jauntlet::UIButtonElement _resumeButton;
	Jauntlet::UIButtonElement _settingsButton;
};

