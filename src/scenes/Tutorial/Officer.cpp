#include "Officer.h"

Officer::Officer(const glm::vec2& position) :
	_position(position)
{
	_animation.stop(0);
	_animation.play(0, 1, 0.5f);
}

void Officer::draw() {
	_animation.update();
	_batch.begin();
	_batch.draw(glm::vec4(_position, 64, 64), _animation.getUV(), _textureID);
	_batch.endAndRender();
}