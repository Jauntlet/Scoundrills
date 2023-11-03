/* Purpose: Controls an individual player, including tasks and pathfinding.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Inputs/InputKey.h>
#include "../drill/DrillManager.h"
#include "../drill/PlayerStation.h"

class Player
{
public:
	Player(float x, float y);

	void update();
	void draw(Jauntlet::SpriteBatch& spriteBatch);
	// Navigate through a tilemap to a position.
	// this function expects a world position not rounded to a tile, so that it can check what player stations it may be connected to.
	void navigateTo(DrillManager* drill, glm::vec2 position);

	void setSpeed(float newSpeed);

	glm::vec2 getPosition();

	Jauntlet::BoxCollider2D collider;
private:
	glm::vec2 _position = glm::vec2(0,0);
	std::vector<glm::vec2> _path;

	float _speed = 300, _storedVelocity = 0;

	PlayerStation* _station = nullptr;
};