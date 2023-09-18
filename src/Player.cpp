#include <Jauntlet/JMath.h>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/Vertex.h>
#include <Jauntlet/Time.h>
#include <SDL/SDL.h>
#include <algorithm>

#include "Pathfinding.h"
#include "Player.h"

#include <iostream> // remove when done debugging
Player::Player(float x, float y) : collider(Jauntlet::CircleCollider2D(16.0f, glm::vec2(x,y))) {
	_position = glm::vec2(x, y);
}

void Player::update() {
	// we have a path to follow
	if (!_path.empty()) {
		glm::vec2 direction = glm::sign(glm::vec2(_path[0].x - _position.x, _path[0].y - _position.y));

		if (direction.x > 0) {
			_position.x += std::min(direction.x * Jauntlet::Time::getDeltaTime() * _speed, _path[0].x - _position.x);
		}
		else {
			_position.x += std::max(direction.x * Jauntlet::Time::getDeltaTime() * _speed, _path[0].x - _position.x);
		}
		if (direction.y > 0) {
			_position.y += std::min(direction.y * Jauntlet::Time::getDeltaTime() * _speed, _path[0].y - _position.y);
		}
		else {
			_position.y += std::max(direction.y * Jauntlet::Time::getDeltaTime() * _speed, _path[0].y - _position.y);
		}

		if (_position == _path[0]) {
			_path[0] = _path.back();
			_path.pop_back();
		}
	}
	
	//update collider
	collider.position = _position;
}

void Player::draw(Jauntlet::SpriteBatch& spriteBatch) {
	glm::vec4 destRect = glm::vec4(_position.x, _position.y, 32, 32);

	spriteBatch.draw(destRect, glm::vec4(0,0,1,1), Jauntlet::ResourceManager::getTexture("Textures/Craig.png").id, 0, Jauntlet::Color(255, 255, 255, 255));
}

void Player::navigateTo(Jauntlet::TileMap& map, glm::vec2 position) {
	_path.clear();

	_path = Pathfinding::findPath(map, _position, position, false);
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