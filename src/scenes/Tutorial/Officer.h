/* Purpose: Renders the police officer used in the tutorial
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Rendering/Textures/SpriteBatch.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/Rendering/Animation/Animation.h>
class Officer {
public:
	Officer(const glm::vec2& position);

	void draw();
private:
	glm::vec2 _position;
	Jauntlet::Animation _animation = Jauntlet::Animation(8);
	Jauntlet::SpriteBatch _batch;
	GLuint _textureID = Jauntlet::ResourceManager::getTexture("Textures/Cops/John Police.png").id;
};