/* Purpose: Container for UI related to save information for playing the game.
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

class SaveInfoElement {
public:
	SaveInfoElement(float yPos);

	void addToManager(Jauntlet::UIManager& uiManager);
	void setVisibility(bool visible);
private:
	Jauntlet::Color _textColor = Jauntlet::Color(250, 250, 250);

	glm::vec2 _position, _playPos;
	Jauntlet::UISpriteElement _background = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/savebcg.png").id, &_position, glm::vec2(800, 300), Jauntlet::UIElement::ORIGIN_PIN::TOP);
	Jauntlet::UIButtonElement _playButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&SaveInfoElement::setVisibility, this, false), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_playPos, glm::vec2(300, 100), Jauntlet::UIElement::ORIGIN_PIN::TOP);
	std::string _playText = "Play!";
	Jauntlet::UITextElement _playTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_playText, &_textColor, &_playPos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.3f);
};