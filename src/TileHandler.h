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

	void setSelectedTile(unsigned int index);//set the tile rather than change it -teehee aa

	// increases what index of tile is selected by amount; can be negative.
	void changeSelectedTile(int changeAmount);
	// gets the filepath of the selected texture
	std::string getSelectedTileTexture();

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
	void cleanTileMaps();
	
	std::vector<Jauntlet::TileMap> _tileMaps;

	// stored for saving the map at the end.
	std::vector<std::string> _tileInfo;
	std::vector<std::vector<std::vector<unsigned int>>> _levelInfos; // totally not me making a 4D array to hold level information -xm

	// for use by user
	unsigned int _selectedTileMap = 0;
	unsigned int _selectedTileID = 1;

	// required to compile tilemap
	Jauntlet::TextureCache _textureCache;
};

