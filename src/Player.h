#pragma once

#include <Jauntlet/Collision/Collision2D.h>
#include <Jauntlet/InputKey.h>
#include <Jauntlet/InputManager.h>
#include <Jauntlet/Rendering/SpriteBatch.h>
#include <Jauntlet/TileMap.h>

class Player
{
public:
	Player(float x, float y);

	void update();
	void draw(Jauntlet::SpriteBatch& spriteBatch);
	// Navigate through a tilemap to a position.
	void navigateTo(Jauntlet::TileMap& map, glm::vec2 position);


	void setPosition(float x, float y);
	void setPosition(glm::vec2 pos);

	void setSpeed(float newSpeed);

	glm::vec2 getPosition();

	Jauntlet::CircleCollider2D collider;
private:
	glm::vec2 _position = glm::vec2(0,0);
	std::vector<glm::vec2> _path;

	float _speed = 120;
};