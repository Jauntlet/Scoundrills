#include <Jauntlet/Errors.h>
#include <Jauntlet/Rendering/Textures/ResourceManager.h>

#include "../GlobalContext.h"
#include "SelectableCrew.h"

SelectableCrew::SelectableCrew(uint8_t playerID, bool isPolice) :
	_playerID(playerID)
{
	_animation.stop(0);
	_animation.play(0, 1, 0.5f);
	
	_texture = Jauntlet::ResourceManager::getTexture(GlobalContext::playerIDtoTexture(playerID, isPolice)).id;
	// select the first 3 players
	playerID < 4 ? Select() : unSelect();
}

void SelectableCrew::unSelect() {
	_selected = false;
	_animation.stop(0);
	_animation.play(0, 1, 0.5f);
	_color = Color(100, 100, 100);
}
void SelectableCrew::Select() {
	_selected = true;
	_animation.stop(2);
	_animation.play(2, 9, 0.1f);
	_color = Color(255, 255, 255);
}
bool SelectableCrew::isSelected() {
	return _selected;
}

bool SelectableCrew::wasClicked(Camera2D& camera) {
	if (!GlobalContext::inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		return false;
	}

	glm::vec2 mousecoords = camera.convertScreenToWorld(GlobalContext::inputManager.getMouseCoords());

	return mousecoords.x > position.x && mousecoords.x < position.x + 240 && mousecoords.y > position.y && mousecoords.y < position.y + 240;
}

uint8_t SelectableCrew::getPlayerID() {
	return _playerID;
}

void SelectableCrew::draw(SpriteBatch& spriteBatch) {
	_animation.update();
	spriteBatch.draw(glm::vec4(position, 240, 240), _animation.getUV(), _texture, 0, _color);
}