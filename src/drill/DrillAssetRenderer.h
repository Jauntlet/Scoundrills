/* Purpose: Renders assets tied to the drill
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once
#include <Jauntlet/Rendering/Animation/Animation.h>
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include <Jauntlet/Rendering/Particles/Particle.h>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/SpriteBatch.h>
#include "PlayerStation.h"

class DrillAssetRenderer {
public:
	DrillAssetRenderer(Jauntlet::Camera2D* _UIcamera);
	// draw the first layer of assets; this renders beneath the walls/floor of the drill
	void drawLayerOne();
	// draw the second layer of assets; this renders above the walls/floor of the drill
	void drawLayerTwo();
	// stop the drill animation
	void startAnimation();
	// start the drill animation
	void stopAnimation();
	
	PlayerStation steeringWheel;
	PlayerStation boiler;
private:
	Jauntlet::SpriteBatch _spriteBatch;
	Jauntlet::Animation _drillAnimation;
	bool _shake = false;

	Jauntlet::Particle _boilerSmoke;
	GLuint _boilerTexture;
	glm::vec2 _smokePos = glm::vec2(0);
};