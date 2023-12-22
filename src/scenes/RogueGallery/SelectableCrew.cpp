#include "SelectableCrew.h"
#include <Jauntlet/Errors.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include "../GlobalContext.h"

SelectableCrew::SelectableCrew(int playerID, const glm::vec2& position) :
	_playerID(playerID),
	position(position)
{
	switch (playerID) {
	case (1):
		_texture = Jauntlet::ResourceManager::getTexture("Textures/Criminals/Will Sabot Togue.png").id;
		break;
	case (2):
		_texture = Jauntlet::ResourceManager::getTexture("Textures/Criminals/Sean Arson Burnes.png").id;
		break;
	case (3):
		_texture = Jauntlet::ResourceManager::getTexture("Textures/Criminals/Rob evan truly.png").id;
	default:
		Jauntlet::error("Player ID " + std::to_string(playerID) + " is not a valid ID!");
		break;
	}
	_animation.stop(0);
	_animation.play(0, 1, 0.5f);
	_animation.update();
}

void SelectableCrew::unSelect() {
	_selected = false;
	_animation.play(0, 1, 0.5f);
}
void SelectableCrew::Select() {
	_selected = true;
	_animation.play(12, 19, 0.025f);
}
bool SelectableCrew::isSelected() {
	return _selected;
}

bool SelectableCrew::wasClicked(Jauntlet::Camera2D& camera) {
	glm::vec2 mousecoords = camera.convertScreenToWorld(GlobalContext::inputManager.getMouseCoords());

	return mousecoords.x > position.x && mousecoords.x < position.x + 128 && mousecoords.y > position.y && mousecoords.y < position.y + 128;
}

void SelectableCrew::draw(Jauntlet::SpriteBatch& spriteBatch) {
	_animation.update();
	spriteBatch.draw(glm::vec4(position, 240, 240), _animation.getUV(), _texture);
}