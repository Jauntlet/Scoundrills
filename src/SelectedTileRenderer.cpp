#include "SelectedTileRenderer.h"

SelectedTileRenderer::SelectedTileRenderer(Jauntlet::TileMap* Tilemap, PlayerManager* playerManager) : 
	_drawColor(255, 255, 255), 
	_textureID(Jauntlet::ResourceManager::getTexture("Textures/WhiteSquare.png").id),
	_tilemap(Tilemap),
	_players(playerManager) 
{
	// Empty
}

void SelectedTileRenderer::draw(Jauntlet::Camera2D* activeCamera, Jauntlet::InputManager* inputManager) {
	glm::vec2 _selectedTilePos = _tilemap->RoundWorldPos(activeCamera->convertScreenToWorld(inputManager->getMouseCoords()));
	
	if (_selectedTile != _tilemap->WorldPosToTilePos(_selectedTilePos)) {
		_selectedTile = _tilemap->WorldPosToTilePos(_selectedTilePos);
		
		// if a player is selected, we highlight the tile if its a valid pathfind pos or not
		if (_players->isPlayerSelected()) {
			if (_players->hoveringStation(_selectedTilePos)) {
				_drawColor = Jauntlet::Color(0, 0, 0, 0);
			}
			else if (_players->isValidDestination(_selectedTilePos)) {
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
		// draw the selected tile sprite
		_spriteBatch.draw({ _selectedTilePos.x, _selectedTilePos.y, 64, 64 }, _textureID, 0 , _drawColor);
		_spriteBatch.endAndRender();
	}
	else {
		_spriteBatch.render();
	}
}