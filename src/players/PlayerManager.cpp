#include <Jauntlet/Collision/Collision2D.h>
#include "../scenes/GlobalContext.h"
#include "PlayerManager.h"
#include "src/players/Player.h"
#include "../drill/DrillManager.h"

PlayerManager::PlayerManager(DrillManager* drill) :
	_pathRenderer(drill, this),
	_drill(drill)
{
	// Empty
}

void PlayerManager::createPlayer(const glm::vec2& position, const std::string& texture) {
	_players.emplace_back(position, texture);
}

bool PlayerManager::processInput(const Jauntlet::Camera2D& activeCamera) {
	glm::vec2 mousePos = activeCamera.convertScreenToWorld(GlobalContext::inputManager.getMouseCoords());
	// if we click
	if (GlobalContext::inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		if (_selectedPlayer == -1) { // we are selecting a player.
			Jauntlet::Collision2D collision;

			for (int i = 0; i < _players.size(); ++i) {
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
		if (_storedMousePos != _drill->drillWalls.RoundWorldPos(mousePos)) {
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
	_damageTick += Jauntlet::Time::getDeltaTime() * (drillHeat / 200);

	if (_damageTick > 2.5) {
		_damageTick = 0;
		int Rand = rand() % _players.size();
		_players[Rand].health -= 1;

		if (_players[Rand].health == 0) {
			if (_players[Rand].heldItem != nullptr) {
				_players[Rand].heldItem->drop(&_drill->drillFloor);
			}
			_players.erase(_players.begin() + Rand);
		}
	}
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
	for (int i = 0; i < _players.size(); ++i) {
		output.push_back(&_players[i]);
	}
	return output;
}

bool PlayerManager::posMatchesPlayerDest(const glm::vec2& worldPos) {
	for (int i = 0; i < _players.size(); i++) {
		if (worldPos == _players[i].getDestination() - glm::vec2(0, 64)) {
			return true;
		}
	}
	return false;
}

void PlayerManager::update(DrillManager& drill) {
	for (int i = 0; i < _players.size(); ++i) {
		_players[i].update(drill);
	}
}

void PlayerManager::draw(Jauntlet::SpriteBatch& spriteBatch) {
	for (int i = 0; i < _players.size(); ++i) {
		_players[i].draw(spriteBatch);
	}
	_pathRenderer.drawPath();
}