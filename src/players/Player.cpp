#include "Player.h"
#include "src/interactable/Holdable.h"
#include "../scenes/GlobalContext.h"

// we overwrite the defined min by windows because windows *really* likes its own min function that is not used elsewhere
#pragma warning (disable:4005)
#define min min

Player::Player(const glm::vec2& position, uint8_t playerID, bool isCop) :
	collider(Jauntlet::BoxCollider2D(glm::vec2(64), position)),
	_position(position),
	_healthBar("Textures/healthbar.png", glm::vec4(0, 0, 0.5, 1), glm::vec4(0.5,0,0.5,1), glm::vec4(_position.x + 8, _position.y + 68, 48, 8)),
	_playerID(playerID),
	_texture(Jauntlet::ResourceManager::getTexture(GlobalContext::playerIDtoTexture(playerID, isCop)).id)
{
	_animation.stop(0);
	_animation.play(0,1,0.5f);
}
Player::Player(const glm::vec2& position, uint8_t playerID, int health, bool isCop) :
	collider(Jauntlet::BoxCollider2D(glm::vec2(64), position)),
	_position(position),
	_healthBar("Textures/healthbar.png", glm::vec4(0, 0, 0.5, 1), glm::vec4(0.5, 0, 0.5, 1), glm::vec4(_position.x + 8, _position.y + 68, 48, 8)),
	_playerID(playerID),
	_texture(Jauntlet::ResourceManager::getTexture(GlobalContext::playerIDtoTexture(playerID, isCop)).id),
	_health(health)
{
	_animation.stop(0);
	_animation.play(0, 1, 0.5f);
}
Player::~Player() {
	if (_station != nullptr) {
		_station->unoccupy();
	}
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
			else {
				// Update animation
				direction = glm::vec2(glm::sign(_path.back().x - _position.x), glm::sign(_path.back().y - _position.y));
				
				if (direction != _moveDir) {
					_moveDir = direction;
					if (_moveDir.x != 0) {
						_flipped = _moveDir.x < 0;
						_animation.play(22, 27, 0.05f);
					} else if (_moveDir.y > 0) {
						_flipped = false;
						_animation.play(12, 19, 0.025f);
					}
					else {
						_flipped = false;
						_animation.play(2, 9, 0.025f);
					}
				}
			}
		}
		// update position of held item whenever we move
		if (heldItem != nullptr) {
			heldItem->position = _position;
		}
	}
	_animation.update();
	_soundSource.update();

	//update collider
	collider.position = _position;
	_healthBar.position = _position + glm::vec2(8, 68);
}

void Player::draw(Jauntlet::SpriteBatch& spriteBatch) {
	if (_flipped) {
		spriteBatch.draw({ _position.x + 64, _position.y, -64, 64 }, _animation.getUV(), _texture, 0);
	}
	else {
		spriteBatch.draw({ _position.x, _position.y, 64, 64 }, _animation.getUV(), _texture, 0);
	}

	if (_health != 30) {
		_healthBar.progress = _health / 30.0f;
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

		_moveDir = glm::vec2(glm::sign(_path.back().x - _position.x), glm::sign(_path.back().y - _position.y));
		if (_moveDir.x != 0) {
			_flipped = _moveDir.x < 0;
			_animation.play(22, 27, 0.05f);
		}
		else if (_moveDir.y > 0) {
			_flipped = false;
			_animation.play(12, 19, 0.05f);
		}
		else {
			_flipped = false;
			_animation.play(2, 9, 0.05f);
		}

		_moving = true;
	}
}

void Player::setSpeed(float newSpeed) {
	_speed = newSpeed;
}
bool Player::damage(int damage) {
	_health -= damage;

	_soundSource.setPosition(glm::vec3(_position, 0));
	_soundSource.playWAV("Sounds/hurt.wav");

	return _health <= 0;
}
void Player::heal(int heal) {
	_health = glm::min(30, _health + heal);
}
int Player::getHealth() const {
	return _health;
}
bool Player::isMoving() const {
	return _moving;
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
uint16_t Player::getPlayerID() const {
	return _playerID;
}

void Player::forceDropItem() {
	heldItem = nullptr;
}

void Player::forcePosition(glm::vec2 newPos) {
	if (_station != nullptr) {
		_station->unoccupy();
	}
	_position = newPos;
}

void Player::forceOccupyStation(PlayerStation* station) {
	_station = station;
	station->occupy();
}

void Player::forceData(float positionX, float positionY, int heldItemID, int health, int playerID) {
	_position = glm::vec2(positionX, positionY);
	_health = health;
	_playerID = playerID;
}

void Player::onDestination(DrillManager& drill) {
	// remove all stored velocity.
	_storedVelocity = 0;

	// Play idle animation
	if (_moveDir.x != 0) {
		_flipped = _moveDir.x < 0;
		_animation.stop(20);
		_animation.play(20, 21, 0.5f);
	} else if (_moveDir.y > 0) {
		_flipped = false;
		_animation.stop(10);
		_animation.play(10, 11, 0.5f);
	} else {
		_flipped = true;
		_animation.stop(0);
		_animation.play(0, 1, 0.5f);
	}

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

	_moving = false;
}