#include "PauseMenu.h"
PauseMenu::PauseMenu(Jauntlet::TextureCache& textureCache) :
	_resumeButton(&GlobalContext::inputManager, std::bind(&PauseMenu::switchState, this, PauseState::HIDDEN), textureCache.getTexture("Textures/UIbutton.png").id, &_resumeButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::CENTER),
	_settingsButton(&GlobalContext::inputManager, std::bind(&PauseMenu::hideAll, this), textureCache.getTexture("Textures/UIbutton.png").id, &_settingsButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::CENTER)
{
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);

	_uiManager.addElement(&_resumeButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_settingsButton, &GlobalContext::normalShader);
	
	_uiManager.optimize();
	_uiManager.resolvePositions();
}

void PauseMenu::update() {
	_camera.update();
}
void PauseMenu::draw() {
	_camera.setActiveCamera();
	_uiManager.draw();
}

bool PauseMenu::isPaused() {
	return _state != PauseState::HIDDEN;
}

void PauseMenu::togglePauseMenu() {
	switchState(_state == PauseState::PAUSED ? PauseState::HIDDEN : PauseState::PAUSED);
}

void PauseMenu::hideAll() {
	_resumeButton.visible = false;
	_settingsButton.visible = false;
}

void PauseMenu::windowResized() {
	_camera.updateCameraSize(GlobalContext::screenSize);
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.resolvePositions();
}

void PauseMenu::switchState(PauseState state) {
	if (state == _state) {
		return;
	} else {
		_state = state;
	}

	hideAll();

	if (_state == PauseState::PAUSED) {
		_resumeButton.visible = true;
		_settingsButton.visible = true;
	}
}