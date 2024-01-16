/* Purpose: Simple displayable versions of the Crew you use in the actual game.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once

#include <Jauntlet/Rendering/Animation/SpriteAnimation.h>
#include <Jauntlet/Rendering/Textures/SpriteBatch.h>
#include <Jauntlet/UI/UIButton.h>
#include <Jauntlet/UI/UIManager.h>

class SelectableCrew
{
public:
	SelectableCrew(uint8_t playerID, bool isPolice = false);

	void unSelect();
	void Select();
	bool isSelected();

	bool wasClicked(Camera2D& camera);

	uint8_t getPlayerID();

	void draw(SpriteBatch& spriteBatch);
	
	glm::vec2 position;
private:
	bool _selected = false;
	Color _color = Color(255, 255, 255);

	int _playerID;
	GLuint _texture;
	SpriteAnimation _animation = SpriteAnimation(28);
};

