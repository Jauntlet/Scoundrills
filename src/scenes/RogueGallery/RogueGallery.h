/* Purpose: The scene for allowing the player to select the Crew they want to take on their journey.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Audio/AudioSource.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include "../../buttons/ControllerButton.h"
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIText.h>
#include <deque>

#include "../GlobalContext.h"
#include "SelectableCrew.h"

class SceneManager;

const uint16_t INMATE_COUNT = 5;

class RogueGallery {
public:
	RogueGallery(int saveID, bool tutorialMode = false);

	void windowResized();
	void gameLoop();
private:
	void loadGame();

	bool _goToTutorial;

	Camera2D _camera = Camera2D(GlobalContext::screenSize.x, GlobalContext::screenSize.y);
	UIManager _uiManager = UIManager(&_camera);

	AudioSource _music = AudioSource(glm::vec3(0,0,0));

	SpriteBatch _batch;
	SelectableCrew _crew[INMATE_COUNT];
	std::deque<SelectableCrew*> _selectedCrew;

	// Title text
	Color _textColor = Color(17, 17, 17);
	std::string _title = "Rogues Gallery";
	std::string _subTitle = "Select Your Crew!";
	glm::vec2 _titlePos = glm::vec2(0);
	glm::vec2 _subTitlePos = glm::vec2(0, 110);
	UIText _titleElement = UIText(GlobalContext::textRenderer, &_title, &_textColor, &_titlePos, UIElement::ORIGIN_PIN::TOP, 0.5f);
	UIText _subtitleElement = UIText(GlobalContext::textRenderer, &_subTitle, &_textColor, &_subTitlePos, UIElement::ORIGIN_PIN::TOP, 0.4f);

	// Confirm Text/Button
	ControllerButton _confirmButton = ControllerButton(&GlobalContext::inputManager, std::bind(&RogueGallery::loadGame, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_titlePos, glm::vec2(600, 200), UIElement::ORIGIN_PIN::BOTTOM);
	std::string _confirmText = "Confirm";
	glm::vec2 _confirmTextPos = glm::vec2(0, -50);
	UIText _confirmTextElement = UIText(GlobalContext::textRenderer, &_confirmText, &_textColor, &_confirmTextPos, UIElement::ORIGIN_PIN::BOTTOM, 0.4f);

	int _saveID;

	// Controller Support
	bool _moving = false;
	uint8_t _controllerSelectedIndex = 0;
	bool _controllerOnCrew = true;
	GLuint _selectionTextureID = Jauntlet::ResourceManager::getTexture("Textures/WhiteSquare.png").id;
};

