#include "SelectedTileRenderer.h"
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

void SelectedTileRenderer::draw(Jauntlet::Camera2D* activeCamera) {
	if (_lastPosition != activeCamera->convertScreenToWorld(GlobalContext::inputManager.getMouseCoords()) || GlobalContext::inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		_lastPosition = activeCamera->convertScreenToWorld(GlobalContext::inputManager.getMouseCoords());
		glm::vec2 _selectedTilePos = _drill->drillWalls.RoundWorldPos(_lastPosition);

		// if a player is selected, we highlight the tile if its a valid pathfind pos or not
		if (_players->isPlayerSelected()) {
			PlayerStation* station;
			if (_drill->isValidDestination(_selectedTilePos, _players) || ((station = _drill->checkHoveringStation(_lastPosition)) != nullptr && !station->isOccupied())) {
				_drawColor = Jauntlet::Color(0, 255, 0);
			} else {
				_drawColor = Jauntlet::Color(255, 0, 0);
			}
		} else {
			// player is not selected, tile is white.
			_drawColor = Jauntlet::Color(255, 255, 255);
		}

		_spriteBatch.begin();
		
		PlayerStation* station;
		if ((station = _drill->checkHoveringStation(_lastPosition)) != nullptr) {
			_spriteBatch.draw(station->getBoundingBox(), _textureID, 0, _drawColor);
		} else {
			_spriteBatch.draw({ _selectedTilePos.x, _selectedTilePos.y, 64, 64 }, _textureID, 0 , _drawColor);
		}
		_spriteBatch.endAndRender();
	} else {
		_spriteBatch.render();
	}
}