#include "PauseMenu.h"
#include "Jauntlet/Rendering/Textures/ResourceManager.h"
#include "src/scenes/SceneManager.h"
PauseMenu::PauseMenu(SceneManager* sceneManager) :
	_sceneManager(sceneManager),
	_resumeButton(&GlobalContext::inputManager, std::bind(&PauseMenu::switchState, this, PauseState::HIDDEN), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_resumeButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::CENTER),
	_settingsButton(&GlobalContext::inputManager, std::bind(&PauseMenu::switchState, this, PauseState::SETTINGS), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_settingsButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::CENTER),
	_quitButton(&GlobalContext::inputManager, std::bind(&PauseMenu::toMainMenu, this), Jauntlet::ResourceManager::getTexture("Textures/UIbutton.png").id, &_quitButtonPos, glm::vec2(600, 200), Jauntlet::UIElement::ORIGIN_PIN::CENTER),
	_resumeTextElement(GlobalContext::textRenderer, &_resumeText, &_textColor, &_resumeButtonPos, Jauntlet::UIElement::ORIGIN_PIN::CENTER, 0.5f),
	_settingsTextElement(GlobalContext::textRenderer, &_settingsText, &_textColor, &_settingsButtonPos, Jauntlet::UIElement::ORIGIN_PIN::CENTER, 0.5f),
	_quitTextElement(GlobalContext::textRenderer, &_quitText, &_textColor, &_quitButtonPos, Jauntlet::UIElement::ORIGIN_PIN::CENTER, 0.5f)
{
	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);

	_uiManager.addElement(&_resumeButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_resumeTextElement, &Jauntlet::TextRenderer::textShader);
	_uiManager.addElement(&_settingsButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_settingsTextElement, &Jauntlet::TextRenderer::textShader);
	_uiManager.addElement(&_quitButton, &GlobalContext::normalShader);
	_uiManager.addElement(&_quitTextElement, &Jauntlet::TextRenderer::textShader);
	
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
	_resumeTextElement.visible = false;
	_settingsButton.visible = false;
	_settingsTextElement.visible = false;
	_quitButton.visible = false;
	_quitTextElement.visible = false;
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
		_resumeTextElement.visible = true;
		_settingsButton.visible = true;
		_settingsTextElement.visible = true;
		_quitButton.visible = true;
		_quitTextElement.visible = true;
	}
}