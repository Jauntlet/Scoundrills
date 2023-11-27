#include "SelectedTileRenderer.h"

SelectedTileRenderer::SelectedTileRenderer(DrillManager* drill, PlayerManager* playerManager) : 
	_drawColor(255, 255, 255), 
	_textureID(Jauntlet::ResourceManager::getTexture("Textures/WhiteSquare.png").id),
	_players(playerManager),
	_drill(drill)
{
	// Empty
}

void SelectedTileRenderer::draw(Jauntlet::Camera2D* activeCamera, Jauntlet::InputManager* inputManager) {
	if (_lastPosition != activeCamera->convertScreenToWorld(inputManager->getMouseCoords())) {
		_lastPosition = activeCamera->convertScreenToWorld(inputManager->getMouseCoords());
		glm::vec2 _selectedTilePos = _drill->drillWalls.RoundWorldPos(_lastPosition);

		// if a player is selected, we highlight the tile if its a valid pathfind pos or not
		if (_players->isPlayerSelected()) {
			if (_drill->checkHoveringStation(_lastPosition)) {
				_drawColor = Jauntlet::Color(0, 0, 0, 0);
			}
			else if (_drill->isValidDestination(_selectedTilePos, _players)) {
				_drawColor = Jauntlet::Color(0, 255, 0);
			}
			else {
				_drawColor = Jauntlet::Color(255, 0, 0);
			}
		}
		else {
			// player is not selected, tile is white.
			_drawColor = Jauntlet::Color(255, 255, 255);
		}

		_spriteBatch.begin();
		_spriteBatch.draw({ _selectedTilePos.x, _selectedTilePos.y, 64, 64 }, _textureID, 0 , _drawColor);
		_spriteBatch.endAndRender();
	}
	else {
		_spriteBatch.render();
	}
}