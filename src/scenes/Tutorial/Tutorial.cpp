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

	_camera.setPosition(glm::vec2(24 * 64 * 0.5f, 30 * 64 * 0.5f * -1));
	_camera.setScale(0.5f);
}

void Tutorial::windowResized() {
	_camera.updateCameraSize(GlobalContext::screenSize);
	_hudCamera.updateCameraSize(GlobalContext::screenSize);
	_uiCoordinator.applyNewScreenSize(GlobalContext::screenSize);
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

	_cameraManager.processInput();

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
	_camera.setActiveCamera();

	_drill.drawLayerOne();

	// Draw the player using a spriteBatch
	_playerSpriteBatch.begin();
	_players.draw(_playerSpriteBatch);
	_playerSpriteBatch.endAndRender();

	_drill.drawLayerTwo();

	if (!GlobalContext::pauseMenu->isPaused()) {
		_selectedTile.draw(&_camera);
	}

	GlobalContext::normalShader.unuse();

	drawHUD();
}

void Tutorial::drawHUD() {
	_hudCamera.setActiveCamera();

	_uiCoordinator.draw();
}