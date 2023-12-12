#include "Player.h"
#include "src/interactable/Holdable.h"

Player::Player(const glm::vec2& position, const std::string& texture) :
	collider(Jauntlet::BoxCollider2D(glm::vec2(64), position)),
	_position(position),
	_healthBar("Textures/healthbar.png", glm::vec4(0, 0, 0.5, 0.5), glm::vec4(0.5), glm::vec4(_position.x + 8, _position.y + 68, 48, 8)),
	_texture(Jauntlet::ResourceManager::getTexture(texture).id)
{
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
			} else if (std::abs(direction.x) < _speed * Jauntlet::Time::getDeltaTime() + _storedVelocity) {
				// The character will move to the desired position whilst using some stored velocity.
				_storedVelocity -= direction.x - _speed * Jauntlet::Time::getDeltaTime();
				_position.x += direction.x;
			} else {
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
			} else if (std::abs(direction.y) < _speed * Jauntlet::Time::getDeltaTime() + _storedVelocity) {
				// The character will move to the desired position whilst using some stored velocity.
				_storedVelocity -= direction.y - _speed * Jauntlet::Time::getDeltaTime();
				_position.y += direction.y;
			} else {
				// The character will not reach the desired position, so use all stored velocity
				_position.y += Jauntlet::Time::getDeltaTime() * _speed * glm::sign(direction.y) + glm::sign(direction.y) * _storedVelocity;
				_storedVelocity = 0;
			}
		}

		if (_position == _path.back()) {
			_path.pop_back();
			
			if (_path.empty()) {
				onDestination(drill);
			}
		}
		// update position of held item whenever we move
		if (heldItem != nullptr) {
			heldItem->position = _position;
		}
	}
	
	//update collider
	collider.position = _position;
}

void Player::draw(Jauntlet::SpriteBatch& spriteBatch) {
	spriteBatch.draw({ _position.x, _position.y, 64, 64 }, Jauntlet::ResourceManager::getTexture("Textures/Craig.png").id, 0);

	if (health != 30) {
		_healthBar.setProgress(health / 30);
		_healthBar.setPosition(glm::vec2(_position.x + 8, _position.y + 68));
		_healthBar.draw(spriteBatch);
	}
}

void Player::navigateTo(DrillManager& drill, PathRenderer& pathRenderer, glm::vec2 position) {
	_path.clear();

	PlayerStation* storedStation;
	if ((storedStation = drill.checkHoveringStation(position)) != nullptr) {
		if (_station == storedStation) {
			// we are already at the correct station
			return;
		} else if (_station != nullptr) {
			// unoccupy station we are already at
			_station->unoccupy();
		}
		
		if (_pipeDest != nullptr) {
			delete _pipeDest;
			_pipeDest = nullptr;
		}
		
		_station = storedStation;
		if (!_station->isOccupied()) {
			_station->occupy();
			
			_path = pathRenderer.getPath();
			
			if (_path.empty()) {
				return;
			}
			_path.erase(_path.begin());
			
			// pathfind to the position of the station the player was assigned to.
			_path.insert(_path.begin(), _station->getAnchorPoint() - glm::vec2(32, 32));
		} else {
			_path = pathRenderer.getPath();
			
			if (_path.empty()) {
				return;
			}
			
			_path.erase(_path.begin());
			_station = nullptr;
		}
	} else {
		if (_station != nullptr) {
			_station->unoccupy();
			_station = nullptr;
		}
		_path = pathRenderer.getPath();
		if (_path.empty()) return;
		_path.erase(_path.begin());
		if (!drill.DestMatchesRandomPipe(drill.pipes.RoundWorldPos(position))) {
			if (_pipeDest != nullptr) {
				delete _pipeDest;
				_pipeDest = nullptr;
			}
			_path.insert(_path.begin(), drill.drillWalls.RoundWorldPos(position));
		} else {
			_pipeDest = new glm::vec2(position);
		}
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

PlayerStation* Player::getStation() const {
	return _station;
}

void Player::forceDropItem() {
	heldItem = nullptr;
}

void Player::onDestination(DrillManager& drill) {
	// remove all stored velocity.
	_storedVelocity = 0;

	// detect if we landed on an item and pick it up.
	Holdable* holdable = drill.getHoldable(_position);
	if (holdable != nullptr) {
		if (heldItem != nullptr) {
			heldItem->position = _position;
			heldItem->drop(&drill.drillWalls);
		}
		holdable->pickup(this);
		heldItem = holdable;
	}

	// run code for station on players arrival
	if (_station != nullptr) {
		_station->onPlayerArrival(*this);
	}

	// if destination is pipe, we try to repair it.
	if (_pipeDest != nullptr) {
		if (heldItem != nullptr && heldItem->itemType == HoldableType::PIPE) {
			// repair the pipe
			drill.repairPipe(drill.pipes.RoundWorldPos(*_pipeDest));
			// remove stored destination
			delete _pipeDest;
			_pipeDest = nullptr;
			// destroy the held item
			drill.removeHoldable(heldItem);
		}
	}
}