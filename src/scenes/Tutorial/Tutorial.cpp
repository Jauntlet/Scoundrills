#include "Tutorial.h"
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include "../MainGame/UICoordinator.h"
#include "../PauseMenu.h"

Tutorial::Tutorial(const std::vector<uint8_t>& playerIDs) {
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(glm::ivec2(GlobalContext::screenSize.x, GlobalContext::screenSize.y));

	for (int i = 0; i < playerIDs.size(); ++i) {
		_players.createPlayer(glm::vec2(64 * (i + 1) + 704, -64 * 10), GlobalContext::playerIDtoTexture(playerIDs[i], true));
	}

	_camera.setScale(2.75f);
	_camera.setPosition(glm::vec2(1337.25, -3475.54));

	_drill.off();
	_uiCoordinator.hideAll();

	_dialogue.pushNewText("Test.");
}

void Tutorial::windowResized() {

	glm::vec2 newCenter = glm::vec2(GlobalContext::screenSize) / 2.0f - _camera.getSize() / 2.0f;

	//_camera.setScale(_camera.getScale() * (_camera.getSize().y / GlobalContext::screenSize.y));
	//_camera.setPosition(glm::vec2(_camera.getPosition().x - newCenter.x, _camera.getPosition().y - newCenter.y));

	_camera.updateSize(GlobalContext::screenSize);
	_hudCamera.updateSize(GlobalContext::screenSize);
	_uiCoordinator.applyNewScreenSize(GlobalContext::screenSize);
	_dialogue.resize();
}

void Tutorial::gameLoop() {
	processInput();
 
	_camera.update();
	_hudCamera.update();
	_drill.update();

	drawGame();
}

void Tutorial::processInput() {
	if (_players.getAllPlayers().size() == 0) {
		return;
	}

	_players.update(_drill);

	if (!_cameraLocked) {
		_cameraManager.processInput();
	}

	//open nav
	if (GlobalContext::inputManager.isKeyPressed(SDLK_TAB)) {
		_uiCoordinator.navigation->toggleNav();
	}

	if (GlobalContext::inputManager.isKeyPressed(SDLK_ESCAPE)) {
		GlobalContext::pauseMenu->togglePauseMenu();
	}

	if (GlobalContext::pauseMenu->isPaused()) {
		Jauntlet::Time::setTimeScale(0.0f);
	}
	else {
		Jauntlet::Time::setTimeScale(1.0f);
	}
}

void Tutorial::drawGame() {
	GlobalContext::normalShader.use();
	_camera.setActive();

	_drill.drawLayerOne();

	// Draw the player using a spriteBatch
	_playerSpriteBatch.begin();
	_players.draw(_playerSpriteBatch);
	_playerSpriteBatch.endAndRender();
	_officer.draw();

	_drill.drawLayerTwo();

	if (!GlobalContext::pauseMenu->isPaused()) {
		_selectedTile.draw(&_camera);
	}

	_dialogue.update();

	GlobalContext::normalShader.unuse();

	drawHUD();
}

void Tutorial::drawHUD() {
	_hudCamera.setActive();

	_uiCoordinator.draw();
}