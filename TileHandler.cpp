#include "TileHandler.h"

TileHandler::TileHandler() : _currentChar(0), _tileMap(_textureCache, 64) {

}

void TileHandler::addTile(std::string filePath) {
	_tileMap.registerTile(_currentChar, filePath);
	_currentChar++;
}
void TileHandler::addTileSet(std::string filePath) {

//	_tileMap.registerTileSet(_currentChar, );
}
void TileHandler::addFunction(std::function<void(int, int)> function) {

}