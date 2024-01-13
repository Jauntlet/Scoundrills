/* Purpose: Handles the dialogue box and the text that renders within it.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UISpriteElement.h>
#include <Jauntlet/UI/UITextElement.h>
#include <deque>

#include "../GlobalContext.h"

const float TEXT_SPEED = 0.025f;

class Dialogue
{
public:
	Dialogue(Jauntlet::Camera2D* camera);

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
	Jauntlet::UIManager _uiManager;

	// Dialogue box
	glm::vec2 _dialogueBoxPos = glm::vec2(0);
	Jauntlet::UISpriteElement _dialogueBox = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/savebcg.png").id, &_dialogueBoxPos, glm::vec2(1000, 375), Jauntlet::UIElement::ORIGIN_PIN::BOTTOM);
	
	// Timer
	float _timer = 0.0f;

	// Text
	glm::vec2 _textPos = glm::vec2(0, -275);
	Jauntlet::Color _textColor = Jauntlet::Color();
	std::string _dialogueText = "";
	std::deque<char> _storedText;
	Jauntlet::UITextElement _dialogueTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_dialogueText, &_textColor, &_textPos, Jauntlet::UIElement::ORIGIN_PIN::BOTTOM, 0.225f);
};

