/* Purpose: Handles the dialogue box and the text that renders within it.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UISprite.h>
#include <Jauntlet/UI/UIText.h>
#include <deque>

#include "../GlobalContext.h"

const float TEXT_SPEED = 0.025f;

class Dialogue
{
public:
	Dialogue(Camera2D* camera);

	void hide();
	void show();

	void resize();

	// updates what text should be displayed
	void pushNewText(std::string newText);
	// stops slowly updating text and forces all stored text to be shown instantly
	void pushAllText();

	// returns true if read-in text has finished being displayed in the dialogue box.
	bool doneReadingText();

	void update();
private:
	// UI Manager
	UIManager _uiManager;

	// Dialogue box
	glm::vec2 _dialogueBoxPos = glm::vec2(0);
	UISprite _dialogueBox = UISprite(Jauntlet::ResourceManager::getTexture("Textures/savebcg.png").id, &_dialogueBoxPos, glm::vec2(1000, 375), UIElement::ORIGIN_PIN::BOTTOM);
	
	// Timer
	float _timer = 0.0f;

	// Text
	glm::vec2 _textPos = glm::vec2(0, -275);
	Color _textColor = Color();
	std::string _dialogueText = "";
	std::deque<char> _storedText;
	UIText _dialogueTextElement = UIText(GlobalContext::textRenderer, &_dialogueText, &_textColor, &_textPos, UIElement::ORIGIN_PIN::BOTTOM, 0.225f);
};

