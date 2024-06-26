/* Purpose: Renders assets tied to the drill
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once
#include <Jauntlet/Rendering/Animation/SpriteAnimation.h>
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include <Jauntlet/Rendering/Particles/ParticleEmitter.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/Rendering/Textures/SpriteBatch.h>
#include "../interactable/AnimatedPlayerStation.h"

class DrillAssetRenderer {
public:
	DrillAssetRenderer(Camera2D* _UIcamera);
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
	SpriteBatch _spriteBatch;
	SpriteAnimation _drillAnimation, _boilerAnimation;
	bool _shake = false, _boilerPlaying = true;
	glm::vec2 _smokePos = glm::vec2(64 * 16.5f, -32 + 10 + 16 + 3);

	ParticleEmitter _boilerSmoke;
	GLuint _boilerTexture;
};