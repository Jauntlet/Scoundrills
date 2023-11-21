/* Purpose: Base class for stations that anchor players to positions to do tasks. Handles pathfinding to location and where that location is.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Collision/Collider2D.h>
#include <Jauntlet/Collision/Collision2D.h>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/SpriteBatch.h>

class Player;

class PlayerStation
{
public:
	// texture: texture you are using for the object.
	// destination: where the texture is drawn: X/Y = Position, W/Z: Width/ Height
	// boundingBox: where the mouse can click on the station: X/Y = Position, W/Z: Width/ Height
	// anchorPointOffset: where the player stands when on the station, offset from the center of the objects bounding box.
	PlayerStation(std::string texture, glm::vec4 destination, glm::vec4 boundingBox, glm::vec2 anchorPointOffset);
	
	// Draw the station on screen
	virtual void draw(Jauntlet::SpriteBatch& spriteBatch);

	// Returns true if position is within the bounding box of the station
	bool isColliding(glm::vec2 position);
	// Returns true if the station is already used by a player.
	bool isOccupied();
	// Occupy a station with a player
	void Occupy(Player* player);

	// Returns the anchor point for the player
	glm::vec2 getAnchorPoint();
	// Returns the collision bounding box of the player station
	glm::vec4 getBoundingBox();
protected:
	bool _occupied = false;

	Jauntlet::BoxCollider2D _collider;
	GLuint _textureID;
	glm::vec4 _destination;
	glm::vec2 _anchorPoint;
};

