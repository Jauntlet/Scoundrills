#include "AnimatedPlayerStation.h"

AnimatedPlayerStation::AnimatedPlayerStation(std::string texture, glm::vec4 destination, unsigned int frames, glm::vec4 boundingBox, glm::vec2 anchorPointOffset) :
	PlayerStation(texture, destination, boundingBox, anchorPointOffset), animation(frames)
{
}

void AnimatedPlayerStation::draw(Jauntlet::SpriteBatch& spriteBatch) {
	animation.update();
	spriteBatch.draw(_destination, animation.getUV(), _textureID);
}