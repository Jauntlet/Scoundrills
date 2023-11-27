#include "Holdable.h"
#include <Jauntlet/Rendering/ResourceManager.h>

Holdable::Holdable(const std::string& texture, const glm::vec2& position, const glm::vec2& size)
: 
	_textureID(Jauntlet::ResourceManager::getTexture(texture).id),
	position(position),
	_size(size)
{
	
}

void Holdable::pickup() {
	_isHeld = true;
}
void Holdable::drop(Jauntlet::TileMap* tilemap) {
	position = tilemap->RoundWorldPos(position) + glm::vec2(0, 64);
	_isHeld = false;
}
void Holdable::draw(Jauntlet::SpriteBatch& spriteBatch) {
	                          //this weird position math always centers the object on tile regardless of size -xm
	spriteBatch.draw(glm::vec4(position + ((glm::vec2(64) - _size) * 0.5f), _size), _textureID);
}

bool Holdable::isHeld() const {
	return _isHeld;
}