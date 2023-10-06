#include <Jauntlet/Time.h>
#include <cstdlib>
#include <iostream>

#include "DrillAssetRenderer.h"

const float SHAKE_AMOUNT = 20.0f;

DrillAssetRenderer::DrillAssetRenderer() {
	// Empty
}

void DrillAssetRenderer::init() {
	_spriteBatch.init();

	_drillAnimation = Jauntlet::Animation(3);

	srand(Jauntlet::Time::getTime());
}

void DrillAssetRenderer::draw() {
	_spriteBatch.begin();
	_spriteBatch.draw({ (3 * 64 + (_shake ? (((float)rand()) / RAND_MAX) * SHAKE_AMOUNT - SHAKE_AMOUNT / 2.0f : 0)),(-42 * 64),(18 * 64),(18 * 64) }, _drillAnimation.getUV(), Jauntlet::ResourceManager::getTexture("Textures/Drill Head.png").id);
	_spriteBatch.end();

	_spriteBatch.render();
	
	if (_shake) {
		_drillAnimation.update();
	}
}

void DrillAssetRenderer::stopAnimation() {
	_drillAnimation.pause();
	_shake = false;
}

void DrillAssetRenderer::startAnimation() {
	_drillAnimation.play(0, 2, 0.1f);
	_shake = true;
}