#include "DrillAssetRenderer.h"

DrillAssetRenderer::DrillAssetRenderer() {
	// Empty
}

void DrillAssetRenderer::init() {
	_spriteBatch.init();
	_spriteBatch.begin();
	//_spriteBatch.draw({0,(-40 * 64),(24 * 64),(24 * 64)}, Jauntlet::ResourceManager::getTexture("Textures/Drill Head.png").id);
	_spriteBatch.draw({ (3 * 64),(-42 * 64),(18 * 64),(18 * 64) }, Jauntlet::ResourceManager::getTexture("Textures/Drill Head.png").id);
	_spriteBatch.end();
}

void DrillAssetRenderer::draw() {
	_spriteBatch.render();
}