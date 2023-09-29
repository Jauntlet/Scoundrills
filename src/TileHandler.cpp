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

void TileHandler::loadFile(std::string filePath, glm::vec2 offset /*= glm::vec2(0)*/) {

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

	_tileMap.loadTileMap(filePath, offset.x, offset.y);
}
void TileHandler::saveFile(std::string filePath) {
	// start by cleaning the tilemap
	cleanTileMap();
	
	std::ofstream file(filePath);

	for (int i = 0; i < _tileInfo.size(); i++) {
		file << _tileInfo[i] << std::endl;
	}
	file << "ENDDEC" << std::endl;
	for (int y = 0; y < _levelInfo.size(); y++) {
		for (int x = 0; x < _levelInfo[y].size(); x++) {
			if (x + 1 == _levelInfo[y].size()) {
				file << _levelInfo[y][x] << std::endl;
			}
			else {
				file << _levelInfo[y][x] << ",";
			}
		}
	}

	file.close();
}
void TileHandler::draw() {
	_tileMap.draw();
}
void TileHandler::setSelectedTile(unsigned int index) {
	 (index > _tileInfo.size())? _selectedTileID = _tileInfo.size(): _selectedTileID = index;
}
void TileHandler::changeSelectedTile(int changeAmount) {
	// this could be changed to a while statement if we were expecting changes of more than one. -xm
	changeAmount = std::max(-1, std::min(changeAmount, 1));

	if (_selectedTileID == 0 && changeAmount == -1) {
		_selectedTileID = _tileInfo.size();
		return;
	}
	if (_selectedTileID == _tileInfo.size() && changeAmount == 1) {
		_selectedTileID = 0;
		return;
	}
	_selectedTileID += changeAmount;
}

void TileHandler::updateTile(glm::ivec2 position) {

	if (position.x < 0) {
		shiftX(std::abs(position.x));
		position.x = 0;
	}
	if (position.y < 0) {
		shiftY(std::abs(position.y));
		position.y = 0;
	}
	
	if (_tileMap.getTileID(position) != _selectedTileID) {

		while (position.y >= _levelInfo.size() - 1) {
			_levelInfo.push_back(std::vector<unsigned int>());
			_levelInfo[_levelInfo.size() - 1].push_back(0);
		}

		while (position.x >= _levelInfo[position.y].size()) {
			_levelInfo[position.y].push_back(0);
		}

		_levelInfo[position.y][position.x] = _selectedTileID;
		_tileMap.UpdateTile(position, _selectedTileID);
	}
}

glm::ivec2 TileHandler::WorldPosToTilePos(glm::vec2 position) {
	return _tileMap.WorldPosToTilePos(position);
}

glm::vec2 TileHandler::RoundWorldPos(glm::vec2 position) {
	return _tileMap.RoundWorldPos(position);
}

void TileHandler::shiftX(unsigned int amount) {
	_tileMap.AddOffset(glm::vec2(-64 * (int)amount, 0));

	for (int y = 0; y < _levelInfo.size(); y++) {
		for (int i = 0; i < amount; i++) {
			_levelInfo[y].insert(_levelInfo[y].begin(), 0);
		}
	}
	forceUpdateTileMap();
}
void TileHandler::shiftY(unsigned int amount) {
	_tileMap.AddOffset(glm::vec2(0, 64 * amount));

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

void TileHandler::cleanTileMap() {
	// check beginning of X axis
	int clean = true;
	while (clean) {
		for (int y = 0; y < _levelInfo.size(); y++) {
			if (_levelInfo[y].size() > 0 && _levelInfo[y][0] != 0) {
				clean = false;
				break;
			}
		}
		if (clean) {
			_tileMap.AddOffset(glm::vec2(64, 0));
			for (int y = 0; y < _levelInfo.size(); y++) {
				if (_levelInfo[y].size() > 0) {
					_levelInfo[y].erase(_levelInfo[y].begin());
				}
				else {
					_levelInfo.erase(_levelInfo.begin() + y);
					if (_levelInfo.size() == 0) {
						return;
					}
				}
			}
		}
	}

	// check end of X axis
	clean = true;
	while (clean) {
		for (int y = 0; y < _levelInfo.size(); y++) {
			if (_levelInfo[y].size() > 0 && _levelInfo[y][_levelInfo[y].size() - 1] != 0) {
				clean = false;
				break;
			}
		}
		if (clean) {
			for (int y = 0; y < _levelInfo.size(); y++) {
				if (_levelInfo[y].size() > 0) {
					_levelInfo[y].pop_back();
				}
				else {
					_levelInfo.erase(_levelInfo.begin() + y);
					if (_levelInfo.size() == 0) {
						return;
					}
				}
			}
		}
	}
	// check beginning of Y axis
	clean = true;
	while (clean) {
		for (int x = 0; x < _levelInfo[0].size(); x++) {
			if (_levelInfo[0][x] != 0) {
				clean = false;
				break;
			}
		}
		if (clean) {
			_levelInfo.erase(_levelInfo.begin());
		}
	}

	// check end of Y axis
	clean = true;
	while (clean) {
		for (int x = 0; x < _levelInfo[_levelInfo.size() - 1].size(); x++) {
			if (_levelInfo[_levelInfo.size() - 1][x] != 0) {
				clean = false;
				break;
			}
		}
		if (clean) {
			_levelInfo.pop_back();
		}
	}
}