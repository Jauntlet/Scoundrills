#include "TileHandler.h"

TileHandler::TileHandler() : _tileMap(_textureCache, 64) {

}

void TileHandler::addTile(std::string filePath) {
	_tileMap.Register(filePath);
}
void TileHandler::addTileSet(std::string filePath) {
	Jauntlet::TileSet newSet = Jauntlet::TileSet(filePath);
	_tileMap.Register(newSet);
}

void TileHandler::loadLevel(std::string filePath) {
	_tileMap.loadTileMap(filePath);
}

void TileHandler::draw() {
	_tileMap.draw();
}