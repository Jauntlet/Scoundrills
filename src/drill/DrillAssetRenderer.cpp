#include <Jauntlet/Time.h>
#include <cstdlib>
#include <iostream>

#include "DrillAssetRenderer.h"

const float SHAKE_AMOUNT = 20.0f;

DrillAssetRenderer::DrillAssetRenderer() :
	steeringWheel("Textures/missing.png", { 64 * 11.5, -64 * 28, 64, 64 }, { 64 * 11.5, -64 * 28, 64, 64 }, { 0, 0 }) 
{

	_drillAnimation = Jauntlet::Animation(3);

	srand(Jauntlet::Time::getTime());
}

void DrillAssetRenderer::drawLayerOne() {
	// Layer one renders beneath the walls / floor of the tilemap
	_spriteBatch.begin();
	
	// draw the drill
	_spriteBatch.draw({ (3 * 64 + (_shake ? (((float)rand()) / RAND_MAX) * SHAKE_AMOUNT - SHAKE_AMOUNT / 2.0f : 0)),(-42 * 64),(18 * 64),(18 * 64) }, _drillAnimation.getUV(), Jauntlet::ResourceManager::getTexture("Textures/Drill Head.png").id);
	
	_spriteBatch.endAndRender();
	
	if (_shake) {
		_drillAnimation.update();
	}
}
void DrillAssetRenderer::drawLayerTwo() {
	// Layer two renders above the walls / floor of the tilemap
	_spriteBatch.begin();

	// draw the steering wheel
	steeringWheel.draw(_spriteBatch);

	_spriteBatch.endAndRender();
}

void DrillAssetRenderer::stopAnimation() {
	_drillAnimation.pause();
	_shake = false;
}

void DrillAssetRenderer::startAnimation() {
	_drillAnimation.play(0, 2, 0.1f);
	_shake = true;
}