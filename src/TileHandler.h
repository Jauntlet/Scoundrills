#pragma once
#include <Jauntlet/TileMap.h>

class TileHandler
{
public:
	TileHandler();

	void addTile(std::string filePath, bool hasCollision);
	void addTileSet(std::string filePath, bool hasCollision);

	void loadFile(std::string filePath, glm::vec2 offset = glm::vec2(0));
	void saveFile(std::string filePath);

	void draw();

	// increases what index of tile is selected by amount; can be negative.
	void changeSelectedTile(int changeAmount);

	// The following functions are to use tilemap functions, but targetting only the active tilemap.
	// Currently multiple tilemaps aren't implemented but they will be shortly - xm
	void updateTile(glm::ivec2 position);
	glm::ivec2 WorldPosToTilePos(glm::vec2 position);
	glm::vec2 RoundWorldPos(glm::vec2 position);
private:
	// shift the entire X axis of the tilemap by amount.
	void shiftX(unsigned int amount);
	// Shift the entire Y axis of the tilemap by amount.
	void shiftY(unsigned int amount);
	// Reads all the data from the level data in TileHandler and force feeds it to the tileMap.
	void forceUpdateTileMap();
	// Clean empty rows from tilemap
	void cleanTileMap();
	
	Jauntlet::TileMap _tileMap;

	// stored for saving the map at the end.
	std::vector<std::string> _tileInfo;
	std::vector<std::vector<unsigned int>> _levelInfo;

	// for use by user
	unsigned int _selectedTileID;

	// required to compile tilemap
	Jauntlet::TextureCache _textureCache;
};

