#include "Dialogue.h"

Dialogue::Dialogue(Jauntlet::Camera2D* camera) :
	_uiManager(camera)
{
	_uiManager.addElement(&_dialogueBox, &GlobalContext::normalShader);
	_uiManager.addElement(&_dialogueTextElement, &Jauntlet::TextRenderer::textShader);

	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);

	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void Dialogue::hide() {
	_dialogueBox.visible = false;
	_dialogueTextElement.visible = false;
}
void Dialogue::show() {
	_dialogueBox.visible = true;
	_dialogueTextElement.visible = true;
}

void Dialogue::resize() {
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.resolvePositions();
}

void Dialogue::pushNewText(std::string newText) {
	_storedText.clear();
	for (char c : newText) {
		_storedText.push_back(c);
	}
	_timer = 0.0f;
	_dialogueText = "";
}
void Dialogue::pushAllText() {
	while (!_storedText.empty()) {
		_dialogueText += _storedText[0];
		_storedText.pop_front();
	}
	_uiManager.resolvePositions();
}

bool Dialogue::doneReadingText() {
	return _storedText.empty();
}

void Dialogue::update() {
	if (_storedText.empty()) {
		_uiManager.draw();
		return;
	}

	_timer += Jauntlet::Time::getDeltaTime();

	while (_timer > TEXT_SPEED && !_storedText.empty()) {
		_timer -= TEXT_SPEED;
		_dialogueText += _storedText[0];
		_storedText.pop_front();
		_uiManager.resolvePositions();
	}

	_uiManager.draw();
}