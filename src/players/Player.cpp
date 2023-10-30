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
				_storedVelocity += Jauntlet::Time::getDeltaTime() * _speed - std::abs(direction.x);
			}
			else if (std::abs(direction.x) < _speed * Jauntlet::Time::getDeltaTime() + _storedVelocity) {
				// The character will move to the desired position whilst using some stored velocity.
				_storedVelocity -= direction.x - _speed * Jauntlet::Time::getDeltaTime();
				_position.x += direction.x;
			}
			else {
				// The character will not reach the desired position, so use all stored velocity
				_position.x += Jauntlet::Time::getDeltaTime() * _speed * glm::sign(direction.x) + glm::sign(direction.x) * _storedVelocity;
				_storedVelocity = 0;
			}
		}

		if (direction.y != 0) {
			if (std::abs(direction.y) < _speed * Jauntlet::Time::getDeltaTime()) {
				// The character will move to the desired position with momentum leftover, so we store the velocity for the next movement.
				_position.y += direction.y;
				_storedVelocity += Jauntlet::Time::getDeltaTime() * _speed - std::abs(direction.y);
			}
			else if (std::abs(direction.y) < _speed * Jauntlet::Time::getDeltaTime() + _storedVelocity) {
				// The character will move to the desired position whilst using some stored velocity.
				_storedVelocity -= direction.y - _speed * Jauntlet::Time::getDeltaTime();
				_position.y += direction.y;
			}
			else {
				// The character will not reach the desired position, so use all stored velocity
				_position.y += Jauntlet::Time::getDeltaTime() * _speed * glm::sign(direction.y) + glm::sign(direction.y) * _storedVelocity;
				_storedVelocity = 0;
			}
		}

		if (_position == _path.back()) {
			_path.pop_back();
			
			// remove all stored velocity if destination is reached.
			if (_path.empty()) {
				_storedVelocity = 0;
			}
		}
	}
	
	//update collider
	collider.position = _position;
}

void Player::draw(Jauntlet::SpriteBatch& spriteBatch) {
	spriteBatch.draw({ _position.x, _position.y, 64, 64 }, Jauntlet::ResourceManager::getTexture("Textures/Craig.png").id, 0);
}

void Player::navigateTo(DrillManager* drill, glm::vec2 position) {
	_path.clear();

	_path = Pathfinding::findPath(&drill->drillWalls, _position, drill->drillWalls.RoundWorldPos(position));

	_path.erase(_path.begin());

	// We add the final destination twice to the vector, because the final vector position for some reason gets destroyed at some point
	// this is a weird bug that only occurs here, as the pathRenderer uses the same method and the result is fine. -xm

	if (drill->checkHoveringStation(position) != nullptr) {
		_station = drill->checkHoveringStation(position);
		if (!_station->isOccupied()) {
			_station->Occupy(this);
			// pathfind to the position of the station the player was assigned to.
			_path.insert(_path.begin(), _station->getAnchorPoint() - glm::vec2(32, 32));
		}
		else {
			_station = nullptr;
		}
	}
	else {
		if (_station != nullptr) {
			_station->Occupy(nullptr);
			_station = nullptr;
		}
		_path.insert(_path.begin(), drill->drillWalls.RoundWorldPos(position));
	}
}

void Player::setSpeed(float newSpeed) {
	_speed = newSpeed;
}

glm::vec2 Player::getPosition() {
	return _position;
}