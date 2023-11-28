/* Purpose: Controls an individual player, including tasks and pathfinding.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Inputs/InputKey.h>
#include "../drill/DrillManager.h"
#include "../interactable/PlayerStation.h"
#include "../interactable/Holdable.h"
#include "../pathfinding/Pathfinding.h"

// TODO: remove the need to include playermanager in players.
// I do not like doing this, it feels wrong... but we have a game to make for now. -xm
class PlayerManager;

class Player
{
public:
	Player(float x, float y);

	void update(DrillManager& drill);
	void draw(Jauntlet::SpriteBatch& spriteBatch);
	// Navigate through a tilemap to a position.
	// this function expects a world position not rounded to a tile, so that it can check what player stations it may be connected to.
	void navigateTo(DrillManager& drill, PlayerManager& playerManager, glm::vec2 position);

	void setSpeed(float newSpeed);

	glm::vec2 getPosition() const;
	glm::vec2 getDestination() const;

	// forces the player to drop his item; called when the item is destroyed
	void forceDropItem();

	Jauntlet::BoxCollider2D collider;
	Holdable* heldItem = nullptr;
private:
	glm::vec2 _position = glm::vec2(0,0);
	std::vector<glm::vec2> _path;

	float _speed = 300, _storedVelocity = 0;

	PlayerStation* _station = nullptr;
};