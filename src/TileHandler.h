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
	glm::vec2 RoundWorldPos(glm::vec2 position);
private:
	Jauntlet::TileMap _tileMap;

	// stored for saving the map at the end.
	std::vector<std::string> _tileInfo;
	std::vector<std::vector<std::string>> _levelInfo;

	// required to compile tilemap
	Jauntlet::TextureCache _textureCache;
};

