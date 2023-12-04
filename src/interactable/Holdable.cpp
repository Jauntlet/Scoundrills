#include "Holdable.h"
#include <Jauntlet/Rendering/ResourceManager.h>
#include "../players/Player.h"

Holdable::Holdable(const std::string& texture, const glm::vec2& position, const glm::vec2& size, HoldableType type)
: 
	_textureID(Jauntlet::ResourceManager::getTexture(texture).id),
	position(position),
	_size(size),
	itemType(type)
{
	switch (type) {
		case HoldableType::WATER:
			_waterAmount = 60;
			break;
		default:
			break;
	}
}

Holdable::~Holdable() {
	if (_player != nullptr) {
		_player->forceDropItem();
	}
}

void Holdable::pickup(Player* player) {
	_player = player;
}
void Holdable::drop(Jauntlet::TileMap* tilemap) {
	position = tilemap->RoundWorldPos(position) + glm::vec2(0, 64);
	_player = nullptr;
}
void Holdable::draw(Jauntlet::SpriteBatch& spriteBatch) {
	                          //this weird position math always centers the object on tile regardless of size -xm
	spriteBatch.draw(glm::vec4(position + ((glm::vec2(64) - _size) * 0.5f), _size), _textureID);
}

bool Holdable::isHeld() const {
	return _player != nullptr;
}

uint32_t Holdable::requestWater(uint32_t requestedAmt) {
	if (itemType != HoldableType::WATER) {
		return 0;
	}

	if (_waterAmount <= requestedAmt) {
		requestedAmt = _waterAmount;
		_waterAmount = 0;
	} else {
		_waterAmount -= requestedAmt;
	}
	return requestedAmt;
}
bool Holdable::isEmpty() const {
	if (itemType == HoldableType::WATER) {
		return _waterAmount == 0;
	}
	else return false;
}