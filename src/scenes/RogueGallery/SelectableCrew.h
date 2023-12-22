/* Purpose: Simple displayable versions of the Crew you use in the actual game.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once
#include <Jauntlet/Rendering/Animation/Animation.h>
#include <Jauntlet/Rendering/Textures/SpriteBatch.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
class SelectableCrew
{
public:
	SelectableCrew(int playerID, const glm::vec2& position);

	void unSelect();
	void Select();
	bool isSelected();

	bool wasClicked(Jauntlet::Camera2D& camera);

	void draw(Jauntlet::SpriteBatch& spriteBatch);
	
	glm::vec2 position;
private:
	bool _selected = false;
	Jauntlet::Color _color = Jauntlet::Color(255, 255, 255);

	int _playerID;
	GLuint _texture;
	Jauntlet::Animation _animation = Jauntlet::Animation(28);
};

