#include "TileHandler.h"
#include <fstream>
#include <Jauntlet/Errors.h>
#include <sstream>

TileHandler::TileHandler() : _tileMap(_textureCache, 64) {

}

void TileHandler::addTile(std::string filePath, bool hasCollision) {
	_tileMap.Register(filePath);
	_tileInfo.push_back("tile" + filePath + (hasCollision ? " collision" : ""));
}
void TileHandler::addTileSet(std::string filePath, bool hasCollision) {
	Jauntlet::TileSet newSet = Jauntlet::TileSet(filePath);
	_tileMap.Register(newSet);
	_tileInfo.push_back("tileSet" + filePath + (hasCollision ? " collision" : ""));
}

void TileHandler::loadFile(std::string filePath) {

	std::ifstream file;
	file.open(filePath);

	if (file.fail()) {
		Jauntlet::fatalError("Failed to open TileMap " + filePath);
	}
	
	std::string line;
	while (std::getline(file, line, '\n')) {
		if (line == "ENDDEC") {
			break;
		}
		_tileInfo.push_back(line);
	}

	file.close();

	_tileMap.loadTileMap(filePath);
}

void TileHandler::draw() {
	_tileMap.draw();
}

glm::vec2 TileHandler::RoundWorldPos(glm::vec2 position) {
	return _tileMap.RoundWorldPos(position);
}