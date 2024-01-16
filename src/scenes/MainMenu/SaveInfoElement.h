/* Purpose: Container for UI related to save information for playing the game.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once
#include "Jauntlet/UI/UIElement.h"
#include "Jauntlet/UI/UISprite.h"
#include "Jauntlet/UI/UIText.h"
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButton.h>
#include "../GlobalContext.h"
#include <Jauntlet/Rendering/TextRenderer.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>

class MainMenu;

class SaveInfoElement {
public:
	SaveInfoElement(float yPos, int saveID, MainMenu* mainMenu);

	void addToManager(UIManager& uiManager);
	void setVisibility(bool visible);
	void deleteSave();
	void startGame();
private:
	bool _hasSaveInfo;
	int _saveID;
	float _yDisplacement;
	MainMenu* _mainMenu;

	Color _textColor = Color(250, 250, 250);
	Color _deleteTextColor = Color(250, 0, 0);

	glm::vec2 _position, _playPos, _deletePos, _loadPos;
	UISprite _background = UISprite(Jauntlet::ResourceManager::getTexture("Textures/savebcg.png").id, &_position, glm::vec2(800, 300), UIElement::ORIGIN_PIN::TOP);
	
	UIButton _playButton = UIButton(&GlobalContext::inputManager, std::bind(&SaveInfoElement::startGame, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_playPos, glm::vec2(300, 100), UIElement::ORIGIN_PIN::TOP);
	std::string _playText = "Play!";
	UIText _playTextElement = UIText(GlobalContext::textRenderer, &_playText, &_textColor, &_playPos, UIElement::ORIGIN_PIN::TOP, 0.3f);
	
	UIButton _loadButton = UIButton(&GlobalContext::inputManager, std::bind(&SaveInfoElement::startGame, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_loadPos, glm::vec2(300, 100), UIElement::ORIGIN_PIN::TOP);
	std::string _loadText = "Load!";
	UIText _loadTextElement = UIText(GlobalContext::textRenderer, &_loadText, &_textColor, &_loadPos, UIElement::ORIGIN_PIN::TOP, 0.3f);

	UIButton _deleteButton = UIButton(&GlobalContext::inputManager, std::bind(&SaveInfoElement::deleteSave, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_deletePos, glm::vec2(300, 100), UIElement::ORIGIN_PIN::TOP);
	std::string _deleteText = "Delete";
	UIText _deleteTextElement = UIText(GlobalContext::textRenderer, &_deleteText, &_deleteTextColor, &_deletePos, UIElement::ORIGIN_PIN::TOP, 0.3f);

	std::string _saveNumText = "Save ";
	glm::vec2 _saveNumPos;
	UIText _saveNumElement = UIText(GlobalContext::textRenderer, &_saveNumText, &_textColor, &_saveNumPos, UIElement::ORIGIN_PIN::TOP, 0.3f);

	std::string _depthCount = "Depth: ";
	glm::vec2 _depthCountPos;
	UIText _depthCountElement = UIText(GlobalContext::textRenderer, &_depthCount, &_textColor, &_depthCountPos, UIElement::ORIGIN_PIN::TOP, 0.2f);

	std::string _playtimeText = "Playtime: ";
	glm::vec2 _playtimePos;
	UIText _playtimeElement = UIText(GlobalContext::textRenderer, &_playtimeText, &_textColor, &_playtimePos, UIElement::ORIGIN_PIN::TOP, 0.2f);
};