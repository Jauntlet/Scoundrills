/* Purpose: A base class for when a player interacts and picks up an item.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Tiles/TileMap.h>
#include <cstdint>
#include <glm/fwd.hpp>

enum class HoldableType {
	WATER, NONE
};

class Holdable
{
public:
	Holdable(const std::string& texture, const glm::vec2& position, const glm::vec2& size, HoldableType type = HoldableType::NONE);

	void pickup();
	// drops the object, putting it ontop of a tile.
	void drop(Jauntlet::TileMap* tilemap);

	// draws the object
	void draw(Jauntlet::SpriteBatch& spriteBatch);

	bool isHeld() const;

	uint32_t requestWater(uint32_t requestedAmt);
	bool isEmpty() const;

	glm::vec2 position;
protected:
	bool _isHeld = false;
	GLuint _textureID;
	glm::vec2 _size;
private:
	HoldableType _itemType;
	union { // its expected that any class override for this will not use this union.
		uint32_t _waterAmount;
	};
};