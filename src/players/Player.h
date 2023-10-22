/* Purpose: Controls an individual player, including tasks and pathfinding.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/InputKey.h>
#include "../drill/DrillManager.h"
#include "../drill/PlayerStation.h"

class Player
{
public:
	Player(float x, float y);

	void update();
	void draw(Jauntlet::SpriteBatch& spriteBatch);
	// Navigate through a tilemap to a position.
	void navigateTo(Jauntlet::TileMap* map, glm::vec2 position);


	void setSpeed(float newSpeed);

	glm::vec2 getPosition();

	Jauntlet::BoxCollider2D collider;
private:
	glm::vec2 _position = glm::vec2(0,0);
	std::vector<glm::vec2> _path;

	float _speed = 300, _storedVelocity = 0;

	PlayerStation* _station = nullptr;
};