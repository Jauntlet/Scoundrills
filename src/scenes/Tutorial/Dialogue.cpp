#include "Dialogue.h"

Dialogue::Dialogue(Jauntlet::UIManager* uiManager) {
	uiManager->addElement(&_dialogueBox, &GlobalContext::normalShader);
	uiManager->addElement(&_dialogueTextElement, &Jauntlet::TextRenderer::textShader);
}

void Dialogue::hide() {
	_dialogueBox.visible = false;
	_dialogueTextElement.visible = false;
}
void Dialogue::show() {
	_dialogueBox.visible = true;
	_dialogueTextElement.visible = true;
}

void Dialogue::pushNewText(std::string newText) {
	for (auto& c : newText) {
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
}

void Dialogue::update() {
	if (_storedText.empty()) return;

	_timer += Jauntlet::Time::getDeltaTime();

	while (_timer > TEXT_SPEED) {
		_timer -= TEXT_SPEED;
		_dialogueText += _storedText[0];
		_storedText.pop_front();
	}
}