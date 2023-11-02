#include <Jauntlet/Collision/Collision2D.h>

#include "PlayerManager.h"

PlayerManager::PlayerManager(int initialPlayers, DrillManager* drill)
	: 
	_drill(drill),
	_pathRenderer(&drill->drillWalls, this) {
	_players.reserve(sizeof(Player) * initialPlayers);

	for (int i = 0; i < initialPlayers; ++i) {
		_players.emplace_back(64 * (i + 1) + 320, -64);
	}
}
void PlayerManager::addPlayer(Player& player) {
	_players.push_back(player);
}
void PlayerManager::createPlayer(int x, int y) {
	_players.emplace_back(x, y);
}

bool PlayerManager::processInput(Jauntlet::InputManager* inputManager, Jauntlet::Camera2D* activeCamera) {
	// if we click
	if (inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		if (_selectedPlayer == -1) { // we are selecting a player.
			Jauntlet::Collision2D collision;
			glm::vec2 mousePos = activeCamera->convertScreenToWorld(inputManager->getMouseCoords());

			for (int i = 0; i < _players.size(); ++i) {
				if (collision.getCollision(&_players[i].collider, mousePos)) {
					_selectedPlayer = i;
					return true;
				}
			}
		}
		else { // we have selected a position for the player to move to.
			if (isValidDestination(_storedMousePos)) {
				_players[_selectedPlayer].navigateTo(_drill, activeCamera->convertScreenToWorld(inputManager->getMouseCoords()));
			}
			_pathRenderer.clearPath();
			_selectedPlayer = -1;
		}
	} else if (_selectedPlayer != -1) {
		// a player is selected and we aren't clicking, so we draw the path via pathrenderer
		if (_storedMousePos != _drill->drillWalls.RoundWorldPos(activeCamera->convertScreenToWorld(inputManager->getMouseCoords()))) {
			_storedMousePos = _drill->drillWalls.RoundWorldPos(activeCamera->convertScreenToWorld(inputManager->getMouseCoords()));
			if (isValidDestination(_storedMousePos)) {
				_pathRenderer.createPath(_players[_selectedPlayer].getPosition(), _storedMousePos);
			}
			else {
				_pathRenderer.clearPath();
			}
		}
	}
	return false;
}
bool PlayerManager::isValidDestination(glm::vec2 worldPos) {
	glm::ivec2 pos = _drill->drillWalls.WorldPosToTilePos(worldPos);
	glm::vec2 floorPos = _drill->drillFloor.WorldPosToTilePos(worldPos);
	worldPos = _drill->drillWalls.RoundWorldPos(worldPos);


	if (_drill->drillWalls.tileHasCollision(pos) || !_drill->drillWalls.isValidTilePos(pos)) {
		return false;
	}
	if (_drill->drillFloor.isTileEmpty(floorPos)) {
		return false;
	}

	// check if position overlaps another player
	for (int i = 0; i < _players.size(); i++) {
		if (worldPos == _players[i].getPosition() - glm::vec2(0, 64)) {
			return false;
		}
	}
	return !_drill->doesTileOverlapStations(pos);
}
bool PlayerManager::hoveringStation(const glm::vec2& worldPos) {
	return _drill->checkHoveringStation(worldPos);
}

bool PlayerManager::isPlayerSelected() {
	return _selectedPlayer != -1;
}

void PlayerManager::update() {
	for (int i = 0; i < _players.size(); ++i) {
		_players[i].update();
	}
}

void PlayerManager::draw(Jauntlet::SpriteBatch& spriteBatch) {
	for (int i = 0; i < _players.size(); ++i) {
		_players[i].draw(spriteBatch);
	}
	_pathRenderer.drawPath();
}