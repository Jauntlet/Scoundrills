#include "../GlobalContext.h"
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <iostream>
#include "MainGame.h"
#include "UICoordinator.h"
#include "../PauseMenu.h"

const float PLAYER_HURT_HEAT = 300.0f; // The minimum heat for players to take damage from it.

MainGame::MainGame(int saveID, const std::vector<uint8_t>& playerIDs) :
	_resources(80,0,0,0),
	_database(saveID)
{
	_saveID = saveID;
	
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(glm::ivec2(GlobalContext::screenSize.x, GlobalContext::screenSize.y));

	for (size_t i = 0; i < playerIDs.size(); ++i) {
		_players.createPlayer(glm::vec2(64 * (i + 1) + 5 * 64, -64 * 18), playerIDs[i]);
	}

	_camera.setPosition(glm::vec2(24 * 64 * 0.5f, 30 * 64 * 0.5f * -1));
	_camera.setScale(0.5f);

	//there isn't currently an intuitive way to get the playerManager into the cavern class so I just put it here. TODO: make this comment sound smart
	_uiCoordinator.navigation->setCavernPlayerManager(&_players);

	// save
	//_database.TrySave(_drill, _players);
}

MainGame::MainGame(int saveID) :
	_database(saveID)
{
	_saveID = saveID;

	GlobalContext::window.setBackgroundColor(Jauntlet::Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(glm::ivec2(GlobalContext::screenSize.x, GlobalContext::screenSize.y));

	// load existing data here.
	_database.Load(_drill, _players);

	//set the cavern's player manager
	_uiCoordinator.navigation->setCavernPlayerManager(&_players);
}

void MainGame::gameLoop() {
	processInput();

	_camera.update();
	_hudCamera.update();
	_drill.update(&_uiCoordinator);

	drawGame();
}

void MainGame::processInput() {
	if (_players.getAllPlayers().size() == 0) {
		return;
	}

	if (_wasDrillMoving && !_drill.navigation.getMoving()) {
		_database.TrySave(_drill, _players);
	}
	_wasDrillMoving = _drill.navigation.getMoving();

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
	if (GlobalContext::inputManager.isKeyPressed(SDLK_TAB) && _drill.isSteeringWheelOccupied()) {
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