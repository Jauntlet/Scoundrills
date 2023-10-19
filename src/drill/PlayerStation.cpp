#include "PlayerStation.h"

PlayerStation::PlayerStation(std::string texture, glm::vec4 destination, glm::vec4 boundingBox, glm::vec2 anchorPointOffset) : 
	_textureID(Jauntlet::ResourceManager::getTexture(texture).id),
	_collider(Jauntlet::BoxCollider2D(glm::vec2(boundingBox.w, boundingBox.z), glm::vec2(boundingBox.x, boundingBox.y))),
	_destination(destination),
	_anchorPoint(glm::vec2(boundingBox.x + (boundingBox.w / 2) + anchorPointOffset.x, boundingBox.y + (boundingBox.z / 2) + anchorPointOffset.y)) 
{

}

void PlayerStation::draw(Jauntlet::SpriteBatch& spriteBatch) {
	spriteBatch.draw(_destination, _textureID);
}

bool PlayerStation::isColliding(glm::vec2 position) {
	// consider creating the Collision2D in the header
	Jauntlet::Collision2D collision;
	return collision.getCollision(&_collider, position);
}
bool PlayerStation::isOccupied() {
	return _occupied;
}

glm::vec2 PlayerStation::getAnchorPoint() {
	return _anchorPoint;
}