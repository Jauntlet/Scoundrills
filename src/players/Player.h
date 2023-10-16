/* Purpose: Controls an individual player, including tasks and pathfinding.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Collision/Collision2D.h>
#include <Jauntlet/InputKey.h>
#include <Jauntlet/InputManager.h>
#include <Jauntlet/Rendering/SpriteBatch.h>
#include <Jauntlet/TileMap.h>
#include "../drill/PlayerStation.h"

class Player
{
public:
	Player(float x, float y);

	void update();
	void draw(Jauntlet::SpriteBatch& spriteBatch);
	// Navigate through a tilemap to a position.
	void navigateTo(Jauntlet::TileMap* map, glm::vec2 position);

	// assign player to specified station.
	void assignStation(PlayerStation* station);
	// unassign player from station.
	void clearStation();

	void setPosition(float x, float y);
	void setPosition(glm::vec2 pos);

	void setSpeed(float newSpeed);

	glm::vec2 getPosition();

	Jauntlet::BoxCollider2D collider;
private:
	glm::vec2 _position = glm::vec2(0,0);
	std::vector<glm::vec2> _path;

	float _speed = 300, _storedVelocity = 0;

	PlayerStation* _station = nullptr;
};