#include "SaveInfoElement.h"
#include <Jauntlet/Filesystems/FileManager.h>

SaveInfoElement::SaveInfoElement(float yPos, int saveID) : 
	_position(0, yPos),
	_hasSaveInfo(true),
	_saveID(saveID),
	_playPos(250, yPos + 200),
	_deletePos(-50, yPos + 200)
{
	// Empty
}
void SaveInfoElement::addToManager(Jauntlet::UIManager& uiManager) {
	uiManager.addElement(&_background, &GlobalContext::normalShader);
	uiManager.addElement(&_playButton, &GlobalContext::normalShader);
	if (_hasSaveInfo) {
		uiManager.addElement(&_deleteButton, &GlobalContext::normalShader);
	}
	uiManager.addElement(&_playTextElement, &Jauntlet::TextRenderer::textShader);
	if (_hasSaveInfo) {
		uiManager.addElement(&_deleteTextElement, &Jauntlet::TextRenderer::textShader);
	}
}
void SaveInfoElement::setVisibility(bool visible) {
	_background.visible = visible;
	_playButton.visible = visible;
	_playTextElement.visible = visible;
	_deleteButton.visible = _hasSaveInfo ? visible : false;
	_deleteTextElement.visible = _hasSaveInfo ? visible : false;
}
void SaveInfoElement::deleteSave() {
	// unimplemented
	_hasSaveInfo = false;
	setVisibility(_playTextElement.visible);
	Jauntlet::FileManager::deleteFile((std::to_string(_saveID) + ".db").c_str());
}