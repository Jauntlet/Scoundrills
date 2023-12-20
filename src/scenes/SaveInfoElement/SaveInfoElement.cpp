#include "SaveInfoElement.h"

SaveInfoElement::SaveInfoElement(float yPos) : 
	_position(0, yPos),
	_playPos(250, yPos + 200)
{
	// Empty
}
void SaveInfoElement::addToManager(Jauntlet::UIManager& uiManager) {
	uiManager.addElement(&_background, &GlobalContext::normalShader);
	uiManager.addElement(&_playButton, &GlobalContext::normalShader);
	uiManager.addElement(&_playTextElement, &Jauntlet::TextRenderer::textShader);
}
void SaveInfoElement::setVisibility(bool visible) {
	_background.visible = visible;
	_playButton.visible = visible;
	_playTextElement.visible = visible;
}