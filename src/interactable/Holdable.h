/* Purpose: A base class for when a player interacts and picks up an item.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Tiles/TileMap.h>
class Holdable
{
public:
	Holdable(const std::string& texture, const glm::vec2& position, const glm::vec2& size);

	void pickup();
	// drops the object, putting it ontop of a tile.
	void drop(Jauntlet::TileMap* tilemap);

	// draws the object
	void draw(Jauntlet::SpriteBatch& spriteBatch);

	bool isHeld();

	glm::vec2 position;
protected:
	bool _isHeld = false;
	GLuint _textureID;
	glm::vec2 _size;
};