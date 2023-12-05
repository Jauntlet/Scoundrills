/* Purpose: Renders assets tied to the drill
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once
#include <Jauntlet/Rendering/Animation/Animation.h>
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include <Jauntlet/Rendering/Particles/Particle.h>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/SpriteBatch.h>
#include "../interactable/AnimatedPlayerStation.h"

class DrillAssetRenderer {
public:
	DrillAssetRenderer(Jauntlet::Camera2D* _UIcamera);
	// draw the first layer of assets; this renders beneath the walls/floor of the drill
	void drawLayerOne();
	// draw the second layer of assets; this renders above the walls/floor of the drill
	void drawLayerTwo();
	// draw the third layer of assets; this renders above the second layer.
	void drawLayerThree();
	// stop the drill animation
	void startAnimation();
	// start the drill animation
	void stopAnimation();
	
	PlayerStation steeringWheel;
private:
	Jauntlet::SpriteBatch _spriteBatch;
	Jauntlet::Animation _drillAnimation, _boilerAnimation;
	bool _shake = false, _boilerPlaying = true;
	glm::vec2 _smokePos = glm::vec2(64 * 16.5f, -32 + 10 + 16 + 3);

	Jauntlet::Particle _boilerSmoke;
	GLuint _boilerTexture;
};