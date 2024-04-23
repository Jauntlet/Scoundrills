#include "SelectedTileRenderer.h"
#include "Jauntlet/Inputs/InputManager.h"
#include "src/interactable/PlayerStation.h"
#include <src/scenes/GlobalContext.h>

SelectedTileRenderer::SelectedTileRenderer(DrillManager* drill, PlayerManager* playerManager) : 
	_drawColor(255, 255, 255), 
	_textureID(Jauntlet::ResourceManager::getTexture("Textures/WhiteSquare.png").id),
	_players(playerManager),
	_drill(drill)
{
	// Empty
}

void SelectedTileRenderer::draw(Camera2D* activeCamera) {
	if (changedPosition(activeCamera) || GlobalContext::inputManager.isKeyPressed(CONTROLLER_FACE_EAST) || GlobalContext::inputManager.isKeyPressed(CONTROLLER_FACE_SOUTH)) {
		glm::vec2 _selectedTilePos = _drill->drillWalls.RoundWorldPos(_lastPosition);

		// if a player is selected, we highlight the tile if its a valid pathfind pos or not
		if (_players->isPlayerSelected()) {
			PlayerStation* station;
			if (_drill->isValidDestination(_selectedTilePos, _players) || ((station = _drill->checkHoveringStation(_lastPosition)) != nullptr && !station->isOccupied())) {
				_drawColor = Color(0, 255, 0);
			} else {
				_drawColor = Color(255, 0, 0);
			}
		} else {
			// player is not selected, tile is white.
			_drawColor = Color(255, 255, 255);
		}

		_spriteBatch.begin();
		
		PlayerStation* station;
		if ((station = _drill->checkHoveringStation(_lastPosition)) != nullptr) {
			_spriteBatch.draw(station->getDisplayBox(), _textureID, 0, _drawColor);
		} else {
			_spriteBatch.draw({ _selectedTilePos.x, _selectedTilePos.y, 64, 64 }, _textureID, 0 , _drawColor);
		}
		_spriteBatch.endAndRender();
	} else {
		_spriteBatch.render();
	}
}

bool SelectedTileRenderer::changedPosition(Camera2D* activeCamera) {
	if (GlobalContext::usingController) {
		if (_lastPosition != activeCamera->getPosition() * (1.0f /activeCamera->getScale())) {
			_lastPosition = activeCamera->getPosition() * (1.0f /activeCamera->getScale());
			return true;
		}
	}
	else {
		if (_lastPosition != activeCamera->convertScreenToWorld(GlobalContext::inputManager.getMouseCoords()) || GlobalContext::inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
			_lastPosition = activeCamera->convertScreenToWorld(GlobalContext::inputManager.getMouseCoords());
			return true;
		}
	}
	return false;
}