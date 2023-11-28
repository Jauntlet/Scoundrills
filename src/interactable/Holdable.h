/* Purpose: A base class for when a player interacts and picks up an item.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Tiles/TileMap.h>
#include <cstdint>
#include <glm/fwd.hpp>

class Player;

enum class HoldableType {
	WATER, NONE
};

class Holdable
{
public:
	Holdable(const std::string& texture, const glm::vec2& position, const glm::vec2& size, HoldableType type = HoldableType::NONE);

	~Holdable();

	void pickup(Player* player);
	// drops the object, putting it ontop of a tile.
	void drop(Jauntlet::TileMap* tilemap);

	// draws the object
	void draw(Jauntlet::SpriteBatch& spriteBatch);

	bool isHeld() const;
	
	// Attempts to get water from the held object. Returns how much water is actually given to the object.
	// Reasons it won't retrieve the requested water amount:
	// 1. It doesn't hold water, will output 0.
	// 2. It doesn't have enough water, will output how much it does have.
	uint32_t requestWater(uint32_t requestedAmt);

	// returns whether or not the object does not have any 
	bool isEmpty() const;

	glm::vec2 position;
protected:
	Player* _player = nullptr;

	GLuint _textureID;
	glm::vec2 _size;
private:
	HoldableType _itemType;
	union { // its expected that any class override for this will not use this union.
		uint32_t _waterAmount;
	};
};