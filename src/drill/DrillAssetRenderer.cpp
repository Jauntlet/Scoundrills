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
}

void DrillAssetRenderer::draw() {
	srand(static_cast<unsigned int>(Jauntlet::Time::getTime()));
	_spriteBatch.begin();
	//_spriteBatch.draw({0,(-40 * 64),(24 * 64),(24 * 64)}, Jauntlet::ResourceManager::getTexture("Textures/Drill Head.png").id);
	_spriteBatch.draw({ (3 * 64 + (_shake ? (((float)rand()) / RAND_MAX) * SHAKE_AMOUNT - SHAKE_AMOUNT / 2.0f : 0)),(-42 * 64),(18 * 64),(18 * 64) }, _drillAnimation.getUV(), Jauntlet::ResourceManager::getTexture("Textures/Drill Head.png").id);
	_spriteBatch.end();

	_spriteBatch.render();
	_drillAnimation.update();
}

void DrillAssetRenderer::stopAnimation() {
	_drillAnimation.stop();
	_shake = false;
}

void DrillAssetRenderer::startAnimation() {
	_drillAnimation.play(0, 2, 0.1f);
	_shake = true;
}