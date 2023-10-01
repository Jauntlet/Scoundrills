#include "TileHandler.h"
#include <fstream>
#include <iostream>
#include <Jauntlet/Errors.h>
#include <Jauntlet/JMath.h>
#include <sstream>


TileHandler::TileHandler() {
}

void TileHandler::addTile(std::string filePath, bool hasCollision) {
	// TODO: Optimize so this function adds to all tilemaps, but only includes them at save if they were used on the tilemap.
	for (int i = 0; i < _tileMaps.size(); i++) {
		_tileMaps[i].Register(filePath);
	}
	_tileInfo.push_back("tile" + filePath + (hasCollision ? " collision" : ""));
}
void TileHandler::addTileSet(std::string filePath, bool hasCollision) {
	// TODO: Optimize so this function adds to all tilemaps, but only includes them at save if they were used on the tilemap.
	// this one is ESPECIALLY inefficent as it copies this tileset a bunch until it is eventually stored inside of the tilemap.
	Jauntlet::TileSet newSet = Jauntlet::TileSet(filePath);
	for (int i = 0; i < _tileMaps.size(); i++) {
		_tileMaps[i].Register(newSet);
	}
	_tileInfo.push_back("tileSet" + filePath + (hasCollision ? " collision" : ""));
}

void TileHandler::loadFile(std::string filePath, glm::vec2 offset /*= glm::vec2(0)*/) {
	_tileMaps.emplace_back(_textureCache, 64);
	_levelInfos.push_back(std::vector<std::vector<unsigned int>>());
	_selectedTileMap = _tileMaps.size() - 1;

	std::ifstream file;

	file.open(filePath);

	if (file.fail()) {
		std::cout << "Failed to open TileMap " << filePath << std::endl;
		file.close();
		return;
	}

	std::string line;
	while (std::getline(file, line, '\n')) {
		if (line == "ENDDEC") {
			break;
		}
		_tileInfo.push_back(line);
	}

	int y = 0;
	_levelInfos[_selectedTileMap].push_back(std::vector<unsigned int>());
	while (std::getline(file, line, '\n')) {
		std::stringstream ss(line);
		while (std::getline(ss, line, ',')) {
			_levelInfos[_selectedTileMap][y].push_back(stoi(line));
		}
		y++;
		_levelInfos[_selectedTileMap].push_back(std::vector<unsigned int>());
	}

	file.close();

	_tileMaps[_selectedTileMap].loadTileMap(filePath, offset.x, offset.y);
}
void TileHandler::saveFile(std::string filePath) {
	// start by cleaning the tilemap
	cleanTileMaps();
	
	/*std::ofstream file(filePath);

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
	}*/

	/*file.close();*/
}
void TileHandler::draw() {
	for (int i = 0; i < _tileMaps.size(); i++) {
		_tileMaps[i].draw();
	}
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
std::string TileHandler::getSelectedTileTexture() {
	if (_selectedTileID == 0) {
		return "Textures/WhiteSquare.png";
	}
	return JMath::Split(_tileInfo[_selectedTileID - 1], '"')[1];
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
	
	if (_tileMaps[_selectedTileMap].getTileID(position) != _selectedTileID) {

		while (position.y >= _levelInfos[_selectedTileMap].size() - 1) {
			_levelInfos[_selectedTileMap].push_back(std::vector<unsigned int>());
			_levelInfos[_selectedTileMap][_levelInfos[_selectedTileMap].size() - 1].push_back(0);
		}

		while (position.x >= _levelInfos[_selectedTileMap][position.y].size()) {
			_levelInfos[_selectedTileMap][position.y].push_back(0);
		}

		_levelInfos[_selectedTileMap][position.y][position.x] = _selectedTileID;
		_tileMaps[_selectedTileMap].UpdateTile(position, _selectedTileID);
	}
}

glm::ivec2 TileHandler::WorldPosToTilePos(glm::vec2 position) {
	return _tileMaps[_selectedTileMap].WorldPosToTilePos(position);
}

glm::vec2 TileHandler::RoundWorldPos(glm::vec2 position) {
	return _tileMaps[_selectedTileMap].RoundWorldPos(position);
}

void TileHandler::shiftX(unsigned int amount) {
	_tileMaps[_selectedTileMap].AddOffset(glm::vec2(-64 * (int)amount, 0));

	for (int y = 0; y < _levelInfos[_selectedTileMap].size(); y++) {
		for (int i = 0; i < amount; i++) {
			_levelInfos[_selectedTileMap][y].insert(_levelInfos[_selectedTileMap][y].begin(), 0);
		}
	}
	forceUpdateTileMap();
}
void TileHandler::shiftY(unsigned int amount) {
	_tileMaps[_selectedTileMap].AddOffset(glm::vec2(0, 64 * amount));

	for (; amount > 0; amount--) {
		_levelInfos[_selectedTileMap].insert(_levelInfos[_selectedTileMap].begin(), std::vector<unsigned int>(0));
	}

	// we make i-nth element the same size as the next i-nth element so that when we update the tilemap,
	// we overwrite old data. This is cleaned up later during saving -xm
	for (int y = _levelInfos[_selectedTileMap].size() - 2; y >= 0; y--) {
		while (_levelInfos[_selectedTileMap][y].size() < _levelInfos[_selectedTileMap][y + 1].size()) {
			_levelInfos[_selectedTileMap][y].push_back(0);
		}
	}
	forceUpdateTileMap();
}

void TileHandler::forceUpdateTileMap() {
	for (int y = 0; y < _levelInfos[_selectedTileMap].size(); y++) {
		for (int x = 0; x < _levelInfos[_selectedTileMap][y].size(); x++) {
			_tileMaps[_selectedTileMap].UpdateTile(glm::vec2(x, y), _levelInfos[_selectedTileMap][y][x]);
		}
	}
}

void TileHandler::cleanTileMaps() {
	for (int i = 0; i < _tileMaps.size(); i++) {
		// check beginning of X axis
		int clean = true;
		while (clean) {
			for (int y = 0; y < _levelInfos[i].size(); y++) {
				if (_levelInfos[i][y].size() > 0 && _levelInfos[i][y][0] != 0) {
					clean = false;
					break;
				}
			}
			if (clean) {
				_tileMaps[i].AddOffset(glm::vec2(64, 0));
				for (int y = 0; y < _levelInfos[i].size(); y++) {
					if (_levelInfos[i][y].size() > 0) {
						_levelInfos[i][y].erase(_levelInfos[i][y].begin());
					}
					else {
						_levelInfos[i].erase(_levelInfos[i].begin() + y);
						if (_levelInfos[i].size() == 0) {
							return;
						}
					}
				}
			}
		}

		// check end of X axis
		clean = true;
		while (clean) {
			for (int y = 0; y < _levelInfos[i].size(); y++) {
				if (_levelInfos[i][y].size() > 0 && _levelInfos[i][y][_levelInfos[i][y].back()] != 0) {
					clean = false;
					break;
				}
			}
			if (clean) {
				for (int y = 0; y < _levelInfos[i].size(); y++) {
					if (_levelInfos[i][y].size() > 0) {
						_levelInfos[i][y].pop_back();
					}
					else {
						_levelInfos[i].erase(_levelInfos[i].begin() + y);
						if (_levelInfos[i].size() == 0) {
							return;
						}
					}
				}
			}
		}
		// check beginning of Y axis
		clean = true;
		while (clean) {
			for (int x = 0; x < _levelInfos[i][0].size(); x++) {
				if (_levelInfos[i][0][x] != 0) {
					clean = false;
					break;
				}
			}
			if (clean) {
				_levelInfos[i].erase(_levelInfos[i].begin());
			}
		}

		// check end of Y axis
		clean = true;
		while (clean) {
			for (int x = 0; x < _levelInfos[i][_levelInfos[i].size() - 1].size(); x++) {
				if (_levelInfos[i][_levelInfos[i].size() - 1][x] != 0) {
					clean = false;
					break;
				}
			}
			if (clean) {
				_levelInfos[i].pop_back();
			}
		}
	}
}