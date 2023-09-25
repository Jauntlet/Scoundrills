#include "TileHandler.h"
#include <fstream>
#include <Jauntlet/Errors.h>
#include <sstream>

TileHandler::TileHandler() : _tileMap(_textureCache, 64) {

}

void TileHandler::addTile(std::string filePath) {
	_tileMap.Register(filePath);
}
void TileHandler::addTileSet(std::string filePath) {
	Jauntlet::TileSet newSet = Jauntlet::TileSet(filePath);
	_tileMap.Register(newSet);
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