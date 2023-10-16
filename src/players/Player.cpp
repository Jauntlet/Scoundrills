#include <Jauntlet/JMath.h>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/Vertex.h>
#include <Jauntlet/Time.h>
#include <SDL/SDL.h>
#include <algorithm>

#include "../pathfinding/Pathfinding.h"
#include "Player.h"

Player::Player(float x, float y) : collider(Jauntlet::BoxCollider2D(glm::vec2(64), glm::vec2(x,y))) {
	_position = glm::vec2(x, y);
}

void Player::update() {
	
	// we have a path to follow
	if (!_path.empty()) {
		glm::vec2 direction = glm::vec2(_path.back().x - _position.x, _path.back().y - _position.y);

		if (direction.x != 0) {
			if (std::abs(direction.x) < _speed * Jauntlet::Time::getDeltaTime()) {
				// The character will move to the desired position with momentum leftover, so we store the velocity for the next movement.
				_position.x += direction.x;
				_storedVelocity.x += Jauntlet::Time::getDeltaTime() * _speed - std::abs(direction.x);
			}
			else if (std::abs(direction.x) < _speed * Jauntlet::Time::getDeltaTime() + _storedVelocity.x) {
				// The character will move to the desired position whilst using some stored velocity.
				_storedVelocity.x -= direction.x - _speed * Jauntlet::Time::getDeltaTime();
				_position.x += direction.x;
			}
			else {
				// The character will not reach the desired position, so use all stored velocity
				_position.x += Jauntlet::Time::getDeltaTime() * _speed * glm::sign(direction.x) + glm::sign(direction.x) * _storedVelocity.x;
				_storedVelocity.x = 0;
			}
		}

		if (direction.y != 0) {
			if (std::abs(direction.y) < _speed * Jauntlet::Time::getDeltaTime()) {
				// The character will move to the desired position with momentum leftover, so we store the velocity for the next movement.
				_position.y += direction.y;
				_storedVelocity.y += Jauntlet::Time::getDeltaTime() * _speed - std::abs(direction.y);
			}
			else if (std::abs(direction.y) < _speed * Jauntlet::Time::getDeltaTime() + _storedVelocity.y) {
				// The character will move to the desired position whilst using some stored velocity.
				_storedVelocity.y -= direction.y - _speed * Jauntlet::Time::getDeltaTime();
				_position.y += direction.y;
			}
			else {
				// The character will not reach the desired position, so use all stored velocity
				_position.y += Jauntlet::Time::getDeltaTime() * _speed * glm::sign(direction.y) + glm::sign(direction.y) * _storedVelocity.y;
				_storedVelocity.y = 0;
			}
		}



		if (_position == _path.back()) {
			_path.pop_back();
			
			// remove all stored velocity if destination is reached.
			if (_path.empty()) {
				_storedVelocity = glm::vec2(0);
			}
		}
	}
	
	//update collider
	collider.position = _position;
}

void Player::draw(Jauntlet::SpriteBatch& spriteBatch) {
	glm::vec4 destRect = glm::vec4(_position.x, _position.y, 64, 64);

	spriteBatch.draw(destRect, Jauntlet::ResourceManager::getTexture("Textures/Craig.png").id, 0);
}

void Player::navigateTo(Jauntlet::TileMap* map, glm::vec2 position) {
	_path.clear();

	_path = Pathfinding::findPath(map, _position, position);
	_path.erase(_path.begin());
	// if the final destination is in a valid position, add it to the vector again. This is because when starting to pathfind, the final destination
	// gets removed at some point.. This is weird, but it does allow us to prevent going to a bad final destination. -xm
	if (!map->tileHasCollision(map->WorldPosToTilePos(position)) && map->isValidTilePos(map->WorldPosToTilePos(position))) {
		// pathfind to the position of the station the player was assigned to.
		if (_station != nullptr) {
			_path.insert(_path.begin(), _station->getAnchorPoint() - glm::vec2(32, 32));
		}
		else {
			_path.insert(_path.begin(), position);
		}
	}
}

void Player::assignStation(PlayerStation* station) {
	_station = station;
}
void Player::clearStation() {
	_station = nullptr;
}

void Player::setPosition(float x, float y) {
	_position = glm::vec2(x, y);
}
void Player::setPosition(glm::vec2 pos) {
	_position = pos;
	collider.position = pos;
}

void Player::setSpeed(float newSpeed) {
	_speed = newSpeed;
}

glm::vec2 Player::getPosition() {
	return _position;
}