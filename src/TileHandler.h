#pragma once
#include <Jauntlet/TileMap.h>

class TileHandler
{
public:
	TileHandler();

	void addTile(std::string filePath, bool hasCollision);
	void addTileSet(std::string filePath, bool hasCollision);

	void loadFile(std::string filePath);

	void draw();

	// The following functions are to use tilemap functions, but targetting only the active tilemap.
	// Currently multiple tilemaps aren't implemented but they will be shortly - xm
	void updateTile(glm::ivec2 position, unsigned int newID);
	glm::ivec2 WorldPosToTilePos(glm::vec2 position);
	glm::vec2 RoundWorldPos(glm::vec2 position);
private:
	// shift the entire X axis of the tilemap by amount.
	void shiftX(unsigned int amount);
	// Shift the entire Y axis of the tilemap by amount.
	void shiftY(unsigned int amount);
	// Reads all the data from the level data in TileHandler and force feeds it to the tileMap.
	void forceUpdateTileMap();
	
	Jauntlet::TileMap _tileMap;

	// stored for saving the map at the end.
	std::vector<std::string> _tileInfo;
	std::vector<std::vector<unsigned int>> _levelInfo;

	// required to compile tilemap
	Jauntlet::TextureCache _textureCache;
};

