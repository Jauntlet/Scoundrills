#include <Jauntlet/Collision/Collision2D.h>

#include "PlayerManager.h"

PlayerManager::PlayerManager(int initialPlayers) {
	for (int i = 0; i < initialPlayers; i++) {
		_players.emplace_back(32 * (i + 1), -32);
	}
}
void PlayerManager::addPlayer(Player& player) {
	_players.push_back(player);
}
void PlayerManager::createPlayer(int x, int y) {
	_players.emplace_back(x, y);
}

bool PlayerManager::processInput(Jauntlet::InputManager* inputManager, Jauntlet::Camera2D* activeCamera, Jauntlet::TileMap* navTileMap) {
	if (_selectedPlayer == -1) { // we are selecting a player.
		Jauntlet::Collision2D collision;
		glm::vec2 mousePos = activeCamera->convertScreenToWorld(inputManager->getMouseCoords());

		for (int i = 0; i < _players.size(); i++) {
			if (collision.getCollision(&_players[i].collider, mousePos)) {
				_selectedPlayer = i;
				return true;
			}
		}
	}
	else { // we have selected a position for the player to move to.
		_players[_selectedPlayer].navigateTo(navTileMap, navTileMap->RoundWorldPos(activeCamera->convertScreenToWorld(inputManager->getMouseCoords())));
		_selectedPlayer = -1;
	}

	return false;
}

void PlayerManager::update() {
	for (int i = 0; i < _players.size(); i++) {
		_players[i].update();
	}
}

void PlayerManager::draw(Jauntlet::SpriteBatch& spriteBatch) {
	for (int i = 0; i < _players.size(); i++) {
		_players[i].draw(spriteBatch);
	}
}