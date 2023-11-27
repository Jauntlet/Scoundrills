#include "Player.h"
#include "PlayerManager.h"


Player::Player(float x, float y) : collider(Jauntlet::BoxCollider2D(glm::vec2(64), glm::vec2(x,y))) {
	_position = glm::vec2(x, y);
}

void Player::update(DrillManager& drill) {
	
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
			
			if (_path.empty()) {
				// remove all stored velocity.
				_storedVelocity = 0;

				// detect if we landed on an item and pick it up.
				Holdable* holdable = drill.getHoldable(_position);
				if (holdable != nullptr) {
					if (_heldItem != nullptr) {
						_heldItem->drop(&drill.drillWalls);
					}
					holdable->pickup();
					_heldItem = holdable;
				}
			}
		}
		// update position of held item whenever we move
		if (_heldItem != nullptr) {
			_heldItem->position = _position;
		}
	}
	
	//update collider
	collider.position = _position;
}

void Player::draw(Jauntlet::SpriteBatch& spriteBatch) {
	spriteBatch.draw({ _position.x, _position.y, 64, 64 }, Jauntlet::ResourceManager::getTexture("Textures/Craig.png").id, 0);
}

void Player::navigateTo(DrillManager& drill, PlayerManager& playerManager, glm::vec2 position) {
	_path.clear();

	// We add the final destination twice to the vector, because the final vector position for some reason gets destroyed at some point
	// this is a weird bug that only occurs here, as the pathRenderer uses the same method and the result is fine. -xm
	PlayerStation* storedStation;
	if ((storedStation = drill.checkHoveringStation(position)) != nullptr) {
		if (_station == storedStation) {
			// we are already at the correct station
			return;
		}
		
		_station = storedStation;
		if (!_station->isOccupied()) {
			_station->Occupy(this);
			
			_path = Pathfinding::findPath(drill, playerManager, _position, _station->getAnchorPoint());
			_path.erase(_path.begin());
			// pathfind to the position of the station the player was assigned to.
			_path.insert(_path.begin(), _station->getAnchorPoint() - glm::vec2(32, 32));
		}
		else {
			_path = Pathfinding::findPath(drill, playerManager, _position, drill.drillWalls.RoundWorldPos(position));
			_path.erase(_path.begin());
			_station = nullptr;
		}
	}
	else {
		if (_station != nullptr) {
			_station->Occupy(nullptr);
			_station = nullptr;
		}
		_path = Pathfinding::findPath(drill, playerManager, _position, drill.drillWalls.RoundWorldPos(position));
		_path.erase(_path.begin());
		_path.insert(_path.begin(), drill.drillWalls.RoundWorldPos(position));
	}
}

void Player::setSpeed(float newSpeed) {
	_speed = newSpeed;
}

glm::vec2 Player::getPosition() const {
	return _position;
}
glm::vec2 Player::getDestination() const {
	return _path.size() > 0 ? _path[0] : _position;
}