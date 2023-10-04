#include "DrillAssetRenderer.h"
#include <Jauntlet/Time.h>
#include <cstdlib>
#include <iostream>

DrillAssetRenderer::DrillAssetRenderer() {
	// Empty
}

void DrillAssetRenderer::init() {
	_spriteBatch.init();
}

void DrillAssetRenderer::draw(bool isOn) {
	srand(static_cast<unsigned int>(Jauntlet::Time::getTime()));
	_spriteBatch.begin();
	//_spriteBatch.draw({0,(-40 * 64),(24 * 64),(24 * 64)}, Jauntlet::ResourceManager::getTexture("Textures/Drill Head.png").id);
	_spriteBatch.draw({ (3 * 64 + (isOn ? (((float)rand()) / RAND_MAX) * 10.0f - 0.5f : 0)),(-42 * 64),(18 * 64),(18 * 64) }, Jauntlet::ResourceManager::getTexture("Textures/Drill Head.png").id);
	_spriteBatch.end();
	
	_spriteBatch.render();
}