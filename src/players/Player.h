/* Purpose: Controls an individual player, including tasks and pathfinding.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Inputs/InputKey.h>
#include "../drill/DrillManager.h"
#include "../interactable/PlayerStation.h"
#include "../interactable/Holdable.h"
#include "../pathfinding/PathRenderer.h"
#include <Jauntlet/Rendering/ProgressBar.h>
#include <Jauntlet/Rendering/Animation/Animation.h>
#include <Jauntlet/Audio/AudioSource.h>

class Player
{
public:
	Player(const glm::vec2& position, uint8_t playerID, bool isCop = false);
	Player(const glm::vec2& position, uint8_t playerID, int health, bool isCop = false);
	~Player();

	void update(DrillManager& drill);
	void draw(Jauntlet::SpriteBatch& spriteBatch);
	// Navigate through a tilemap to a position.
	// this function expects a world position not rounded to a tile, so that it can check what player stations it may be connected to.
	void navigateTo(DrillManager& drill, PathRenderer& pathRenderer, glm::vec2 position);

	void setSpeed(float newSpeed);
	// Returns true if the player dies
	bool damage(int damage);
	// heal the player by specified amount of health
	void heal(int heal);
	// returns health of the player
	int getHealth() const;
	// returns if the player is currently moving
	bool isMoving() const;

	glm::vec2 getPosition() const;
	glm::vec2 getDestination() const;
	PlayerStation* getStation() const;
	// this is specifically for saving players; player ID in the playerManager is different.
	uint16_t getPlayerID() const;

	// forces the player to drop his item; called when the item is destroyed
	void forceDropItem();

	// forces the player to a specific location; unoccupies any station attached to them
	void forcePosition(glm::vec2 newPos);

	void forceOccupyStation(PlayerStation* station);

	// forces data onto the player
	void forceData(float positionX, float positionY, int heldItemID, int health, int playerID);

	Jauntlet::BoxCollider2D collider;
	Holdable* heldItem = nullptr;
private:
	// runs when player reaches destination
	void onDestination(DrillManager& drill);

	glm::vec2 _position = glm::vec2(0,0);
	std::vector<glm::vec2> _path;

	int _health = 30;
	float _speed = 300.0f, _storedVelocity = 0.0f;

	PlayerStation* _station = nullptr;
	glm::vec2* _pipeDest = nullptr;

	// Rendering
	GLuint _texture;
	Jauntlet::Animation _animation = Jauntlet::Animation(28);
	Jauntlet::ProgressBar _healthBar;
	glm::vec2 _moveDir = glm::vec2(0);
	bool _flipped = false, _moving = false;

	// Sounds
	Jauntlet::AudioSource _soundSource = Jauntlet::AudioSource(glm::vec3(_position, 0));

	// data stored specifically for saving
	uint8_t _playerID = 0;	
};