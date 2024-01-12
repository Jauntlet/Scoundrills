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

class MainMenu;

class SaveInfoElement {
public:
	SaveInfoElement(float yPos, int saveID, MainMenu* mainMenu);

	void addToManager(Jauntlet::UIManager& uiManager);
	void setVisibility(bool visible);
	void deleteSave();
	void startGame();
private:
	bool _hasSaveInfo;
	int _saveID;
	MainMenu* _mainMenu;

	Jauntlet::Color _textColor = Jauntlet::Color(250, 250, 250);
	Jauntlet::Color _deleteTextColor = Jauntlet::Color(250, 0, 0);

	glm::vec2 _position, _playPos, _deletePos, _loadPos;
	Jauntlet::UISpriteElement _background = Jauntlet::UISpriteElement(Jauntlet::ResourceManager::getTexture("Textures/savebcg.png").id, &_position, glm::vec2(800, 300), Jauntlet::UIElement::ORIGIN_PIN::TOP);
	
	Jauntlet::UIButtonElement _playButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&SaveInfoElement::startGame, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_playPos, glm::vec2(300, 100), Jauntlet::UIElement::ORIGIN_PIN::TOP);
	std::string _playText = "Play!";
	Jauntlet::UITextElement _playTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_playText, &_textColor, &_playPos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.3f);
	
	Jauntlet::UIButtonElement _loadButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&SaveInfoElement::startGame, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_loadPos, glm::vec2(300, 100), Jauntlet::UIElement::ORIGIN_PIN::TOP);
	std::string _loadText = "Load!";
	Jauntlet::UITextElement _loadTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_loadText, &_textColor, &_loadPos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.3f);

	Jauntlet::UIButtonElement _deleteButton = Jauntlet::UIButtonElement(&GlobalContext::inputManager, std::bind(&SaveInfoElement::deleteSave, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_deletePos, glm::vec2(300, 100), Jauntlet::UIElement::ORIGIN_PIN::TOP);
	std::string _deleteText = "Delete";
	Jauntlet::UITextElement _deleteTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_deleteText, &_deleteTextColor, &_deletePos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.3f);

	std::string _saveNumText = "Save ";
	glm::vec2 _saveNumPos;
	Jauntlet::UITextElement _saveNumElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_saveNumText, &_textColor, &_saveNumPos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.3f);

	std::string _depthCount = "Depth: ";
	glm::vec2 _depthCountPos;
	Jauntlet::UITextElement _depthCountElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_depthCount, &_textColor, &_depthCountPos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.3f);

	std::string _playtimeText = "Playtime: ";
	glm::vec2 _playtimePos;
	Jauntlet::UITextElement _playtimeElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_playtimeText, &_textColor, &_playtimePos, Jauntlet::UIElement::ORIGIN_PIN::TOP, 0.3f);
};