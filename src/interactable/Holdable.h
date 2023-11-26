/* Purpose: A base class for when a player interacts and picks up an item.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "../players/Player.h"
class Holdable
{
public:
	Holdable(const std::string& texture, const glm::vec2& position, const glm::vec2& size);

	// drops the object, putting it ontop of a tile.
	void drop(Jauntlet::TileMap* tilemap);

	// draws the object
	void draw(Jauntlet::SpriteBatch& spriteBatch);

	glm::vec2 position;
protected:
	GLuint _textureID;
	glm::vec2 _size;
};

