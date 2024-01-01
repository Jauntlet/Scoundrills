#include "../GlobalContext.h"
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include "MainGame.h"
#include "UICoordinator.h"
#include "../PauseMenu.h"
#include "../../Database.h"

const float PLAYER_HURT_HEAT = 200.0f; // The minimum heat for players to take damage from it.

MainGame::MainGame(const std::vector<uint8_t>& playerIDs) :
	_resources(100,100,0,0)
{
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(glm::ivec2(GlobalContext::screenSize.x, GlobalContext::screenSize.y));

	for (int i = 0; i < playerIDs.size(); ++i) {
		_players.createPlayer(glm::vec2(64 * (i + 1) + 704, -64 * 10), GlobalContext::playerIDtoTexture(playerIDs[i]));
	}

	_camera.setPosition(glm::vec2(24 * 64 * 0.5f, 30 * 64 * 0.5f * -1));
	_camera.setScale(0.5f);
}

MainGame::MainGame(int saveID) {
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(glm::ivec2(GlobalContext::screenSize.x, GlobalContext::screenSize.y));

	Database database = Database(saveID);
	// Loading database stuff here
}

void MainGame::gameLoop() {
	processInput();

	_camera.update();
	_hudCamera.update();
	_drill.update();

	drawGame();
}

void MainGame::processInput() {
	if (_players.getAllPlayers().size() == 0) {
		return;
	}
	
	_players.update(_drill);

	if (_resources.heat > PLAYER_HURT_HEAT) {
		_players.damageTick(_resources.heat);

		if (_players.getAllPlayers().size() == 0) {
			Jauntlet::Time::setTimeScale(0);
			_uiCoordinator.showLoseScreen();
			return;
		}
	}

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
	} else {
		Jauntlet::Time::setTimeScale(1.0f);
	}
}

void MainGame::drawGame() {	
	GlobalContext::normalShader.use();
	_camera.setActive();

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

void MainGame::drawHUD() {
	_hudCamera.setActive();

	_uiCoordinator.draw();
}

void MainGame::windowResized() {
	_camera.updateSize(GlobalContext::screenSize);

	_hudCamera.updateSize(GlobalContext::screenSize);
	_uiCoordinator.applyNewScreenSize(GlobalContext::screenSize);
}