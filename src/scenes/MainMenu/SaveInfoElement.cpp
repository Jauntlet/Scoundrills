#include <Jauntlet/Filesystems/FileManager.h>
#include <iostream>
#include <iterator>
#include <string>

#include "../../Database.h"
#include "Jauntlet/Rendering/TextRenderer.h"
#include "MainMenu.h"
#include "SaveInfoElement.h"

SaveInfoElement::SaveInfoElement(float yPos, int saveID, MainMenu* mainMenu) :
	_position(0, yPos),
	_mainMenu(mainMenu),
	_saveID(saveID),
	_playPos(250, yPos + 200),
	_loadPos(250, yPos + 200),
	_deletePos(-50, yPos + 200),
	_saveNumPos(-250,yPos),
	_depthCountPos(-225, yPos + 75)
{
	_saveNumText += std::to_string(_saveID);
	
	if (Database::IsSlotFull(saveID)) {
		_hasSaveInfo = true;
		
		// Load information here
		_depthCount += std::to_string(0);
	} else {
		_hasSaveInfo = false;
		_depthCount += "0";
	}
	
}

void SaveInfoElement::addToManager(Jauntlet::UIManager& uiManager) {
	uiManager.addElement(&_background, &GlobalContext::normalShader);
	uiManager.addElement(&_playButton, &GlobalContext::normalShader);
	if (_hasSaveInfo) {
		uiManager.addElement(&_deleteButton, &GlobalContext::normalShader);
		uiManager.addElement(&_deleteTextElement, &Jauntlet::TextRenderer::textShader);
	}
	uiManager.addElement(&_playTextElement, &Jauntlet::TextRenderer::textShader);
	uiManager.addElement(&_saveNumElement,&Jauntlet::TextRenderer::textShader);
	uiManager.addElement(&_depthCountElement, &Jauntlet::TextRenderer::textShader);
}

void SaveInfoElement::setVisibility(bool visible) {
	_background.visible = visible;
	_playButton.visible = visible;
	_playTextElement.visible = visible;
	_saveNumElement.visible = visible;
	_depthCountElement.visible = visible;
	_deleteButton.visible = _hasSaveInfo ? visible : false;
	_deleteTextElement.visible = _hasSaveInfo ? visible : false;
}

void SaveInfoElement::deleteSave() {
	_hasSaveInfo = false;
	setVisibility(_playTextElement.visible);
	Database::Delete(_saveID);
}

void SaveInfoElement::startGame() {
	if (_hasSaveInfo) {
		_mainMenu->startSavedGame(_saveID);
	}
	else {
		_mainMenu->promptTutorial(_saveID);
	}
}