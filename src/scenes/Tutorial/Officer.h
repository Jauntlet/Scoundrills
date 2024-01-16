/* Purpose: Renders the police officer used in the tutorial
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Rendering/Animation/SpriteAnimation.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/Rendering/Textures/SpriteBatch.h>
#include <Jauntlet/Rendering/Cameras/Camera2D.h>

class Officer {
public:
	Officer(const glm::vec2& position, const std::string& texture);

	void draw(const Camera2D& camera);

	void walkOffscreen();
	bool isWalking();
private:
	glm::vec2 _position;
	SpriteAnimation _animation = SpriteAnimation(8);
	SpriteBatch _batch;
	GLuint _textureID;

	bool _walking = false;
	float _timeWalking = 0;
};