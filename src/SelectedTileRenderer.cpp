#include "SelectedTileRenderer.h"

SelectedTileRenderer::SelectedTileRenderer() : _drawColor(255, 255, 255), _textureID(0), _tilemap(nullptr) {
	// Empty
}
void SelectedTileRenderer::init(Jauntlet::TileMap* Tilemap) {
	_tilemap = Tilemap;
	_textureID = Jauntlet::ResourceManager::getTexture("Textures/WhiteSquare.png").id;

	_spriteBatch.init();
}

void SelectedTileRenderer::draw(Jauntlet::Camera2D* activeCamera, Jauntlet::InputManager* inputManager) {
	glm::vec2 _selectedTilePos = _tilemap->RoundWorldPos(activeCamera->convertScreenToWorld(inputManager->getMouseCoords()));
	glm::ivec2 _selectedTile = _tilemap->WorldPosToTilePos(_selectedTilePos);
	if (!_tilemap->isValidTilePos(_selectedTile) || _tilemap->tileHasCollision(_selectedTile) || _tilemap->isTileEmpty(_selectedTile)) {
		_drawColor = Jauntlet::Color(255, 0, 0);
	}
	else {
		_drawColor = Jauntlet::Color(0, 255, 0);
	}

	_spriteBatch.begin();

	// draw the selected tile sprite
	_spriteBatch.draw({ _selectedTilePos.x, _selectedTilePos.y, 64, 64 }, _textureID, 0 , _drawColor);

	_spriteBatch.endAndRender();
}