#include "Officer.h"
#include <Jauntlet/Time.h>

Officer::Officer(const glm::vec2& position, const std::string& texture) :
	_position(position),
	_textureID(Jauntlet::ResourceManager::getTexture(texture).id)
{
	_animation.stop(0);
	_animation.play(0, 1, 0.5f);
}

void Officer::draw(const Jauntlet::Camera2D& camera) {
	if (_walking) {
		_position += glm::vec2(300.0f * Jauntlet::Time::getDeltaTime());
	}

	_animation.update();
	_batch.begin();
	_batch.draw(glm::vec4(_position, 64, 64), _animation.getUV(), _textureID);
	_batch.endAndRender();
}

void Officer::walkOffscreen() {
	_animation.play(2, 7, 0.05f);
	_walking = true;
}
bool Officer::isWalking() {
	return _walking;
}