#include <Jauntlet/Time.h>
#include <cstdlib>
#include "DrillAssetRenderer.h"

const float SHAKE_AMOUNT = 20.0f;

DrillAssetRenderer::DrillAssetRenderer(Jauntlet::Camera2D* _UIcamera) :
	steeringWheel("Textures/missing.png", { 64 * 11.5, -64 * 28, 64, 64 }, { 64 * 11.5, -64 * 28, 64, 64 }, { 0, 0 }),
	_boilerSmoke(_UIcamera, _smokePos, "Textures/smoke.png"),
	_drillAnimation(3),
	_boilerAnimation(6),
	_boilerTexture(Jauntlet::ResourceManager::getTexture("Textures/Boiler.png").id)
{
	_boilerAnimation.play(0, 2, 0.1f);

	Jauntlet::ParticleGrow grow = Jauntlet::ParticleGrow(0.0f, 10.0f);

	_boilerSmoke.addProperty(grow);

	srand(Jauntlet::Time::getTime());
}

void DrillAssetRenderer::drawLayerOne() {
	// Layer one renders beneath the walls / floor of the tilemap
	_spriteBatch.begin();
	
	// draw the drill
        _spriteBatch.draw({
				(3 * 64 + (_shake ? (((float)rand()) / RAND_MAX) * SHAKE_AMOUNT - SHAKE_AMOUNT * 0.5f : 0)),
				(-42 * 64), (18 * 64), (18 * 64)},
            _drillAnimation.getUV(),
            Jauntlet::ResourceManager::getTexture("Textures/Drill Head.png").id);

        _spriteBatch.endAndRender();
	
	if (_shake) {
		_drillAnimation.update();
	}
	
	_boilerAnimation.update();
}
void DrillAssetRenderer::drawLayerTwo() {
	// Layer two renders above the walls / floor of the tilemap
	_spriteBatch.begin();

	// draw the player stations
	steeringWheel.draw(_spriteBatch);
	
	_spriteBatch.draw(glm::vec4(64 * 15.5, -64 * 2, 64 * 2, 96 * 2), _boilerAnimation.getUV(), _boilerTexture);
	
	_spriteBatch.endAndRender();
}
void DrillAssetRenderer::drawLayerThree() {
	_boilerSmoke.update();
	_boilerSmoke.draw();
}

void DrillAssetRenderer::stopAnimation() {
	_drillAnimation.pause();
	_shake = false;

	_boilerAnimation.play(2,5,0.1f,false);
}

void DrillAssetRenderer::startAnimation() {
	_drillAnimation.play(0, 2, 0.1f);
	_shake = true;

	_boilerAnimation.play(0, 2, 0.1f);
}