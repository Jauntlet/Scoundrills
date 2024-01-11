/* Purpose: Renders the police officer used in the tutorial
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Rendering/Animation/Animation.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/Rendering/Textures/SpriteBatch.h>
#include <Jauntlet/Rendering/Cameras/Camera2D.h>

class Officer {
public:
	Officer(const glm::vec2& position, const std::string& texture);

	void draw(const Jauntlet::Camera2D& camera);

	void walkOffscreen();
	bool isWalking();
private:
	glm::vec2 _position;
	Jauntlet::Animation _animation = Jauntlet::Animation(8);
	Jauntlet::SpriteBatch _batch;
	GLuint _textureID;

	bool _walking = false;
	float _timeWalking = 0;
};