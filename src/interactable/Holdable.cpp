#include "Holdable.h"

Holdable::Holdable(const std::string& texture, const glm::vec2& position, const glm::vec2& size)
: 
	_textureID(Jauntlet::ResourceManager::getTexture(texture).id),
	position(position),
	_size(size)
{
	
}

void Holdable::drop(Jauntlet::TileMap* tilemap) {
	position = tilemap->RoundWorldPos(position);
}
void Holdable::draw(Jauntlet::SpriteBatch& spriteBatch) {
	spriteBatch.draw(glm::vec4(position, _size), _textureID);
}