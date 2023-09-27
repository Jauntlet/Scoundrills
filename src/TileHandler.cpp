#include "TileHandler.h"
#include <fstream>
#include <Jauntlet/Errors.h>
#include <sstream>

#include <iostream> // REMOVE

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

	int y = 0;
	_levelInfo.push_back(std::vector<unsigned int>());
	while (std::getline(file, line, '\n')) {
		std::stringstream ss(line);
		while (std::getline(ss, line, ',')) {
			_levelInfo[y].push_back(stoi(line));
		}
		y++;
		_levelInfo.push_back(std::vector<unsigned int>());
	}

	file.close();

	_tileMap.loadTileMap(filePath);
}

void TileHandler::draw() {
	_tileMap.draw();
}

void TileHandler::updateTile(glm::ivec2 position, unsigned int newID) {
	std::cout << "Position: " << position.x << ", " << position.y << std::endl;

	if (position.x < 0) {
		std::cout << "Ran shiftX" << std::endl;
		shiftX(std::abs(position.x));
	}
	if (position.y < 0) {
		std::cout << "Ran shiftY" << std::endl;
		shiftY(std::abs(position.y));
	}
	
	if (_tileMap.getTileID(position) != newID) {
		_levelInfo[position.y][position.x] = newID;
		_tileMap.UpdateTile(position, newID);
	}
}

glm::ivec2 TileHandler::WorldPosToTilePos(glm::vec2 position) {
	return _tileMap.WorldPosToTilePos(position);
}

glm::vec2 TileHandler::RoundWorldPos(glm::vec2 position) {
	return _tileMap.RoundWorldPos(position);
}

void TileHandler::shiftX(unsigned int amount) {
	_tileMap.AddOffset(glm::vec2(-64 * amount, 0));

	for (int y = 0; y < _levelInfo.size(); y++) {
		_levelInfo[y].insert(_levelInfo[y].begin(), 0);
	}
	forceUpdateTileMap();
}
void TileHandler::shiftY(unsigned int amount) {
	_tileMap.AddOffset(glm::vec2(0, 32 * amount));

	for (; amount > 0; amount--) {
		_levelInfo.insert(_levelInfo.begin(), std::vector<unsigned int>(0));
	}

	// we make i-nth element the same size as the next i-nth element so that when we update the tilemap,
	// we overwrite old data. This is cleaned up later during saving -xm
	for (int y = _levelInfo.size() - 2; y >= 0; y--) {
		while (_levelInfo[y].size() < _levelInfo[y + 1].size()) {
			_levelInfo[y].push_back(0);
		}
	}
	forceUpdateTileMap();
}

void TileHandler::forceUpdateTileMap() {
	for (int y = 0; y < _levelInfo.size(); y++) {
		for (int x = 0; x < _levelInfo[y].size(); x++) {
			_tileMap.UpdateTile(glm::vec2(x, y), _levelInfo[y][x]);
		}
	}
}