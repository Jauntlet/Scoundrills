#pragma once
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UISpriteElement.h>
#include <Jauntlet/UI/UITextElement.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include "../GlobalContext.h"
#include <deque>

const float TEXT_SPEED = 0.025f;

class Dialogue
{
public:
	Dialogue(Jauntlet::UIManager* uiManager);

	void hide();
	void show();

	// updates what text should be displayed
	void pushNewText(std::string newText);
	// stops slowly updating text and forces all stored text to be shown instantly
	void pushAllText();

	void update();
private:
	// Dialogue box
	glm::vec2 _dialogueBoxPos = glm::vec2(0);
	Jauntlet::UISpriteElement _dialogueBox = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_dialogueBoxPos, glm::vec2(80, 30), Jauntlet::UIElement::ORIGIN_PIN::BOTTOM);
	
	// Timer
	float _timer = 0.0f;

	// Text
	Jauntlet::Color _textColor = Jauntlet::Color();
	std::string _dialogueText = "";
	std::deque<char> _storedText;
	Jauntlet::UITextElement _dialogueTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_dialogueText, &_textColor, &_dialogueBoxPos, Jauntlet::UIElement::ORIGIN_PIN::BOTTOM, 0.5f);
};

