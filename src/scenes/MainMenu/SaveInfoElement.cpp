#include <Jauntlet/Filesystems/FileManager.h>
#include <Jauntlet/JMath.h>
#include <iostream>
#include <iterator>
#include <string>

#include "../../Database.h"
#include "Jauntlet/Rendering/TextRenderer.h"
#include "MainMenu.h"
#include "SaveInfoElement.h"
#include "src/scenes/GlobalContext.h"

SaveInfoElement::SaveInfoElement(float yPos, int saveID, MainMenu* mainMenu) :
	_position(0, yPos),
	_mainMenu(mainMenu),
	_saveID(saveID),
	_yDisplacement(yPos),
	_playPos(250, yPos + 200),
	_loadPos(250, yPos + 200),
	_deletePos(-50, yPos + 200)
{
	_saveNumText += std::to_string(_saveID);

	if (Database::IsSlotFull(saveID)) {
		_hasSaveInfo = true;

		// Load information from database here
		_depthCount += JMath::Split(std::to_string(Database::GetSaveData(saveID)[0]), '.')[0];
		if (Database::GetSaveData(saveID)[1] > 60) {
			_playtimeText += JMath::Split(std::to_string(Database::GetSaveData(saveID)[1] / 60), '.')[0] + "m";
		}
		else {
			_playtimeText += JMath::Split(std::to_string(Database::GetSaveData(saveID)[1]), '.')[0] + "s";
		}
	} else {
		_hasSaveInfo = false;
		_depthCount += "0";
		_playtimeText += "0s";
	}
	
	// Correct text positions
	_saveNumPos = glm::vec2(-375 + GlobalContext::textRenderer->calculateTextSize(_saveNumText, glm::vec2(0.3f)).x /2,yPos);
	_playtimePos = glm::vec2(-375 + GlobalContext::textRenderer->calculateTextSize(_playtimeText, glm::vec2(0.2f)).x /2,yPos + 75);
	_depthCountPos =  glm::vec2(-375 + GlobalContext::textRenderer->calculateTextSize(_depthCount, glm::vec2(0.2f)).x /2,yPos + 125);
}

void SaveInfoElement::addToManager(UIManager& uiManager) {
	uiManager.addElement(&_background, &GlobalContext::normalShader);
	uiManager.addElement(&_playButton, &GlobalContext::normalShader);
	if (_hasSaveInfo) {
		uiManager.addElement(&_deleteButton, &GlobalContext::normalShader);
		uiManager.addElement(&_deleteTextElement, &TextRenderer::textShader);
	}
	uiManager.addElement(&_playTextElement, &TextRenderer::textShader);
	uiManager.addElement(&_saveNumElement,&TextRenderer::textShader);
	uiManager.addElement(&_depthCountElement, &TextRenderer::textShader);
	uiManager.addElement(&_playtimeElement, &TextRenderer::textShader);
}

void SaveInfoElement::setVisibility(bool visible) {
	_background.visible = visible;
	_playButton.visible = visible;
	_playTextElement.visible = visible;
	_saveNumElement.visible = visible;
	_depthCountElement.visible = visible;
	_playtimeElement.visible = visible;
	_deleteButton.visible = _hasSaveInfo ? visible : false;
	_deleteTextElement.visible = _hasSaveInfo ? visible : false;
}

void SaveInfoElement::deleteSave() {
	_hasSaveInfo = false;
	setVisibility(_playTextElement.visible);
	Database::Delete(_saveID);

	_depthCount = "Depth: 0";
	_playtimeText = "Playtime: 0s";

	_playtimePos = glm::vec2(-375 + GlobalContext::textRenderer->calculateTextSize(_playtimeText, glm::vec2(0.2f)).x / 2, _yDisplacement + 75);
	_depthCountPos = glm::vec2(-375 + GlobalContext::textRenderer->calculateTextSize(_depthCount, glm::vec2(0.2f)).x / 2, _yDisplacement + 125);
}

void SaveInfoElement::startGame() {
	if (_hasSaveInfo) {
		_mainMenu->startSavedGame(_saveID);
	}
	else {
		_mainMenu->promptTutorial(_saveID);
	}
}