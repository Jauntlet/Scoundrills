/* Purpose: The scene for allowing the player to select the Crew they want to take on their journey.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Audio/AudioSource.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UITextElement.h>
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

	Jauntlet::Camera2D _camera = Jauntlet::Camera2D(GlobalContext::screenSize.x, GlobalContext::screenSize.y);
	Jauntlet::UIManager _uiManager = Jauntlet::UIManager(&_camera);

	Jauntlet::AudioSource _music = Jauntlet::AudioSource(glm::vec3(0,0,0));

	Jauntlet::SpriteBatch _batch;
	SelectableCrew _crew[INMATE_COUNT];
	std::deque<SelectableCrew*> _selectedCrew;

	// Title text
	Jauntlet::Color _textColor = Jauntlet::Color(17, 17, 17);
	std::string _title = "Rogues Gallery";
	std::string _subTitle = "Select Your Crew!";
	glm::vec2 _titlePos = glm::vec2(0);
	glm::vec2 _subTitlePos = glm::vec2(0, 110);
	Jauntlet::UITextElement _titleElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_title, &_textColor, &_titlePos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.5f);
	Jauntlet::UITextElement _subtitleElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_subTitle, &_textColor, &_subTitlePos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.4f);

	// Confirm Text/Button
	Jauntlet::UIButtonElement _confirmButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&RogueGallery::loadGame, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_titlePos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::BOTTOM);
	std::string _confirmText = "Confirm";
	glm::vec2 _confirmTextPos = glm::vec2(0, -50);
	Jauntlet::UITextElement _confirmTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_confirmText, &_textColor, &_confirmTextPos, Jauntlet::UIElement::ORIGIN_PIN::BOTTOM, 0.4f);

	int _saveID;
};

