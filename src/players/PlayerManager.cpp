#include <Jauntlet/Collision/Collision2D.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include "../scenes/GlobalContext.h"
#include "Jauntlet/Inputs/InputManager.h"
#include "PlayerManager.h"
#include "src/players/Player.h"
#include "../drill/DrillManager.h"

PlayerManager::PlayerManager(DrillManager* drill) :
	_pathRenderer(drill, this),
	_drill(drill),
	_selectButton(&GlobalContext::inputManager)
{
	_players.reserve(sizeof(Player) * 5);
	_selectButton.addKey(SDL_BUTTON_LEFT);
	_selectButton.addKey(CONTROLLER_FACE_EAST, CONTROLLER_FACE_SOUTH);
}

Player* PlayerManager::createPlayer(const glm::vec2& position, uint8_t playerID, bool isCop) {
	if (_players.size() == 5) {
		Jauntlet::error("Player size is already maxed out!");
		return nullptr;
	}
	_players.emplace_back(position, playerID, isCop);
	return &_players.back();
}
Player* PlayerManager::createPlayer(const glm::vec2& position, uint8_t playerID, int health, bool isCop) {
	if (_players.size() == 5) {
		Jauntlet::error("Player size is already maxed out!");
		return nullptr;
	}
	_players.emplace_back(position, playerID, health, isCop);
	return &_players.back();
}

bool PlayerManager::processInput(const Camera2D& activeCamera) {
	glm::vec2 mousePos = GlobalContext::usingController ? activeCamera.getPosition() * (1.0f / activeCamera.getScale()) : activeCamera.convertScreenToWorld(GlobalContext::inputManager.getMouseCoords());
	// if we click
	if (_selectButton.isPressed()) {
		if (_selectedPlayer == -1) { // we are selecting a player.
			Collision2D collision;

			for (size_t i = 0; i < _players.size(); ++i) {
				if (collision.calcCollision(&_players[i].collider, mousePos)) {
					_selectedPlayer = i;
					return true;
				}
				else if (_players[i].getStation() != nullptr && _players[i].getStation()->isColliding(mousePos)) {
					_selectedPlayer = i;
					return true;
				}
			}
		} else { // we have selected a position for the player to move to.
			if (_drill->isValidDestination(_storedMousePos, this) || _drill->checkHoveringStation(mousePos) != nullptr) {
				_players[_selectedPlayer].navigateTo(*_drill, _pathRenderer, mousePos);
			}
			_pathRenderer.clearPath();
			_selectedPlayer = -1;
		}
	} else if (_selectedPlayer != -1) {
		// a player is selected and we aren't clicking, so we draw the path via pathrenderer
		if (_storedMousePos != _drill->drillWalls.RoundWorldPos(mousePos) || _players[_selectedPlayer].isMoving()) {
			_storedMousePos = _drill->drillWalls.RoundWorldPos(mousePos);
			if (_drill->isValidDestination(_storedMousePos, this) || _drill->checkHoveringStation(_storedMousePos) != nullptr) {
				_pathRenderer.createPath(_players[_selectedPlayer].getPosition(), _storedMousePos);
			} else {
				_pathRenderer.clearPath();
			}
		}
	}
	return false;
}
void PlayerManager::damageTick(const int& drillHeat) {
	if (_players.size() == 0) return;

	_damageTick += Jauntlet::Time::getDeltaTime() * (drillHeat / 200.0f);

	if (_damageTick > 2.5) {
		_damageTick = 0;
		int Rand = rand() % _players.size();

		// is true if the player dies
		if (_players[Rand].damage(1)) {
			if (_players[Rand].heldItem != nullptr) {
				_players[Rand].heldItem->drop(&_drill->drillFloor);
			}
			if (_selectedPlayer == Rand) {
				_selectedPlayer = -1;
			}
			_players.erase(_players.begin() + Rand);
		}
	}
}

uint8_t PlayerManager::size() {
	return _players.size();
}

bool PlayerManager::isPlayerSelected() {
	return _selectedPlayer != -1;
}
Player* PlayerManager::getSelectedPlayer() {
	if (_selectedPlayer == -1) {
		return nullptr;
	} else {
		return &_players[_selectedPlayer];
	}
}
std::vector<Player*> PlayerManager::getAllPlayers() {
	std::vector<Player*> output;
	for (size_t i = 0; i < _players.size(); ++i) {
		output.push_back(&_players[i]);
	}
	return output;
}
PathRenderer* PlayerManager::getPathRenderer() {
	return &_pathRenderer;
}

bool PlayerManager::posMatchesPlayerDest(const glm::vec2& worldPos) {
	for (size_t i = 0; i < _players.size(); i++) {
		if (worldPos == _players[i].getDestination() - glm::vec2(0, 64)) {
			return true;
		}
	}
	return false;
}

void PlayerManager::update(DrillManager& drill) {
	for (size_t i = 0; i < _players.size(); ++i) {
		_players[i].update(drill);
	}
}

void PlayerManager::draw(SpriteBatch& spriteBatch) {
	for (size_t i = 0; i < _players.size(); ++i) {
		_players[i].draw(spriteBatch);
	}
	_pathRenderer.drawPath();
}